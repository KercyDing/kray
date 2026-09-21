#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <limits>
#include <ostream>
#include <print>
#include <span>

import config;
import geometry;
import math;

int to_byte(double x) {
    x = std::clamp(x, 0.0, 1.0);

    return static_cast<int>(255.999 * x);
}

[[nodiscard]]
double linear_to_gamma(double linear) {
    return linear > 0.0 ? std::sqrt(linear) : 0.0;
}

void write_color(std::ostream &out, const Color &color) {
    const double r = linear_to_gamma(color.x());
    const double g = linear_to_gamma(color.y());
    const double b = linear_to_gamma(color.z());

    std::println(out, "{} {} {}", to_byte(r), to_byte(g), to_byte(b));
}

[[nodiscard]]
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

int main() {
    const Vec3 viewport_u{
        config::viewport_width,
        0.0,
        0.0,
    };

    const Vec3 viewport_v{
        0.0,
        -config::viewport_height,
        0.0,
    };

    const Vec3 pixel_delta_u = viewport_u / config::width;
    const Vec3 pixel_delta_v = viewport_v / config::height;

    const Point3 viewport_upper_left = config::camera_center - Vec3{0.0, 0.0, config::focal_length}
                                       - viewport_u / 2.0 - viewport_v / 2.0;

    const Point3 pixel00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::ofstream out{"image.ppm"};

    if (!out) {
        std::println(stderr, "failed to open image.ppm");
        return 1;
    }

    std::print(out, "P3\n{} {}\n255\n", config::width, config::height);

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

    for (int y = 0; y < config::height; ++y) {
        std::println(stderr, "scanlines remaining: {}", config::height - y);

        for (int x = 0; x < config::width; ++x) {
            Color pixel_color{};

            const Point3 pixel_center = pixel00 + x * pixel_delta_u + y * pixel_delta_v;

            for (int sample = 0; sample < config::samples_per_pixel; ++sample) {
                const double offset_u = rng.uniform() - 0.5;

                const double offset_v = rng.uniform() - 0.5;

                const Point3 pixel_sample =
                    pixel_center + offset_u * pixel_delta_u + offset_v * pixel_delta_v;

                const Vec3 ray_direction = pixel_sample - config::camera_center;

                const Ray ray{
                    config::camera_center,
                    ray_direction,
                };

                pixel_color += ray_color(ray, world, config::max_depth, rng);
            }

            pixel_color /= config::samples_per_pixel;

            write_color(out, pixel_color);
        }
    }

    std::println(stderr, "done");

    return 0;
}
