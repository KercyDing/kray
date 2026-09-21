#define SDL_MAIN_HANDLED

#include <SDL3/SDL.h>

#include <array>
#include <print>
#include <vector>

import config;
import math;
import geometry;
import render;

using namespace config;

int main() {
    std::vector<uint32_t> pixels(window_width * window_height);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("kray", window_width, window_height, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

    SDL_Texture *texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height);

    const Vec3 viewport_u{
        viewport_width,
        0.0,
        0.0,
    };

    const Vec3 viewport_v{
        0.0,
        -viewport_height,
        0.0,
    };

    const Vec3 pixel_delta_u = viewport_u / window_width;
    const Vec3 pixel_delta_v = viewport_v / window_height;

    const Point3 viewport_upper_left =
        camera_center - Vec3{0.0, 0.0, focal_length} - viewport_u / 2.0 - viewport_v / 2.0;

    const Point3 pixel00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    const std::array world{
        Sphere{
            .center = Point3{0.0, 0.0, -1.0},
            .radius = 0.5,
        },
        Sphere{
            .center = Point3{0.0, -100.5, -1.0},
            .radius = 100.0,
        },
    };

    Rng rng{42};

    for (int y = 0; y < window_height; ++y) {
        std::println(stderr, "scanlines remaining: {}", window_height - y);

        for (int x = 0; x < window_width; ++x) {
            Color pixel_color{};

            const Point3 pixel_center = pixel00 + x * pixel_delta_u + y * pixel_delta_v;

            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                const double offset_u = rng.uniform() - 0.5;

                const double offset_v = rng.uniform() - 0.5;

                const Point3 pixel_sample =
                    pixel_center + offset_u * pixel_delta_u + offset_v * pixel_delta_v;

                const Vec3 ray_direction = pixel_sample - camera_center;

                const Ray ray{
                    camera_center,
                    ray_direction,
                };

                pixel_color += ray_color(ray, world, max_depth, rng);
            }

            pixel_color /= samples_per_pixel;

            pixels[y * window_width + x] = pack_color(pixel_color);
        }
    }

    SDL_UpdateTexture(texture, nullptr, pixels.data(), window_width * sizeof(uint32_t));

    std::println(stderr, "done");

    bool running = true;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
