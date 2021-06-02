#include <iostream>
#include <map>
#include <SDL.h>
#include "windowpainter.h"
#include "../common/transports/sharedmemtransport.hpp"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    WindowPainter windowPainter;
    SharedMemTransport transport;
    GuestPixelBufferReceiver receiver(&transport, &windowPainter);

    windowPainter.run();

    SDL_Quit();
}
