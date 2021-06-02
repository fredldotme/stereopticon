#ifndef GUESTPIXELBUFFERSENDER_H
#define GUESTPIXELBUFFERSENDER_H

#include "../guestpixelbuffer.h"
#include "../transport.h"

#include <unistd.h>

class GuestPixelBufferSender {
public:
    GuestPixelBufferSender(Transport* transport) : m_transport(transport) {};

    bool spawnWindow(GuestWindowSpawnCommand& gwsc)
    {
        if (!this->m_transport)
            return false;

        const unsigned long long headerSend = this->m_transport->write(&gwsc.header, sizeof(gwsc.header));
        if (headerSend != sizeof(gwsc.header))
            return false;

        const unsigned long long dataSend = this->m_transport->write(&gwsc.data, sizeof(gwsc.data));
        if (dataSend != sizeof(gwsc.data))
            return false;

        return true;
    }

    bool destroyWindow(GuestWindowDestroyCommand& gwdc)
    {
        if (!this->m_transport)
            return false;

        const unsigned long long sent = this->m_transport->write(&gwdc, sizeof(gwdc));
        if (sent != sizeof(gwdc))
            return false;

        return true;
    }

    bool redrawCommand(GuestPixelBufferRedrawCommand& gpbrc)
    {
        if (!this->m_transport)
            return false;

        const unsigned long long headerSend = this->m_transport->write(&gpbrc.header, sizeof(gpbrc.header));
        if (headerSend != sizeof(gpbrc.header))
            return false;

        const unsigned long long pixelSend = this->m_transport->write(gpbrc.pixelBuffer, gpbrc.header.data.bufferSize);
        if (pixelSend != gpbrc.header.data.bufferSize)
            return false;

        return true;
    };

private:
    Transport* m_transport = nullptr;
};

#endif
