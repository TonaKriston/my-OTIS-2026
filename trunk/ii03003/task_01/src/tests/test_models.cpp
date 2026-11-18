#include "models.hpp"
#include "simulation.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {
void expectClose(double actual, double expected) {
    if (!std::isfinite(actual) || std::abs(actual - expected) > 1e-10) {
        std::cerr << "Expected " << expected << ", got " << actual << '\n';
        std::abort();
    }
}
}

int main() {
    // Model 1.2: input with delay k=2.
    DelayedLinearModel delayed(0.5, 2.0, 2);
    const auto d = simulate(delayed, InputType::Impulse, 5, 0.1);
    expectClose(d[0].yNext, 0.0);
    expectClose(d[1].yNext, 0.0);
    expectClose(d[2].yNext, 2.0);
    expectClose(d[3].yNext, 1.0);
    expectClose(d[4].yNext, 0.5);

    // Model 2.6: with y_(-1)=y_0=0 and u_0=1, y_1=b.
    PolynomialNonlinearModel nonlinear(0.25, 0.15, 0.6);
    const auto nl = simulate(nonlinear, InputType::Step, 3, 0.1);
    expectClose(nl[0].yNext, 0.6);
    // At tau=1: y_2=0.25*(0.6)^3 -0.15*(0) + 0.6.
    expectClose(nl[1].yNext, 0.25 * std::pow(0.6, 3) + 0.6);
    // At tau=2, prior y_(tau-1)=y_1=0.6.
    expectClose(nl[2].yNext, 0.25 * std::pow(nl[1].yNext, 3) - 0.15 * 0.6 + 0.6);
    // Because input is squared, +/-1 give equal input contributions.
    const auto nlImpulse = simulate(nonlinear, InputType::Impulse, 2, 0.1);
    expectClose(nlImpulse[1].yNext, 0.25 * std::pow(0.6, 3));

    // Model 3.10: y_(tau+1)=y_tau + dt*(-a*y_tau+b+u_tau).
    OffsetEulerModel ode(0.7, 0.3);
    const auto o = simulate(ode, InputType::Step, 2, 0.1);
    expectClose(o[0].yNext, 0.13);
    expectClose(o[1].yNext, 0.13 + 0.1 * (-0.7 * 0.13 + 0.3 + 1.0));
    // At impulse tau=1: input is zero, but constant offset b is still added.
    const auto oi = simulate(ode, InputType::Impulse, 2, 0.1);
    expectClose(oi[1].yNext, 0.13 + 0.1 * (-0.7 * 0.13 + 0.3));

    expectClose(inputAt(InputType::Harmonic, 0), 0.0);
    expectClose(inputAt(InputType::Harmonic, 1), std::sin(1.0));
    expectClose(inputAt(InputType::Impulse, 3), 0.0);
    std::cout << "All variant-12 model tests passed.\n";
}
