#ifndef RDPSERVER_H
#define RDPSERVER_H

#include <freerdp/listener.h>
#include <thread>
#include <unistd.h>

// Takes ownership of all provided data
class RdpThreadMemory {
public:
    RdpThreadMemory(freerdp_listener* listener, int socket) :
        m_listener(listener), m_socket(socket) {}
    ~RdpThreadMemory() {
        if (this->m_listener) {
            delete this->m_listener;
            this->m_listener = nullptr;
        }
        if (this->m_socket >= 0) {
            close(this->m_socket);
        }
    }
    freerdp_listener* listener() { return this->m_listener; }
    int socket() { return this->m_socket; }
private:
    freerdp_listener* m_listener = nullptr;
    int m_socket = -1;
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
