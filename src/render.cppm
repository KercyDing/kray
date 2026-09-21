module;

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <span>

export module render;

export import math;
export import geometry;

export int to_byte(double x) {
    x = std::clamp(x, 0.0, 1.0);

    return static_cast<int>(255.999 * x);
}

export [[nodiscard]]
double linear_to_gamma(double linear) {
    return linear > 0.0 ? std::sqrt(linear) : 0.0;
}

export [[nodiscard]]
Color ray_color(const Ray &ray, std::span<const Sphere> world, int depth, Rng &rng) {
    if (depth <= 0) {
        return Color{0.0, 0.0, 0.0};
    }

    if (const auto record = hit(world, ray, 0.001, std::numeric_limits<double>::infinity())) {
        const Vec3 scatter_direction = random_on_hemisphere(record->normal, rng);

        const Ray scattered{
            record->point,
            scatter_direction,
        };

        return 0.5 * ray_color(scattered, world, depth - 1, rng);
    }

    const Vec3 direction = unit_vector(ray.direction());

    const double t = 0.5 * (direction.y() + 1.0);

    return lerp(Color{1.0, 1.0, 1.0}, Color{0.5, 0.7, 1.0}, t);
}

export uint32_t pack_color(const Color &color) {
    const uint8_t r = to_byte(linear_to_gamma(color.x()));
    const uint8_t g = to_byte(linear_to_gamma(color.y()));
    const uint8_t b = to_byte(linear_to_gamma(color.z()));

    return (r << 24) | (g << 16) | (b << 8) | 255;
}
