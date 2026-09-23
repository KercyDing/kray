module;

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <span>
#include <vector>

module renderer;

import config;
import math;
import geometry;

namespace {

    constexpr std::size_t pixel_count = config::window_width * config::window_height;

    [[nodiscard]]
    std::size_t index(int x, int y) {
        return static_cast<std::size_t>(y) * config::window_width + x;
    }

    [[nodiscard]]
    int to_byte(double x) {
        x = std::clamp(x, 0.0, 1.0);

        return static_cast<int>(255.999 * x);
    }

    [[nodiscard]]
    double linear_to_gamma(double linear) {
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
    Color ray_color(const Ray &ray, std::span<const Sphere> world, int depth, Rng &rng) {
        if (depth <= 0) {
            return Color{0.0, 0.0, 0.0};
        }

        if (const auto record = hit(world, ray, 0.001, std::numeric_limits<double>::infinity())) {
            Vec3 scatter_direction = record->normal + random_unit_vector(rng);

            const Ray scattered{
                record->point,
                scatter_direction,
            };

            return record->albedo * ray_color(scattered, world, depth - 1, rng);
        }

        const Vec3 direction = unit_vector(ray.direction());

        const double t = 0.5 * (direction.y() + 1.0);

        return lerp(Color{1.0, 1.0, 1.0}, Color{0.5, 0.7, 1.0}, t);
    }

} // namespace

Camera::Camera() {
    constexpr Vec3 viewport_u{config::viewport_width, 0.0, 0.0};

    constexpr Vec3 viewport_v{0.0, -config::viewport_height, 0.0};

    pixel_delta_u_ = viewport_u / config::window_width;

    pixel_delta_v_ = viewport_v / config::window_height;

    constexpr Point3 viewport_upper_left = config::camera_center
                                           - Vec3{0.0, 0.0, config::focal_length} - viewport_u / 2.0
                                           - viewport_v / 2.0;

    pixel00_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
}

Ray Camera::ray(const int x, const int y, const double offset_u, const double offset_v) const {
    const Point3 pixel_center = pixel00_ + x * pixel_delta_u_ + y * pixel_delta_v_;

    const Point3 pixel_sample =
        pixel_center + offset_u * pixel_delta_u_ + offset_v * pixel_delta_v_;

    return Ray{config::camera_center, pixel_sample - config::camera_center};
}

Renderer::Renderer() : accumulation_(pixel_count), pixels_(pixel_count) {}

void Renderer::render_pass() {
    for (int y = 0; y < config::window_height; ++y) {
        for (int x = 0; x < config::window_width; ++x) {
            const double offset_u = rng_.uniform() - 0.5;
            const double offset_v = rng_.uniform() - 0.5;

            const Ray ray = camera_.ray(x, y, offset_u, offset_v);

            accumulation_[index(x, y)] += ray_color(ray, world_, config::max_depth, rng_);
        }
    }

    ++samples_done_;

    const double scale = 1.0 / samples_done_;

    for (std::size_t i = 0; i < pixels_.size(); ++i) {
        pixels_[i] = pack_color(scale * accumulation_[i]);
    }
}

const std::vector<std::uint32_t> &Renderer::pixels() const {
    return pixels_;
}
