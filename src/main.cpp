#include <fstream>
#include <print>
#include <ranges>

namespace constants {
constexpr int width = 800;
constexpr int height = 450;
}  // namespace constants

struct Color {
    double r;
    double g;
    double b;
};

int to_byte(double x) {
    x = std::clamp(x, 0.0, 1.0);

    return static_cast<int>(255.999 * x);
}

int main() {
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

        const Color color{
            .r = u,
            .g = v,
            .b = 0.25,
        };

        std::println(
            out, "{} {} {}", to_byte(color.r), to_byte(color.g), to_byte(color.b));
    }

    return 0;
}
