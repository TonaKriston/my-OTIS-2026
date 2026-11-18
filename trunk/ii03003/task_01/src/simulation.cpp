#include "simulation.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

// Domain-specific exception for simulation, numerical and output failures.
class SimulationFailure : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

std::string inputName(InputType type) {
    switch (type) {
        case InputType::Step: return "step";
        case InputType::Impulse: return "impulse";
        case InputType::Harmonic: return "harmonic";
    }
    throw std::invalid_argument("Unknown input type");
}

double inputAt(InputType type, std::size_t step) {
    switch (type) {
        case InputType::Step: return 1.0;
        case InputType::Impulse: return step == 0 ? 1.0 : 0.0;
        case InputType::Harmonic: return std::sin(static_cast<double>(step));
    }
    throw std::invalid_argument("Unknown input type");
}

std::vector<Row> simulate(const Model& model, InputType signal, std::size_t n,
                          double dt, double y0) {
    if (n == 0 || !(dt > 0.0) || !std::isfinite(dt) || !std::isfinite(y0)) {
        throw SimulationFailure("n must be positive, dt must be finite and >0");
    }
    std::vector<double> inputs;
    inputs.reserve(n);
    std::vector<Row> rows;
    rows.reserve(n);
    double y = y0;
    double previousY = 0.0;  // Initial history y_(-1) = 0 for Model 2.6.
    for (std::size_t tau = 0; tau < n; ++tau) {
        const double u = inputAt(signal, tau);
        inputs.push_back(u);
        const double next = model.nextStep({tau, y, previousY, u, inputs, dt});
        if (!std::isfinite(next)) {
            throw SimulationFailure(model.id() + " diverged at step " + std::to_string(tau)
                + "; change coefficients or the integration step");
        }
        rows.push_back({tau, u, y, next});
        previousY = y;
        y = next;
    }
    return rows;
}

void writeCsv(const std::filesystem::path& path, const std::vector<Row>& rows) {
    std::ofstream out(path);
    if (!out) throw SimulationFailure("Cannot open CSV: " + path.string());
    out << "tau,u_tau,y_tau,y_tau_plus_1\n";
    out << std::setprecision(12);
    for (const auto& r : rows) {
        out << r.tau << ',' << r.input << ',' << r.yCurrent << ',' << r.yNext << '\n';
    }
    if (!out) throw SimulationFailure("Error writing CSV: " + path.string());
}

namespace {
std::string number(double value, int precision = 3) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

std::string polyline(const std::vector<Row>& rows, double left, double top,
                     double plotWidth, double plotHeight,
                     double low, double high) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    for (std::size_t i = 0; i < rows.size(); ++i) {
        const double x = left + (rows.size() == 1 ? 0.0
            : plotWidth * static_cast<double>(i) / static_cast<double>(rows.size() - 1));
        const double y = top + (high - rows[i].yNext) / (high - low) * plotHeight;
        if (i) ss << ' ';
        ss << x << ',' << y;
    }
    return ss.str();
}
} // namespace

