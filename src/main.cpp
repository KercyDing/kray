#define SDL_MAIN_HANDLED

#include <cstdint>
#include <exception>
#include <print>
#include <vector>

#include <SDL3/SDL.h>

import config;
import engine;

using namespace config;

static int run() {
    const sdl3::Context context{SDL_INIT_VIDEO};

    const sdl3::Window window{
        "kray",
        window_width,
        window_height,
        0,
    };

    const sdl3::Renderer renderer{window.get(), nullptr};

    const sdl3::Texture texture{
        renderer.get(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height,
    };

    Raytracer raytracer;

    int remaining = samples_per_pixel;
    bool running = true;
    bool done = false;

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
            std::println("Remaining: {}", remaining);

            raytracer.render_pass();

            const auto &pixels = raytracer.pixels();

            if (auto result =
                    texture.update(nullptr, pixels.data(), window_width * sizeof(std::uint32_t));
                !result) {
                std::println(stderr, "Texture update failed: {}", result.error());
                return 1;
            }

            --remaining;
        } else if (!done) {
            done = true;
            std::println("Done.");
        }

        if (auto result = renderer.present(texture.get(), nullptr, nullptr); !result) {
            std::println(stderr, "Present failed: {}", result.error());
            return 1;
        }
    }

    return 0;
}

int main() {
    try {
        run();
    } catch (const std::exception &error) {
        std::println(stderr, "Fatal: {}", error.what());
        return 1;
    }
}
