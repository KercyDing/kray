export module constants;

export import math;

export namespace constants {
    // ppm size
    constexpr int width = 800;
    constexpr int height = 450;

    // viewport size
    constexpr double viewport_height = 2.0;
    constexpr double viewport_width = viewport_height * width / height;

    // camera specifications
    constexpr double focal_length = 1.0;
    const Point3 camera_center{0.0, 0.0, 0.0};

    constexpr int max_depth = 16;
    constexpr int samples_per_pixel = 32;
};  // namespace constants