void writeSvg(const std::filesystem::path& path, const Model& model,
              const std::vector<Row>& stepRows,
              const std::vector<Row>& impulseRows,
              const std::vector<Row>& harmonicRows) {
    if (stepRows.empty() || stepRows.size() != impulseRows.size()
        || stepRows.size() != harmonicRows.size()) {
        throw SimulationFailure("SVG requires equally sized, nonempty series");
    }
    double minY = 0.0;
    double maxY = 0.0;
    for (const auto* rows : {&stepRows, &impulseRows, &harmonicRows}) {
        for (const auto& r : *rows) {
            minY = std::min(minY, r.yNext);
            maxY = std::max(maxY, r.yNext);
        }
    }
    const double padding = std::max(0.15, (maxY - minY) * 0.12);
    minY -= padding;
    maxY += padding;

    constexpr double L = 88;
    constexpr double T = 113;
    constexpr double W = 960;
    constexpr double H = 420;
    std::ofstream out(path);
    if (!out) throw SimulationFailure("Cannot open SVG: " + path.string());
    out << R"SVG(<svg xmlns="http://www.w3.org/2000/svg" width="1130" height="650" )SVG"
           R"SVG(viewBox="0 0 1130 650">
)SVG";
    out << R"SVG(<rect width="1130" height="650" fill="#ffffff"/>
)SVG";
    out << R"SVG(<text x="88" y="48" font-size="27" font-weight="bold" )SVG"
           R"SVG(font-family="Arial, sans-serif" fill="#172135">)SVG"
        << model.title() << "</text>\n";
    out << R"SVG(<text x="88" y="78" font-size="15" )SVG"
           R"SVG(font-family="Arial, sans-serif" fill="#51627a">)SVG"
           "Variant 12 | output y(tau+1) | input step, impulse, sin(tau)</text>\n";
    out << R"SVG(<rect x=")SVG" << L << R"SVG(" y=")SVG" << T << R"SVG(" width=")SVG" << W
        << R"SVG(" height=")SVG" << H << R"SVG(" fill="#f8fafc"/>
)SVG";
    for (int i = 0; i <= 5; ++i) {
        const double y = T + H * i / 5.0;
        const double v = maxY - (maxY - minY) * i / 5.0;
        out << R"SVG(<line x1=")SVG" << L << R"SVG(" x2=")SVG" << (L + W)
            << R"SVG(" y1=")SVG" << y << R"SVG(" y2=")SVG" << y
            << R"SVG(" stroke="#d8e0ea"/>
)SVG";
        out << R"SVG(<text x=")SVG" << (L - 13) << R"SVG(" y=")SVG" << y + 5
            << R"SVG(" text-anchor="end" font-family="Arial" font-size="13" )SVG"
               R"SVG(fill="#51627a">)SVG" << number(v) << "</text>\n";
    }
    for (int i = 0; i <= 5; ++i) {
        const double x = L + W * static_cast<double>(i) / 5.0;
        const auto step = 1 + static_cast<std::size_t>(std::llround(
            static_cast<double>(stepRows.size() - 1) * static_cast<double>(i) / 5.0));
        out << R"SVG(<line x1=")SVG" << x << R"SVG(" x2=")SVG" << x
            << R"SVG(" y1=")SVG" << T << R"SVG(" y2=")SVG" << (T + H)
            << R"SVG(" stroke="#e7edf4"/>
)SVG";
        out << R"SVG(<text x=")SVG" << x << R"SVG(" y=")SVG" << (T + H + 26)
            << R"SVG(" text-anchor="middle" font-family="Arial" font-size="13" )SVG"
               R"SVG(fill="#51627a">)SVG" << step << "</text>\n";
    }
    const std::array<const std::vector<Row>*, 3> lines = {&stepRows, &impulseRows, &harmonicRows};
    const std::array<const char*, 3> colors = {"#2463d4", "#e17d30", "#19926e"};
    const std::array<const char*, 3> labels = {"Step: u=1", "Impulse: u(0)=1", "Harmonic: sin(tau)"};
    for (std::size_t i = 0; i < lines.size(); ++i) {
        out << R"SVG(<polyline points=")SVG" << polyline(*lines[i], L, T, W, H, minY, maxY)
            << R"SVG(" fill="none" stroke=")SVG" << colors[i]
            << R"SVG(" stroke-width="2.8" stroke-linecap="round" )SVG"
               R"SVG(stroke-linejoin="round"/>
)SVG";
        const double legendX = L + static_cast<double>(i) * 315.0;
        out << R"SVG(<line x1=")SVG" << legendX << R"SVG(" x2=")SVG" << (legendX + 35)
            << R"SVG(" y1="610" y2="610" stroke=")SVG" << colors[i]
            << R"SVG(" stroke-width="4"/>
)SVG";
        out << R"SVG(<text x=")SVG" << legendX + 46
            << R"SVG(" y="615" font-family="Arial" font-size="16" )SVG"
               R"SVG(fill="#22334d">)SVG" << labels[i] << "</text>\n";
    }
    out << R"SVG(<text x="568" y="580" font-family="Arial" font-size="15" )SVG"
           R"SVG(text-anchor="middle" fill="#334155">Simulation step (tau+1)</text>
)SVG";
    out << R"SVG(<text x="27" y="326" font-family="Arial" font-size="15" )SVG"
           R"SVG(text-anchor="middle" fill="#334155" )SVG"
           R"SVG(transform="rotate(-90 27 326)">y(tau+1)</text>
)SVG";
    out << "</svg>\n";
    if (!out) throw SimulationFailure("Error writing SVG: " + path.string());
}
