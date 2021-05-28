#ifndef GUESTPIXELBUFFERSENDER_H
#define GUESTPIXELBUFFERSENDER_H

#include "../guestpixelbuffer.h"
#include <unistd.h>

class GuestPixelBufferSender {
public:
	GuestPixelBufferSender(int socketFd) : m_socketFd(socketFd) {};

        bool spawnWindow(GuestWindowSpawnCommand& gwsc)
        {
            if (this->m_socketFd < 0)
                return false;

            const unsigned long long headerSend = write(this->m_socketFd, &gwsc.header, sizeof(gwsc.header));
            if (headerSend != sizeof(gwsc.header))
                return false;

            const unsigned long long dataSend = write(this->m_socketFd, &gwsc.data, sizeof(gwsc.data));
            if (dataSend != sizeof(gwsc.data))
                return false;

            return true;
        }

        bool destroyWindow(GuestWindowDestroyCommand& gwdc)
        {
            if (this->m_socketFd < 0)
                return false;

            const unsigned long long sent = write(this->m_socketFd, &gwdc, sizeof(gwdc));
            if (sent != sizeof(gwdc))
                return false;

            return true;
        }

	bool redrawCommand(GuestPixelBufferRedrawCommand& gpbrc)
	{
		if (this->m_socketFd < 0)
			return false;

		const unsigned long long headerSend = write(this->m_socketFd, &gpbrc.header, sizeof(gpbrc.header));
		if (headerSend != sizeof(gpbrc.header))
			return false;

		const unsigned long long pixelSend = write(this->m_socketFd, gpbrc.pixelBuffer, gpbrc.header.data.bufferSize);
		if (pixelSend != gpbrc.header.data.bufferSize)
			return false;

		return true;
	};

private:
	int m_socketFd = -1;
};

#endif
