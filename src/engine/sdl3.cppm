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

} // namespace sdl3
