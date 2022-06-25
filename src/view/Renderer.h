#ifndef TIKTOKGEN_RENDERER_H
#define TIKTOKGEN_RENDERER_H

#include "SDL_video.h"
#include "utils/SDLWindowDeleter.h"
#include "imgui.h"
#include "utils/SDL_GLContextDeleter.h"
#include <memory>

class Renderer {
public:
    Renderer();

    [[nodiscard]] bool isInLoop() const;

    void preFrame();

    void postFrame();

    virtual ~Renderer();

private:
    const ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool is_in_loop_ = true;

    std::unique_ptr<SDL_Window, SDLWindowDeleter> window_;

    std::unique_ptr<void, SDL_GLContextDeleter> gl_context_;
};


#endif //TIKTOKGEN_RENDERER_H
