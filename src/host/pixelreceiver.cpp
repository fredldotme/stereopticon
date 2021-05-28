#include "pixelreceiver.h"
#include "../common/guestpixelbuffer.h"

static int getChannelsForFormat(PixelFormat& format)
{
    switch (format) {
    case PixelFormat::FORMAT_RGBA8888:
    case PixelFormat::FORMAT_ARGB8888:
        return 4;
    default:
        return 3;
    }
}

static int getSdlPixelFormatForPixelFormat(PixelFormat& format)
{
    switch (format) {
    case PixelFormat::FORMAT_RGBA8888:
        return SDL_PIXELFORMAT_RGBA8888;
    case PixelFormat::FORMAT_ARGB8888:
        return SDL_PIXELFORMAT_ARGB8888;
    default:
        return SDL_PIXELFORMAT_RGB444;
    }
}

void PixelReceiver::run()
{
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            break;

        // Update dirty window contents
        for (auto it = this->m_waylandWindowIdMap.begin(); it != this->m_waylandWindowIdMap.end(); it++) {
            if (!it->second->dirty)
                continue;

            SDL_Renderer* renderer = it->second->renderer;
            SDL_Texture* texture = it->second->texture;

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);

            it->second->dirty = false;
        }
    }
}

void PixelReceiver::spawnWindow(GuestWindowSpawnCommand& spawnData)
{
    if (this->windowExists(spawnData.header.windowId))
        return;

    auto hostWindow = std::make_shared<HostWindowPair>();
    hostWindow->window = SDL_CreateWindow("",
                                         spawnData.data.x,
                                         spawnData.data.y,
                                         spawnData.data.width,
                                         spawnData.data.height,
                                         SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL);
    if (!hostWindow->window)
        return;

    hostWindow->renderer = SDL_CreateRenderer(hostWindow->window,
                                              -1,
                                              SDL_RENDERER_ACCELERATED);

    if (!hostWindow->renderer)
        return;

    this->m_waylandWindowIdMap.insert({spawnData.header.windowId, hostWindow});
}

void PixelReceiver::destroyWindow(GuestWindowDestroyCommand& command)
{
    if (!this->windowExists(command.header.windowId))
        return;

    // Destructing the HostWindowPair destroys the window on the host side.
    this->m_waylandWindowIdMap.erase(command.header.windowId);
}

void PixelReceiver::receiveRedraw(GuestPixelBufferRedrawCommand& command)
{
    if (!this->windowExists(command.header.header.windowId))
        return;

    const int channels = getChannelsForFormat(command.header.data.format);
    auto hostWindow = this->m_waylandWindowIdMap.find(command.header.header.windowId)->second;

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void*)command.pixelBuffer,
                command.header.data.width,
                command.header.data.height,
                8 * channels,
                command.header.data.width * channels,
                0x0000FF00,
                0x00FF0000,
                0xFF000000,
                0x000000FF);

    if (!surface)
        return;

    // Create a texture for uploading the window content
    if (!hostWindow->texture) {
        int windowWidth = 0;
        int windowHeight = 0;
        SDL_GetWindowSize(hostWindow->window, &windowWidth, &windowHeight);
        hostWindow->texture = SDL_CreateTexture(
            hostWindow->renderer,
            getSdlPixelFormatForPixelFormat(command.header.data.format),
            SDL_TEXTUREACCESS_STREAMING,
            windowWidth,
            windowHeight);
        SDL_SetTextureBlendMode(hostWindow->texture, SDL_BLENDMODE_BLEND);
    }

    // Update only the dirty area
    SDL_Rect dirtyArea;
    dirtyArea.x = command.header.data.offsetX;
    dirtyArea.y = command.header.data.offsetY;
    dirtyArea.w = command.header.data.width;
    dirtyArea.h = command.header.data.height;

    SDL_UpdateTexture(hostWindow->texture, &dirtyArea, (void*)command.pixelBuffer, channels * 8);

    // Mark as dirty
    hostWindow->dirty = true;
}

bool PixelReceiver::windowExists(wayland_window_id_t& windowId)
{
    const bool mapKeyExists = this->m_waylandWindowIdMap.find(windowId) !=
                              this->m_waylandWindowIdMap.end();
    return mapKeyExists;
}
