#ifndef TIKTOKGEN_SDLWINDOWDELETER_H
#define TIKTOKGEN_SDLWINDOWDELETER_H

#include "SDL_video.h"

struct SDLWindowDeleter {
    inline void operator()(SDL_Window *window) {
        SDL_DestroyWindow(window);
    }
};

#endif //TIKTOKGEN_SDLWINDOWDELETER_H
