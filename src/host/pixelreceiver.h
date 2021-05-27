#ifndef PIXELRECEIVER_H
#define PIXELRECEIVER_H

#include <map>
#include <memory>
#include <SDL.h>
#include "../common/receiver/guestpixelbufferreceiver.hpp"

struct HostWindowPair
{
    HostWindowPair() {}
    ~HostWindowPair()
    {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        if (backBuffer) {
            SDL_FreeSurface(backBuffer);
            backBuffer = nullptr;
        }
        if (frontBuffer) {
            SDL_FreeSurface(frontBuffer);
            frontBuffer = nullptr;
        }
    }
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Surface* frontBuffer = nullptr;
    SDL_Surface* backBuffer = nullptr;
    bool dirty = false;
};

class PixelReceiver : public PixelBufferHandler
{
public:
    void spawnWindow(GuestWindowSpawnCommand& spawnData) override;
    void receiveRedraw(GuestPixelBufferRedrawCommand& command) override;
    bool windowExists(wayland_window_id_t& windowId) override;

private:
    std::map<wayland_window_id_t, std::shared_ptr<HostWindowPair> > m_waylandWindowIdMap;
};

#endif