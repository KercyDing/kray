export module geometry:hit_record;

export import math;

export struct HitRecord {
    Point3 point{};
    Vec3 normal{};
    Color albedo;
    double t{};
    bool front_face{};

    void set_face_normal(const Ray &ray, const Vec3 &outward_normal) {
        front_face = dot(ray.direction(), outward_normal) < 0.0;

        normal = front_face ? outward_normal : -outward_normal;
    }
};
