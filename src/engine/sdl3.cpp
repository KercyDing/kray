module;

#include <SDL3/SDL.h>

module engine;

namespace sdl3 {

    Window::Window(const char *title, const int width, const int height,
                   const SDL_WindowFlags flags)
        : window_(SDL_CreateWindow(title, width, height, flags)) {}

    Window::~Window() {
        SDL_DestroyWindow(window_);
    }

    SDL_Window *Window::get() const {
        return window_;
    }

    Renderer::Renderer(SDL_Window *window, const char *name)
        : renderer_(SDL_CreateRenderer(window, name)) {}

    Renderer::~Renderer() {
        SDL_DestroyRenderer(renderer_);
    }

    SDL_Renderer *Renderer::get() const {
        return renderer_;
    }

    Texture::Texture(SDL_Renderer *renderer, const SDL_PixelFormat format,
                     const SDL_TextureAccess access, const int w, const int h)
        : texture_(SDL_CreateTexture(renderer, format, access, w, h)) {}

    Texture::~Texture() {
        SDL_DestroyTexture(texture_);
    }

    SDL_Texture *Texture::get() const {
        return texture_;
    }

} // namespace sdl3
