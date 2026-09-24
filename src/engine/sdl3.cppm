module;

#include <SDL3/SDL.h>

export module engine:sdl3;

export namespace sdl3 {

    class Window {
    public:
        Window(const char *title, int width, int height, SDL_WindowFlags flags);

        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        [[nodiscard]] SDL_Window *get() const;

    private:
        SDL_Window *window_;
    };

    class Renderer {
    public:
        Renderer(SDL_Window *window, const char *name);

        ~Renderer();

        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        [[nodiscard]] SDL_Renderer *get() const;

    private:
        SDL_Renderer *renderer_{};
    };

    class Texture {
    public:
        Texture(SDL_Renderer *renderer, SDL_PixelFormat format, SDL_TextureAccess access, int w,
                int h);

        ~Texture();

        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;

        [[nodiscard]] SDL_Texture *get() const;

    private:
        SDL_Texture *texture_{};
    };

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
