module;

#include <cmath>

export module math.vec3;

export class Vec3 {
public:
    constexpr Vec3() = default;

    constexpr Vec3(double x, double y, double z) : e_{x, y, z} {}

    [[nodiscard]] constexpr double x() const { return e_[0]; }
    [[nodiscard]] constexpr double y() const { return e_[1]; }
    [[nodiscard]] constexpr double z() const { return e_[2]; }

    [[nodiscard]] constexpr Vec3 operator-() const { return Vec3{-e_[0], -e_[1], -e_[2]}; }

    [[nodiscard]] constexpr double operator[](int i) const { return e_[i]; }
    [[nodiscard]] constexpr double &operator[](int i) { return e_[i]; }

    constexpr Vec3 &operator+=(const Vec3 &other) {
        e_[0] += other.e_[0];
        e_[1] += other.e_[1];
        e_[2] += other.e_[2];
        return *this;
    }

    constexpr Vec3 &operator-=(const Vec3 &other) {
        e_[0] -= other.e_[0];
        e_[1] -= other.e_[1];
        e_[2] -= other.e_[2];
        return *this;
    }

    constexpr Vec3 &operator*=(double t) {
        e_[0] *= t;
        e_[1] *= t;
        e_[2] *= t;
        return *this;
    }

    constexpr Vec3 &operator/=(double t) { return *this *= 1.0 / t; }

    [[nodiscard]] constexpr double length_squared() const {
        return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
    }

    [[nodiscard]] double length() const { return std::sqrt(length_squared()); }

private:
    double e_[3]{0.0, 0.0, 0.0};
};

export [[nodiscard]]
constexpr Vec3 operator+(Vec3 lhs, const Vec3 &rhs) {
    lhs += rhs;
    return lhs;
}

export [[nodiscard]]
constexpr Vec3 operator-(Vec3 lhs, const Vec3 &rhs) {
    lhs -= rhs;
    return lhs;
}

export [[nodiscard]]
constexpr Vec3 operator*(Vec3 v, double t) {
    v *= t;
    return v;
}

export [[nodiscard]]
constexpr Vec3 operator*(double t, Vec3 v) {
    v *= t;
    return v;
}

export [[nodiscard]]
constexpr Vec3 operator/(Vec3 v, double t) {
    v /= t;
    return v;
}

export [[nodiscard]]
constexpr double dot(const Vec3 &a, const Vec3 &b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

export [[nodiscard]]
constexpr Vec3 lerp(const Vec3 &a, const Vec3 &b, double t) {
    return (1.0 - t) * a + t * b;
}

export [[nodiscard]]
Vec3 unit_vector(const Vec3 &v) {
    return v / v.length();
}

export using Point3 = Vec3;
export using Color = Vec3;
