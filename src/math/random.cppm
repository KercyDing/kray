module;

#include <cstdint>

export module math:random;

export class Rng {
public:
    explicit Rng(const std::uint64_t seed, const std::uint64_t stream = 0) {
        std::uint64_t state = seed ^ (stream * 0x9e3779b97f4a7c15ull);

        for (std::uint64_t &word : s_) {
            state += 0x9e3779b97f4a7c15ull;

            std::uint64_t z = state;
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
            z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;

            word = z ^ (z >> 31);
        }
    }

    [[nodiscard]]
    double uniform() {
        const std::uint64_t result = rotl(s_[1] * 5, 7) * 9;

        const std::uint64_t t = s_[1] << 17;

        s_[2] ^= s_[0];
        s_[3] ^= s_[1];
        s_[1] ^= s_[2];
        s_[0] ^= s_[3];

        s_[2] ^= t;
        s_[3] = rotl(s_[3], 45);

        return static_cast<double>(result >> 11) * 0x1.0p-53;
    }

private:
    [[nodiscard]]
    static constexpr std::uint64_t rotl(const std::uint64_t x, const int k) {
        return (x << k) | (x >> (64 - k));
    }

    std::uint64_t s_[4]{};
};
