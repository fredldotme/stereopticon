#ifndef GUESTPIXELBUFFERRECEIVER_H
#define GUESTPIXELBUFFERRECEIVER_H

#include <unistd.h>
#include <thread>
#include "../guestpixelbuffer.h"

class PixelBufferHandler {
public:
    virtual void spawnWindow(GuestWindowSpawnCommand& spawnData) = 0;
    virtual void destroyWindow(GuestWindowDestroyCommand& command) = 0;
    virtual void receiveRedraw(GuestPixelBufferRedrawCommand& command) = 0;
    virtual bool windowExists(wayland_window_id_t& windowId) = 0;
};

struct ReadSocketThreadMemory {
    bool read = true;
    bool done = false;
    int socketFd = -1;
    PixelBufferHandler* handler = nullptr;
};

static void socketReadLoop(ReadSocketThreadMemory* memory)
{
    if (memory->socketFd < 0)
        goto done;

    if (!memory->handler)
        goto done;

    while (memory->read) {
        // Header should be sent first
        GuestPixelBufferHeader header;
        ssize_t receivedLen = read(memory->socketFd, (void*)&header, sizeof(header));

        // Not matching data? Try again
        if (receivedLen != sizeof(header))
            continue;

        size_t remainingLen = 0;

        if (header.command == GuestCommand::COMMAND_WINDOW_SPAWN) {
            GuestWindowSpawnData spawnData;
            remainingLen = sizeof(spawnData);
            receivedLen = read(memory->socketFd, (void*)&spawnData, remainingLen);

            if (receivedLen != remainingLen)
                continue;

            GuestWindowSpawnCommand command;
            command.header = header;
            command.data = spawnData;
            memory->handler->spawnWindow(command);
        } else if (header.command == GuestCommand::COMMAND_WINDOW_DESTROY) {
            // We got all the data we need, cause the handler to destroy the window immediately
            GuestWindowDestroyCommand command;
            command.header = header;
            memory->handler->destroyWindow(command);
        } else if (header.command == GuestCommand::COMMAND_REDRAW) {
            // Ignore windows that haven't been spawned yet
            const bool windowExists = memory->handler->windowExists(header.windowId);
            if (!windowExists)
                continue;

            GuestPixelBufferRedrawData redrawData;
            remainingLen = sizeof(redrawData);
            receivedLen = read(memory->socketFd, (void*)&redrawData, remainingLen);

            // Received data has to match size
            if (receivedLen != remainingLen)
                continue;

            // Avoid zero-pixel messages
            if (redrawData.bufferSize <= 0)
                continue;

            char* pixels = new char[redrawData.bufferSize];
            receivedLen = read(memory->socketFd, (void*)pixels, redrawData.bufferSize);

            // If received length doesn't match buffer size in header, ignore.
            if (receivedLen != redrawData.bufferSize) {
                delete[] pixels;
                continue;
            }

            // Reconstruct values with received data
            GuestPixelBufferRedrawCommand command;
            GuestPixelBufferRedrawHeader redrawHeader;
            redrawHeader.header = header;
            redrawHeader.data = redrawData;
            command.header = redrawHeader;
            command.pixelBuffer = pixels;

            // Pass on redraw to handler
            memory->handler->receiveRedraw(command);

            // Done with this frame
            delete[] pixels;
        }
    }

done:
    memory->done = true;
}

class GuestPixelBufferReceiver {
public:
    GuestPixelBufferReceiver(int socketFd, PixelBufferHandler* handler)
    {
        this->m_readLoopMemory.handler = handler;
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
