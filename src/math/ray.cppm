export module math:ray;

export import :vec3;

export class Ray {
public:
    constexpr Ray() = default;

    constexpr Ray(Point3 origin, Vec3 direction) : origin_{origin}, direction_{direction} {}

    [[nodiscard]] constexpr Point3 origin() const {
        return origin_;
    }

    [[nodiscard]] constexpr Vec3 direction() const {
        return direction_;
    }

    [[nodiscard]] constexpr Point3 at(double t) const {
        return origin_ + t * direction_;
    }

private:
    Point3 origin_{};
    Vec3 direction_{};
};
