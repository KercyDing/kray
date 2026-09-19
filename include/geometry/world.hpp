#pragma once

#include <optional>
#include <span>

#include "geometry/sphere.hpp"

[[nodiscard]]
inline std::optional<HitRecord> hit(std::span<const Sphere> world, const Ray &ray, double t_min,
                                    double t_max) {
    std::optional<HitRecord> closest_hit;

    double closest_t = t_max;

    for (const Sphere &sphere : world) {
        if (const auto record = hit(sphere, ray, t_min, closest_t)) {
            closest_t = record->t;
            closest_hit = record;
        }
    }

    return closest_hit;
}
