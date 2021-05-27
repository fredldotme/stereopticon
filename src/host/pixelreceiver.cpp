#include "pixelreceiver.h"


void PixelReceiver::spawnWindow(GuestWindowSpawnData& spawnData)
{
    // TODO
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