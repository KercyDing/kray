module;

#include <cstdint>
#include <random>

export module math:random;

export class Rng {
public:
    explicit Rng(const std::uint64_t seed) : engine_{seed} {}

    Rng() : Rng{std::random_device{}()} {}

    [[nodiscard]]
    double uniform() {
        return distribution_(engine_);
    }

private:
    std::mt19937_64 engine_;
    std::uniform_real_distribution<double> distribution_{0.0, 1.0};
};
