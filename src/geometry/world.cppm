module;

#include <optional>
#include <span>

export module geometry.world;

export import math;
export import geometry.hit_record;
export import geometry.sphere;

export [[nodiscard]]
std::optional<HitRecord> hit(std::span<const Sphere> world, const Ray &ray, double t_min,
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
