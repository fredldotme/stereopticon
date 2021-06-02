#ifndef SHAREDMEMTRANSPORT_H
#define SHAREDMEMTRANSPORT_H

#include "../transport.h"

class SharedMemTransport : public Transport {
public:
    SharedMemTransport(const char* path) {

    }

    unsigned long write(const void* buffer, size_t numBytes)
    {

    }

    unsigned long read(void *buf, size_t numBytes)
    {

    }

private:

};

#endif
