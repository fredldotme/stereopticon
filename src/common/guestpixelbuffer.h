#ifndef GUESTPIXELBUFFER_H
#define GUESTPIXELBUFFER_H

// Wayland window ID
typedef unsigned long long wayland_window_id_t;

// Pixel formats as supported by the guest compositor and the host display
enum PixelFormat {
    FORMAT_UNKNOWN = 0,
    FORMAT_RGBA8888,
    FORMAT_ARGB8888
};

enum GuestCommand {
    COMMAND_UNKNOWN = 0, // To be discarded by the host display
    COMMAND_WINDOW_SPAWN,
    COMMAND_WINDOW_DESTROY,
    COMMAND_REDRAW,
};

// Header of pixel buffer data that gets sent over the wire
// from the Linux guest to the Host side window display
struct GuestPixelBufferHeader {
    unsigned short version;
    GuestCommand command;
    wayland_window_id_t windowId;
} __attribute__((packed));

// Window spawn metadata. Positioning on the guest screen
// to be mapped on the host screen.
struct GuestWindowSpawnData {
    unsigned long x;
    unsigned long y;
    unsigned long width;
    unsigned long height;
} __attribute__((packed));

// Sent by the guest compositor to the host in one chunk,
// read by the host display as two chunks (first header, then data).
struct GuestWindowSpawnCommand {
    GuestPixelBufferHeader header;
    GuestWindowSpawnData data;
} __attribute__((packed));

// Sent and read in one chunk.
// Only the common header is needed as it includes the Wayland window id.
struct GuestWindowDestroyCommand {
    GuestWindowDestroyCommand() {
        this->header.command = GuestCommand::COMMAND_WINDOW_DESTROY;
    }
    GuestPixelBufferHeader header;
} __attribute__((packed));

// Redraw metadata. Describes information about where and how
// to draw pixel buffers that are sent after the RedrawHeader.
struct GuestPixelBufferRedrawData {
    PixelFormat format;
    unsigned long width;
    unsigned long height;
    unsigned long offsetX;
    unsigned long offsetY;
    unsigned long long bufferSize;
} __attribute__((packed));

// Combination of both the main header and the redraw metadata.
// Sent by the guest compositor to the host side in one chunk
// and read on the host side in two separate steps,
// hence the combination of two structs.
struct GuestPixelBufferRedrawHeader {
    GuestPixelBufferHeader header;
    GuestPixelBufferRedrawData data;
} __attribute__((packed));

struct GuestPixelBufferRedrawCommand {
    GuestPixelBufferRedrawCommand() {
        this->header.header.command = GuestCommand::COMMAND_REDRAW;
    }
    GuestPixelBufferRedrawCommand(
        unsigned long& width,
        unsigned long& height,
        unsigned long& offsetX,
        unsigned long& offsetY,
        unsigned long long& bufferSize,
        char* pixelBuffer)
    {
        this->header.header.command = GuestCommand::COMMAND_REDRAW;
        this->header.data.width = width;
        this->header.data.height = height;
        this->header.data.offsetX = offsetX;
        this->header.data.offsetY = offsetY;
        this->header.data.bufferSize = bufferSize;
        this->pixelBuffer = pixelBuffer;
    };
    GuestPixelBufferRedrawHeader header;
    char* pixelBuffer = nullptr;
};

#endif
