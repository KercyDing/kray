#define SDL_MAIN_HANDLED

#include <charconv>
#include <chrono>
#include <exception>
#include <print>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <SDL3/SDL.h>

import config;
import engine;

using namespace config;

namespace {
    int parse_samples(const int argc, char **argv) {
        int samples = default_samples_per_pixel;

        for (int i = 1; i < argc; ++i) {
            if (const std::string_view arg{argv[i]}; arg == "--samples") {
                if (++i >= argc) {
                    throw std::runtime_error{"--samples requires a value"};
                }

                const std::string_view value{argv[i]};

                int parsed{};

                const auto [ptr, ec] =
                    std::from_chars(value.data(), value.data() + value.size(), parsed);

                if (ec != std::errc{} || ptr != value.data() + value.size() || parsed <= 0) {
                    throw std::runtime_error{"invalid --samples value"};
                }

                samples = parsed;
            } else {
                throw std::runtime_error{"unknown argument"};
            }
        }

        return samples;
    }

    int run(const int samples) {
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

        int remaining = samples;
        bool running = true;
        bool done = false;

        const auto start = std::chrono::steady_clock::now();

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

                if (auto result = texture.update(
                        nullptr, pixels.data(), window_width * sizeof(std::uint32_t));
                    !result) {
                    std::println(stderr, "Texture update failed: {}", result.error());
                    return 1;
                }

                --remaining;
            } else if (!done) {
                done = true;

                const auto end = std::chrono::steady_clock::now();

                const auto elapsed =
                    std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                std::println("Done.\nTime: {}", elapsed);
            }

            if (auto result = renderer.present(texture.get(), nullptr, nullptr); !result) {
                std::println(stderr, "Present failed: {}", result.error());
                return 1;
            }
        }

        return 0;
    }

} // namespace

int main(const int argc, char **argv) {
    try {
        return run(parse_samples(argc, argv));
    } catch (const std::exception &error) {
        std::println(stderr, "Fatal: {}", error.what());
        return 1;
    }
}
