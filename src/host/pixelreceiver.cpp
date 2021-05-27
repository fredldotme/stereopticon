#include "pixelreceiver.h"


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
    // TODO
}

bool PixelReceiver::windowExists(wayland_window_id_t& windowId)
{
    const bool mapKeyExists = this->m_waylandWindowIdMap.find(windowId) !=
                              this->m_waylandWindowIdMap.end();
    return mapKeyExists;
}