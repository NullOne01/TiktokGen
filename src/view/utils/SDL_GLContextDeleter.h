#ifndef TIKTOKGEN_SDL_GLCONTEXTDELETER_H
#define TIKTOKGEN_SDL_GLCONTEXTDELETER_H

#include "SDL_video.h"

struct SDL_GLContextDeleter {
    inline void operator()(SDL_GLContext context) {
        SDL_GL_DeleteContext(context);
    }
};

#endif //TIKTOKGEN_SDL_GLCONTEXTDELETER_H
