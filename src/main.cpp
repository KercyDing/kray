#define SDL_MAIN_HANDLED

#include <cstdint>
#include <vector>

#include <SDL3/SDL.h>

import config;
import renderer;
import sdl3;

using namespace config;

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    const sdl3::Window window{"kray", window_width, window_height, 0};

    const sdl3::Renderer sdl_renderer{window.get(), nullptr};

    const sdl3::Texture texture{sdl_renderer.get(),
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                window_width,
                                window_height};

    Renderer renderer;

    int remaining = samples_per_pixel;

    bool running = true;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        if (!running) {
            break;
        }

        if (remaining > 0) {
            renderer.render_pass();

            const std::vector<std::uint32_t> &pixels = renderer.pixels();

            SDL_UpdateTexture(
                texture.get(), nullptr, pixels.data(), window_width * sizeof(std::uint32_t));

            --remaining;
        }

        SDL_RenderClear(sdl_renderer.get());

        SDL_RenderTexture(sdl_renderer.get(), texture.get(), nullptr, nullptr);

        SDL_RenderPresent(sdl_renderer.get());
    }

    SDL_Quit();

    return 0;
}
