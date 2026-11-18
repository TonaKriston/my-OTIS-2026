#pragma once
#include <string>
#include <vector>

struct Sample {
    int step{};
    double time{};
    double u{};
    double y{};
};

class Model {
public:
    virtual ~Model() = default;
    virtual std::string name() const = 0;
    virtual double calculateNext(double u, int step, const std::vector<double>& history,
                                 double dt) const = 0;
};

inline double previousY(const std::vector<double>& history, int lag) {
    if (history.empty() || lag < 0) return 0.0;
    const long long index = static_cast<long long>(history.size()) - 1 - lag;
    if (index < 0) return 0.0;
    return history[static_cast<std::size_t>(index)];
}
