#define SDL_MAIN_HANDLED

#include <SDL3/SDL.h>

#include <cstdint>
#include <vector>

import config;
import renderer;

using namespace config;

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("kray", window_width, window_height, 0);

    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(window, nullptr);

    SDL_Texture *texture = SDL_CreateTexture(sdl_renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height);

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
                texture, nullptr, pixels.data(), window_width * sizeof(std::uint32_t));

            --remaining;
        }

        SDL_RenderClear(sdl_renderer);

        SDL_RenderTexture(sdl_renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(sdl_renderer);
    }

    return 0;
}
