#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <stddef.h>

class Transport {
public:
    virtual unsigned long write(const void* buffer, size_t numBytes) = 0;
    virtual unsigned long read(void *buf, size_t numBytes) = 0;
};

#endif
