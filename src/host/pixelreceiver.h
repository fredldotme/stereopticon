#ifndef PIXELRECEIVER_H
#define PIXELRECEIVER_H

#include <map>
#include "../common/receiver/guestpixelbufferreceiver.hpp"

class PixelReceiver : public PixelBufferHandler
{
public:
    void spawnWindow(GuestWindowSpawnData& spawnData) override;
    void receiveRedraw(GuestPixelBufferRedrawCommand& command) override;
    bool windowExists(wayland_window_id_t& windowId) override;

private:
    std::map<wayland_window_id_t, int> m_waylandWindowIdMap;
};

#endif