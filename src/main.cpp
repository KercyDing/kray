#include <algorithm>
#include <cmath>
#include <fstream>
#include <print>
#include <ranges>

#include "math/ray.hpp"
#include "math/vec3.hpp"

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
double hit_sphere(const Point3 &center, double radius, const Ray &ray) {
    const Vec3 oc = ray.origin() - center;

    const double a = dot(ray.direction(), ray.direction());
    const double half_b = dot(oc, ray.direction());
    const double c = dot(oc, oc) - radius * radius;

    const double discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0) {
        return -1.0;
    }

    const double sqrt_d = std::sqrt(discriminant);

    const double t1 = (-half_b - sqrt_d) / a;
    const double t2 = (-half_b + sqrt_d) / a;

    if (t1 >= 0.0) {
        return t1;
    }

    if (t2 >= 0.0) {
        return t2;
    }

    return -1.0;
}

[[nodiscard]]
Color ray_color(const Ray &ray) {
    const Point3 sphere_center{0.0, 0.0, -1.0};

    const double t = hit_sphere(sphere_center, 0.5, ray);

    if (t >= 0.0) {
        const Point3 p = ray.at(t);

        const Vec3 normal = unit_vector(p - sphere_center);

        return 0.5 * (normal + Color{1.0, 1.0, 1.0});
    }

    const Vec3 direction = unit_vector(ray.direction());

    const double a = 0.5 * (direction.y() + 1.0);

    return lerp(Color{1.0, 1.0, 1.0}, Color{0.5, 0.7, 1.0}, a);
}

int main() {
    // ========== Draw PPM ==========
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

    const auto pixels =
        std::views::cartesian_product(std::views::iota(0, height), std::views::iota(0, width));

    std::ofstream out{"image.ppm"};

    if (!out) {
        std::println(stderr, "failed to open image.ppm");
        return 1;
    }

    std::print(out, "P3\n{} {}\n255\n", width, height);

    for (auto [y, x] : pixels) {
        const Point3 pixel_center = pixel00 + x * pixel_delta_u + y * pixel_delta_v;

        const Vec3 ray_direction = pixel_center - camera_center;

        const Ray ray{
            camera_center,
            ray_direction,
        };

        write_color(out, ray_color(ray));
    }

    return 0;
}
