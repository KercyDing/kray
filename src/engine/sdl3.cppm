module;

#include <expected>
#include <stdexcept>
#include <string>

#include <SDL3/SDL.h>

export module engine:sdl3;

namespace {

    // Throws the SDL error. The bool form is for APIs where false means failure, unlike
    // SDL_PollEvent.
    void require(const bool ok) {
        if (!ok) {
            throw std::runtime_error(SDL_GetError());
        }
    }

    auto *require(auto *handle) {
        if (handle == nullptr) {
            throw std::runtime_error(SDL_GetError());
        }

        return handle;
    }

} // namespace

export namespace sdl3 {

    // Context
    class Context {
    public:
        explicit Context(SDL_InitFlags flags);

        ~Context();

        Context(const Context &) = delete;
        Context &operator=(const Context &) = delete;
    };

    // Window
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

    // Renderer
    class Renderer {
    public:
        Renderer(SDL_Window *window, const char *name);

        ~Renderer();

        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        [[nodiscard]] SDL_Renderer *get() const;
        [[nodiscard]] std::expected<void, std::string> present(SDL_Texture *texture,
                                                               const SDL_FRect *srcrect,
                                                               const SDL_FRect *dstrect) const;

    private:
        SDL_Renderer *renderer_{};
    };

    // Texture
    class Texture {
    public:
        Texture(SDL_Renderer *renderer, SDL_PixelFormat format, SDL_TextureAccess access, int w,
                int h);

        ~Texture();

        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;

        [[nodiscard]] SDL_Texture *get() const;

        [[nodiscard]] std::expected<void, std::string> update(const SDL_Rect *rect,
                                                              const void *pixels, int pitch) const;

    private:
        SDL_Texture *texture_{};
    };

    // ========== Impl ==========
    // Context
    Context::Context(const SDL_InitFlags flags) {
        require(SDL_Init(flags));
    }

    Context::~Context() {
        SDL_Quit();
    }

    // Window
    Window::Window(const char *title, const int width, const int height,
                   const SDL_WindowFlags flags)
        : window_(require(SDL_CreateWindow(title, width, height, flags))) {}

    Window::~Window() {
        SDL_DestroyWindow(window_);
    }

    SDL_Window *Window::get() const {
        return window_;
    }

    // Renderer
    Renderer::Renderer(SDL_Window *window, const char *name)
        : renderer_(require(SDL_CreateRenderer(window, name))) {}

    Renderer::~Renderer() {
        SDL_DestroyRenderer(renderer_);
    }

    SDL_Renderer *Renderer::get() const {
        return renderer_;
    }

    std::expected<void, std::string> Renderer::present(SDL_Texture *texture,
                                                       const SDL_FRect *srcrect,
                                                       const SDL_FRect *dstrect) const {
        if (!SDL_RenderClear(renderer_)) {
            return std::unexpected(SDL_GetError());
        }

        if (!SDL_RenderTexture(renderer_, texture, srcrect, dstrect)) {
            return std::unexpected(SDL_GetError());
        }

        if (!SDL_RenderPresent(renderer_)) {
            return std::unexpected(SDL_GetError());
        }

        return {};
    }

    // Texture
    Texture::Texture(SDL_Renderer *renderer, const SDL_PixelFormat format,
                     const SDL_TextureAccess access, const int w, const int h)
        : texture_(require(SDL_CreateTexture(renderer, format, access, w, h))) {}

    Texture::~Texture() {
        SDL_DestroyTexture(texture_);
    }

    SDL_Texture *Texture::get() const {
        return texture_;
    }

    std::expected<void, std::string> Texture::update(const SDL_Rect *rect, const void *pixels,
                                                     const int pitch) const {
        if (!SDL_UpdateTexture(texture_, rect, pixels, pitch)) {
            return std::unexpected(SDL_GetError());
        }

        return {};
    }

} // namespace sdl3
