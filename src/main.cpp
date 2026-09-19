#include <fstream>
#include <print>
#include <ranges>

#include "math/ray.hpp"
#include "math/vec3.hpp"

namespace constants {
constexpr int width = 800;
constexpr int height = 450;
}  // namespace constants

int to_byte(double x) {
    x = std::clamp(x, 0.0, 1.0);

    return static_cast<int>(255.999 * x);
}

void write_color(std::ofstream &out, const Color &color) {
    std::println(out, "{} {} {}", to_byte(color.x()), to_byte(color.y()), to_byte(color.z()));
}

int main() {
    // ========== Draw PPM ==========
    int width = constants::width;
    int height = constants::height;

    const auto pixels =
        std::views::cartesian_product(std::views::iota(0, height), std::views::iota(0, width));

    std::ofstream out{"image.ppm"};

    if (!out) {
        std::println(stderr, "failed to open image.ppm");
        return 1;
    }

    std::print(out, "P3\n{} {}\n255\n", width, height);

    for (auto [y, x] : pixels) {
        const double u = static_cast<double>(x) / (width - 1);
        const double v = static_cast<double>(y) / (height - 1);

        const Color color{u, v, 0.25};

        write_color(out, color);
    }

    // ========== Ray Test ==========
    const Ray ray{
        Point3{1.0, 2.0, 3.0},
        Vec3{1.0, 1.0, 1.0},
    };

    const Point3 p = ray.at(2.0);

    std::println("({}, {}, {})", p.x(), p.y(), p.z());

    return 0;
}
