#include <iostream>
#include <map>
#include <SDL.h>
#include "pixelreceiver.h"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    PixelReceiver windowPainter;
    GuestPixelBufferReceiver receiver(0, &windowPainter);

    windowPainter.run();

    SDL_Quit();
}
