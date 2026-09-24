module;

#include <array>
#include <cstdint>
#include <vector>

export module engine:renderer;

import config;
import math;
import geometry;

export class Camera {
public:
    Camera();

    [[nodiscard]] Ray ray(int x, int y, double offset_u, double offset_v) const;

private:
    Point3 pixel00_{};
    Vec3 pixel_delta_u_{};
    Vec3 pixel_delta_v_{};
};

export class Renderer {
public:
    Renderer();

    void render_pass();

    [[nodiscard]] const std::vector<std::uint32_t> &pixels() const;

private:
    std::array<Sphere, 2> world_{
        Sphere{
            .center = Point3{0.0, 0.0, -1.0},
            .radius = 0.5,
            .albedo = Color{0.9, 0.6, 0.6},
        },
        Sphere{
            .center = Point3{0.0, -100.5, -1.0},
            .radius = 100.0,
            .albedo = Color{0.8, 0.8, 0.3},
        },
    };

    Camera camera_{};

    Rng rng_{42};

    std::vector<Color> accumulation_{};
    std::vector<std::uint32_t> pixels_{};

    int samples_done_{0};
};
