export module config;

export import math;

export namespace config {
    // ppm size
    constexpr int window_width = 1080;
    constexpr int window_height = 720;

    // viewport size
    constexpr double viewport_height = 2.0;
    constexpr double viewport_width = viewport_height * window_width / window_height;

    // camera specifications
    constexpr double focal_length = 1.0;
    constexpr Point3 camera_center{0.0, 0.0, 0.0};

    constexpr int max_depth = 16;
    constexpr int samples_per_pixel = 64;
}; // namespace config
