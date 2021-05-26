#ifndef GUESTPIXELBUFFERSENDER_H
#define GUESTPIXELBUFFERSENDER_H

#include "../guestpixelbuffer.h"
#include <unistd.h>

class GuestPixelBufferSender {
public:
	GuestPixelBufferSender(int socketFd) : m_socketFd(socketFd) {};

	bool redrawCommand(GuestPixelBufferRedrawCommand gpbrc)
	{
		if (this->m_socketFd < 0)
			return false;

		const unsigned long long headerSend = write(this->m_socketFd, &gpbrc.header, sizeof(gpbrc.header));
		if (headerSend != sizeof(gpbrc.header))
			return false;

		const unsigned long long pixelSend = write(this->m_socketFd, gpbrc.pixelBuffer, gbprc.header.bufferSize);
		if (pixelSend != gpbrc.header.bufferSize)
			return false;

		return true;
	};

private:
	int m_socketFd = -1;
};

#endif