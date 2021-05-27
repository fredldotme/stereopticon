#include <iostream>
#include <map>
#include <SDL.h>
#include "pixelreceiver.h"

int main(int argc, char** argv)
{
    PixelReceiver windowPainter;
    GuestPixelBufferReceiver receiver(0, &windowPainter);
}
