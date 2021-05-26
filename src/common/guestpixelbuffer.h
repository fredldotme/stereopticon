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
};

struct GuestPixelBufferRedrawHeader {
	GuestPixelBufferHeader header;
	PixelFormat format;
	unsigned long width;
	unsigned long height;
	unsigned long offsetX;
	unsigned long offsetY;
	unsigned long long bufferSize;
};

struct GuestPixelBufferRedrawCommand {
	GuestPixelBufferRedrawCommand(
		unsigned long& width,
		unsigned long& height,
		unsigned long& offsetX = 0,
		unsigned long& offsetY = 0,
		unsigned long long& bufferSize = 0,
		char* pixelBuffer = nullptr)
	{
		this->header.header.command = GuestCommand::COMMAND_REDRAW;
		this->header.width = width;
		this->header.height = height;
		this->header.offsetX = offsetX;
		this->header.offsetY = offsetY;
		this->header.bufferSize = bufferSize;
		this->pixelBuffer = pixelBuffer;
	};
	GuestPixelBufferRedrawHeader header;
	char* pixelBuffer = nullptr;
};

#endif
