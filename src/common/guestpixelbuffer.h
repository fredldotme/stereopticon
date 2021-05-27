#ifndef GUESTPIXELBUFFER_H
#define GUESTPIXELBUFFER_H

// Pixel formats as supported by the guest compositor and the host display
enum PixelFormat {
	FORMAT_UNKNOWN = 0,
	FORMAT_RGBA8888,
	FORMAT_ARGB8888
};

enum GuestCommand {
	COMMAND_UNKNOWN = 0, // To be discarded by the host display
	COMMAND_REDRAW,
};

// Header of pixel buffer data that gets sent over the wire
// from the Linux guest to the Host side window display
struct GuestPixelBufferHeader {
	unsigned short version;
	GuestCommand command;
	unsigned long long windowId;
} __attribute((packed))__;

// Redraw metadata. Describes information about where and how
// to draw pixel buffers that are sent after the RedrawHeader.
struct GuestPixelBufferRedrawData {
	PixelFormat format;
	unsigned long width;
	unsigned long height;
	unsigned long offsetX;
	unsigned long offsetY;
	unsigned long long bufferSize;
} __attribute((packed))__;

// Combination of both the main header and the redraw metadata.
// Sent by the guest compositor to the host side in one chunk
// and read on the host side in two separate steps,
// hence the combination of two structs.
struct GuestPixelBufferRedrawHeader {
	GuestPixelBufferHeader header;
	GuestPixelBufferRedrawData data;
} __attribute((packed))__;

struct GuestPixelBufferRedrawCommand {
	GuestPixelBufferRedrawCommand() {}
	GuestPixelBufferRedrawCommand(
		unsigned long& width,
		unsigned long& height,
		unsigned long& offsetX = 0,
		unsigned long& offsetY = 0,
		unsigned long long& bufferSize = 0,
		char* pixelBuffer = nullptr)
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
