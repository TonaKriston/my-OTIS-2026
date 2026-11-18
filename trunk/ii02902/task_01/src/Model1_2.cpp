#include "Model1_2.h"

Model1_2::Model1_2(double a_val, double b_val, int k_val, double initial_y)
    : Model(initial_y), a(a_val), b(b_val), delay_k(k_val < 0 ? 0 : k_val) {
    u_history.resize(delay_k + 1, 0.0);
}

double Model1_2::step(double u) {
    // Сдвигаем историю входов
    if (!u_history.empty()) {
        for (size_t i = u_history.size() - 1; i > 0; --i) {
            u_history[i] = u_history[i - 1];
        }
        u_history[0] = u;
    }
    
    // Получаем вход с учетом задержки k
    double delayed_u = (u_history.size() > static_cast<size_t>(delay_k)) ? u_history[delay_k] : u;
    
    const double next_y = a * getY() + b * delayed_u;
    setY(next_y);
    return next_y;
}

void Model1_2::reset(double initial_y) {
    Model::reset(initial_y);
    std::fill(u_history.begin(), u_history.end(), 0.0);
}