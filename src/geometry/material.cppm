export module geometry:material;

import math;

export enum MaterialType {
    lambertian,
    conductor,
    dielectric,
};

export struct Material {
    MaterialType type{lambertian};
    Color albedo{1.0, 1.0, 1.0};
    double fuzz{0.0};
    double ior{1.0};
};
