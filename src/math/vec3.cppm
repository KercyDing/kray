module;

#include <cmath>

export module math:vec3;

import :random;

export class Vec3 {
public:
    constexpr Vec3() = default;

    constexpr Vec3(const double x, const double y, const double z) : e_{x, y, z} {}

    [[nodiscard]] constexpr double x() const {
        return e_[0];
    }

    [[nodiscard]] constexpr double y() const {
        return e_[1];
    }

    [[nodiscard]] constexpr double z() const {
        return e_[2];
    }

    [[nodiscard]] constexpr Vec3 operator-() const {
        return Vec3{-e_[0], -e_[1], -e_[2]};
    }

    [[nodiscard]] constexpr double operator[](int i) const {
        return e_[i];
    }

    [[nodiscard]] constexpr double &operator[](int i) {
        return e_[i];
    }

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

    constexpr Vec3 &operator/=(double t) {
        return *this *= 1.0 / t;
    }

    [[nodiscard]] constexpr double length_squared() const {
        return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
    }

    [[nodiscard]] double length() const {
        return std::sqrt(length_squared());
    }

private:
    double e_[3]{0.0, 0.0, 0.0};
};

export {
    [[nodiscard]]
    constexpr Vec3 operator+(Vec3 lhs, const Vec3 &rhs) {
        lhs += rhs;
        return lhs;
    }

    [[nodiscard]]
    constexpr Vec3 operator-(Vec3 lhs, const Vec3 &rhs) {
        lhs -= rhs;
        return lhs;
    }

    [[nodiscard]]
    constexpr Vec3 operator*(Vec3 v, double t) {
        v *= t;
        return v;
    }

    [[nodiscard]]
    constexpr Vec3 operator*(double t, Vec3 v) {
        v *= t;
        return v;
    }

    [[nodiscard]]
    constexpr Vec3 operator*(const Vec3 &u, const Vec3 &v) {
        return {
            u.x() * v.x(),
            u.y() * v.y(),
            u.z() * v.z(),
        };
    }

    [[nodiscard]]
    constexpr Vec3 operator/(Vec3 v, const double t) {
        v /= t;
        return v;
    }
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

export [[nodiscard]]
Vec3 random_unit_vector(Rng &rng) {
    while (true) {
        auto direction = Vec3{
            2.0 * rng.uniform() - 1.0,
            2.0 * rng.uniform() - 1.0,
            2.0 * rng.uniform() - 1.0,
        };

        if (const double length_squared = direction.length_squared();
            length_squared > 1e-160 && length_squared <= 1.0) {
            direction /= std::sqrt(length_squared);
            return direction;
        }
    }
}

export [[nodiscard]]
Vec3 reflect(const Vec3 &v, const Vec3 &n) {
    return v - 2.0 * dot(v, n) * n;
}

export [[nodiscard]]
Vec3 refract(const Vec3 &uv, const Vec3 &n, const double eta_ratio) {
    const double cos_theta = std::min(dot(-uv, n), 1.0);

    const Vec3 r_out_perp = eta_ratio * (uv + cos_theta * n);

    const Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;

    return r_out_perp + r_out_parallel;
}

export using Point3 = Vec3;
export using Color = Vec3;
