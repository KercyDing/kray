#include <algorithm>
#include <fstream>
#include <limits>
#include <print>
#include <span>

import geometry;
import math;

namespace constants {
// ppm size
constexpr int width = 800;
constexpr int height = 450;

// viewport size
constexpr double viewport_height = 2.0;
constexpr double viewport_width = viewport_height * width / height;

// camera specifications
constexpr double focal_length = 1.0;
const Point3 camera_center{0.0, 0.0, 0.0};
}  // namespace constants

int to_byte(double x) {
    x = std::clamp(x, 0.0, 1.0);

    return static_cast<int>(255.999 * x);
}

void write_color(std::ofstream &out, const Color &color) {
    std::println(out, "{} {} {}", to_byte(color.x()), to_byte(color.y()), to_byte(color.z()));
}

[[nodiscard]]
Color ray_color(const Ray &ray, std::span<const Sphere> world) {
    if (const auto record = hit(world, ray, 0.001, std::numeric_limits<double>::infinity())) {
        return 0.5 * (record->normal + Color{1.0, 1.0, 1.0});
    }

    const Vec3 direction = unit_vector(ray.direction());

    const double t = 0.5 * (direction.y() + 1.0);

    return lerp(Color{1.0, 1.0, 1.0}, Color{0.5, 0.7, 1.0}, t);
}

int main() {
    int width = constants::width;
    int height = constants::height;
    double viewport_width = constants::viewport_width;
    double viewport_height = constants::viewport_height;

    double focal_length = constants::focal_length;
    Point3 camera_center = constants::camera_center;

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

    const Vec3 pixel_delta_u = viewport_u / width;
    const Vec3 pixel_delta_v = viewport_v / height;

    const Point3 viewport_upper_left =
        camera_center - Vec3{0.0, 0.0, focal_length} - viewport_u / 2.0 - viewport_v / 2.0;

    const Point3 pixel00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::ofstream out{"image.ppm"};

    if (!out) {
        std::println(stderr, "failed to open image.ppm");
        return 1;
    }

    std::print(out, "P3\n{} {}\n255\n", width, height);

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

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Point3 pixel_center = pixel00 + x * pixel_delta_u + y * pixel_delta_v;

            const Vec3 ray_direction = pixel_center - camera_center;

            const Ray ray{
                camera_center,
                ray_direction,
            };

            write_color(out, ray_color(ray, world));
        }
    }

    return 0;
}
