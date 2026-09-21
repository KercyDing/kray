module;

#include <cmath>
#include <optional>

export module geometry:sphere;

export import math;
export import :hit_record;

export struct Sphere {
    Point3 center{};
    double radius{};
};

export [[nodiscard]]
std::optional<HitRecord> hit(const Sphere &sphere, const Ray &ray, double t_min, double t_max) {
    const Vec3 oc = ray.origin() - sphere.center;

    const double a = dot(ray.direction(), ray.direction());

    const double half_b = dot(oc, ray.direction());

    const double c = dot(oc, oc) - sphere.radius * sphere.radius;

    const double discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0) {
        return std::nullopt;
    }

    const double sqrt_d = std::sqrt(discriminant);

    double root = (-half_b - sqrt_d) / a;

    if (root <= t_min || root >= t_max) {
        root = (-half_b + sqrt_d) / a;

        if (root <= t_min || root >= t_max) {
            return std::nullopt;
        }
    }

    HitRecord record;

    record.t = root;
    record.point = ray.at(root);

    const Vec3 outward_normal = (record.point - sphere.center) / sphere.radius;

    record.set_face_normal(ray, outward_normal);

    return record;
}
