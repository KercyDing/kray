module;

#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>

export module engine:scatter;

import math;
import geometry;

export struct Scatter {
    Ray ray;
    Color attenuation;
};

namespace {

    [[nodiscard]]
    double reflectance(const double cosine, const double eta_ratio) {
        double r0 = (1.0 - eta_ratio) / (1.0 + eta_ratio);
        r0 *= r0;

        return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5);
    }

} // namespace

export [[nodiscard]]
std::optional<Scatter> scatter(const Ray &incoming, const HitRecord &hit, Rng &rng) {
    switch (hit.material.type) {
        case lambertian: {
            const Vec3 direction = hit.normal + random_unit_vector(rng);

            return Scatter{
                .ray = Ray{hit.point, direction},
                .attenuation = hit.material.albedo,
            };
        }

        case conductor: {
            const Vec3 direction = reflect(unit_vector(incoming.direction()), hit.normal)
                                   + hit.material.fuzz * random_unit_vector(rng);

            if (dot(direction, hit.normal) <= 0.0) {
                return std::nullopt;
            }

            return Scatter{
                .ray = Ray{hit.point, direction},
                .attenuation = hit.material.albedo,
            };
        }

        case dielectric: {
            const Vec3 unit_direction = unit_vector(incoming.direction());

            if (hit.material.ior == 1.0) {
                return Scatter{
                    .ray = Ray{hit.point, unit_direction},
                    .attenuation = Color{1.0, 1.0, 1.0},
                };
            }

            const double eta_ratio = hit.front_face ? 1.0 / hit.material.ior : hit.material.ior;

            const double cos_theta = std::min(dot(-unit_direction, hit.normal), 1.0);

            const double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

            const bool cannot_refract = eta_ratio * sin_theta > 1.0;

            Vec3 direction;

            if (cannot_refract || reflectance(cos_theta, eta_ratio) > rng.uniform()) {
                direction = reflect(unit_direction, hit.normal);
            } else {
                direction = refract(unit_direction, hit.normal, eta_ratio);
            }

            return Scatter{
                .ray = Ray{hit.point, direction},
                .attenuation = Color{1.0, 1.0, 1.0},
            };
        }
    }

    std::unreachable();
}
