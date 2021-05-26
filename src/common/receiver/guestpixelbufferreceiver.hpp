#ifndef GUESTPIXELBUFFERRECEIVER_H
#define GUESTPIXELBUFFERRECEIVER_H

#include <unistd.h>
#include <thread>
#include "../guestpixelbuffer.h"

struct ReadSocketThreadMemory {
	bool read = true;
	bool done = false;
	int socketFd = -1;
};

void socketReadLoop(ReadSocketThreadMemory* memory)
{
	while (memory->read) {
		
	}
	memory->done = true;
}

class GuestPixelBufferReceiver {
public:
	GuestPixelBufferReceiver(int socketFd)
	{
		this->m_readLoopMemory.socketFd = socketFd;
		this->m_readThread = new std::thread(socketReadLoop, &m_readLoopMemory);
	}
	~GuestPixelBufferReceiver()
	{
		this->m_readLoopMemory.read = false;
		if (this->m_readThread) {
			while(!this->m_readLoopMemory.done) {
				usleep(1000);
			}
			delete this->m_readThread;
			this->m_readThread = nullptr;
		}
	}

private:
	std::thread* m_readThread = nullptr;
	ReadSocketThreadMemory m_readLoopMemory;
};

#endif