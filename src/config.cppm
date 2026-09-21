export module config;

export import math;

export namespace config {
    // ppm size
    inline constexpr int window_width = 1080;
    inline constexpr int window_height = 720;

    // viewport size
    inline constexpr double viewport_height = 2.0;
    inline constexpr double viewport_width = viewport_height * window_width / window_height;

    // camera specifications
    inline constexpr double focal_length = 1.0;
    inline const Point3 camera_center{0.0, 0.0, 0.0};

    inline constexpr int max_depth = 16;
    inline constexpr int samples_per_pixel = 64;
}; // namespace config
