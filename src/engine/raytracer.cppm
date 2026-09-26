module;

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <ranges>
#include <span>
#include <vector>

export module engine:raytracer;

import config;
import math;
import geometry;

import :scatter;

using namespace config;

export class Camera {
public:
    Camera();

    [[nodiscard]] Ray ray(int x, int y, double offset_u, double offset_v) const;

private:
    Point3 pixel00_{};
    Vec3 pixel_delta_u_{};
    Vec3 pixel_delta_v_{};
};

export class Raytracer {
public:
    Raytracer();

    void render_pass();

    [[nodiscard]] const std::vector<std::uint32_t> &pixels() const;

private:
    std::array<Sphere, 4> world_{
        // Ground
        Sphere{
            .center = Point3{0.0, -100.5, -1.0},
            .radius = 100.0,
            .material =
                Material{
                    .type = lambertian,
                    .albedo = Color{0.8, 0.8, 0.1},
                    .fuzz = 0.0,
                },
        },
        // Red - back
        Sphere{
            .center = Point3{0.0, -0.1, -1.6},
            .radius = 0.4,
            .material =
                Material{
                    .type = lambertian,
                    .albedo = Color{0.9, 0.6, 0.6},
                    .fuzz = 0.1,
                },
        },

        // Green - front left
        Sphere{
            .center = Point3{-0.55, -0.1, -1.0},
            .radius = 0.4,
            .material =
                Material{
                    .type = conductor,
                    .albedo = Color{0.6, 0.9, 0.6},
                    .fuzz = 0.1,
                },
        },

        // Blue - front right
        Sphere{
            .center = Point3{0.55, -0.1, -1.0},
            .radius = 0.4,
            .material =
                Material{
                    .type = dielectric,
                    .albedo = Color{0.6, 0.6, 5.0},
                    .ior = 1.05,
                },
        },
    };

    Camera camera_{};

    Rng rng_{42};

    std::vector<Color> accumulation_{};
    std::vector<std::uint32_t> pixels_{};

    int samples_done_{0};
};

namespace {

    constexpr std::size_t pixel_count = window_width * window_height;

    [[nodiscard]]
    std::size_t index(const int x, const int y) {
        return static_cast<std::size_t>(y) * window_width + x;
    }

    [[nodiscard]]
    int to_byte(double x) {
        x = std::clamp(x, 0.0, 1.0);

        return static_cast<int>(255.999 * x);
    }

    [[nodiscard]]
    double linear_to_gamma(const double linear) {
        return linear > 0.0 ? std::sqrt(linear) : 0.0;
    }

    [[nodiscard]]
    std::uint32_t pack_color(const Color &color) {
        const auto r = static_cast<std::uint32_t>(to_byte(linear_to_gamma(color.x())));
        const auto g = static_cast<std::uint32_t>(to_byte(linear_to_gamma(color.y())));
        const auto b = static_cast<std::uint32_t>(to_byte(linear_to_gamma(color.z())));

        return (r << 24) | (g << 16) | (b << 8) | 0xFFu;
    }

    [[nodiscard]]
    Color ray_color(const Ray &ray, const std::span<const Sphere> world, const int depth,
                    Rng &rng) {
        if (depth <= 0) {
            return Color{0.0, 0.0, 0.0};
        }

        if (const auto record = hit(world, ray, 0.001, infinity)) {
            const auto result = scatter(ray, *record, rng);

            if (!result) {
                return Color{0.0, 0.0, 0.0};
            }

            return result->attenuation * ray_color(result->ray, world, depth - 1, rng);
        }

        const Vec3 direction = unit_vector(ray.direction());

        const double t = 0.5 * (direction.y() + 1.0);

        return lerp(Color{1.0, 1.0, 1.0}, Color{0.5, 0.7, 1.0}, t);
    }

} // namespace

Camera::Camera() {
    constexpr Vec3 viewport_u{viewport_width, 0.0, 0.0};

    constexpr Vec3 viewport_v{0.0, -viewport_height, 0.0};

    pixel_delta_u_ = viewport_u / window_width;

    pixel_delta_v_ = viewport_v / window_height;

    constexpr Point3 viewport_upper_left =
        camera_center - Vec3{0.0, 0.0, focal_length} - viewport_u / 2.0 - viewport_v / 2.0;

    pixel00_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
}

Ray Camera::ray(const int x, const int y, const double offset_u, const double offset_v) const {
    const Point3 pixel_center = pixel00_ + x * pixel_delta_u_ + y * pixel_delta_v_;

    const Point3 pixel_sample =
        pixel_center + offset_u * pixel_delta_u_ + offset_v * pixel_delta_v_;

    return Ray{camera_center, pixel_sample - camera_center};
}

Raytracer::Raytracer() : accumulation_(pixel_count), pixels_(pixel_count) {}

void Raytracer::render_pass() {
    for (int y = 0; y < window_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            const double offset_u = rng_.uniform() - 0.5;
            const double offset_v = rng_.uniform() - 0.5;

            const Ray ray = camera_.ray(x, y, offset_u, offset_v);

            accumulation_[index(x, y)] += ray_color(ray, world_, max_depth, rng_);
        }
    }

    ++samples_done_;

    const double scale = 1.0 / samples_done_;

    for (auto &&[pixel, color] : std::views::zip(pixels_, accumulation_)) {
        pixel = pack_color(scale * color);
    }
}

const std::vector<std::uint32_t> &Raytracer::pixels() const {
    return pixels_;
}
