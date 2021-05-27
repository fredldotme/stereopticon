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

void PixelReceiver::run()
{
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            break;

        for (auto it = this->m_waylandWindowIdMap.begin(); it != this->m_waylandWindowIdMap.end(); it++) {
            SDL_Window* window = it->second->window;
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
                                              0);

    if (!hostWindow->renderer)
        return;

    this->m_waylandWindowIdMap.insert({spawnData.header.windowId, hostWindow});
}

void PixelReceiver::receiveRedraw(GuestPixelBufferRedrawCommand& command)
{
    if (this->windowExists(command.header.header.windowId))
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

    // Let's override the obsolete back buffer with a new frame
    if (hostWindow->backBuffer)
        SDL_FreeSurface(hostWindow->backBuffer);

    // But if there is no current frontBuffer, then display that immediately
    if (!hostWindow->frontBuffer)
        hostWindow->frontBuffer = surface;
    else
        hostWindow->backBuffer = surface;

    hostWindow->dirty = true;
}

bool PixelReceiver::windowExists(wayland_window_id_t& windowId)
{
    const bool mapKeyExists = this->m_waylandWindowIdMap.find(windowId) !=
                              this->m_waylandWindowIdMap.end();
    return mapKeyExists;
}
