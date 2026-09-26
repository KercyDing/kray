module;

#include <optional>
#include <span>

export module geometry:world;

import math;

import :hit_record;
import :sphere;

export [[nodiscard]]
std::optional<HitRecord> hit(const std::span<const Sphere> world, const Ray &ray,
                             const double t_min, const double t_max) {
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
