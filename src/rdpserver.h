#ifndef RDPSERVER_H
#define RDPSERVER_H

#include <freerdp/listener.h>
#include <thread>

class RdpThreadMemory {
public:
    RdpThreadMemory(freerdp_listener* listener) :
        m_listener(listener) {}
    ~RdpThreadMemory() {
        if (!this->m_listener)
            return;
        delete this->m_listener;
        this->m_listener = nullptr;
    }
    freerdp_listener* listener() { return this->m_listener; }
private:
    freerdp_listener* m_listener = nullptr;
};

class RdpServer {
public:
    RdpServer();
    ~RdpServer();
private:
    RdpThreadMemory* m_rdp_shared_memory = nullptr;
    std::thread* m_listenThread = nullptr;
};

#endif
