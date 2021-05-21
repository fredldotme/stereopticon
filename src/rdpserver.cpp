#include "rdpserver.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <linux/vm_sockets.h>

static void rdp_main_loop(RdpThreadMemory* sharedmem)
{
    int i;
    int fds;
    int max_fds;
    int rcount;
    void* rfds[32];
    fd_set rfds_set;

    memset(rfds, 0, sizeof(rfds));

    std::cout << "Starting RDP server loop on socket " << sharedmem->socket() << std::endl;
    while (true)
    {
        rcount = 0;

        if (sharedmem->listener()->GetFileDescriptor(sharedmem->listener(), rfds, &rcount) != TRUE)
        {
            break;
        }

        max_fds = 0;
        FD_ZERO(&rfds_set);

        for (i = 0; i < rcount; i++)
        {
            fds = (int)(long)(rfds[i]);

            if (fds > max_fds)
                max_fds = fds;

            FD_SET(fds, &rfds_set);
        }

        if (max_fds == 0)
            break;

        if (select(max_fds + 1, &rfds_set, NULL, NULL, NULL) == -1)
        {
            if (!((errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == EINPROGRESS) ||
                  (errno == EINTR)))
            {
                break;
            }
        }

        if (sharedmem->listener()->CheckFileDescriptor(sharedmem->listener()) != TRUE)
        {
            break;
        }
    }

    sharedmem->listener()->Close(sharedmem->listener());
}

static BOOL peer_accepted(freerdp_listener* instance, freerdp_peer* client)
{
    // TODO: Spawn per-peer thread on incoming connections
    return true;
}

RdpServer::RdpServer()
{
    int listenSocket = socket(AF_VSOCK, SOCK_STREAM, 0);
    freerdp_listener* rdpListener = freerdp_listener_new();
    this->m_rdp_shared_memory = new RdpThreadMemory(rdpListener, listenSocket);

    this->m_rdp_shared_memory->listener()->PeerAccepted = peer_accepted;

    const bool success = this->m_rdp_shared_memory->listener()->OpenFromSocket(
                this->m_rdp_shared_memory->listener(),
                this->m_rdp_shared_memory->socket());
    this->m_listenThread = new std::thread(rdp_main_loop,
                                           this->m_rdp_shared_memory);
}

RdpServer::~RdpServer()
{
    if (this->m_listenThread) {
        this->m_listenThread->join();
        delete this->m_listenThread;
        this->m_listenThread = nullptr;
    }
    if (this->m_rdp_shared_memory) {
        delete this->m_rdp_shared_memory;
        this->m_rdp_shared_memory = nullptr;
    }
}
