#include "Model2_1.h"
#include <cmath>

Model2_1::Model2_1(double p_a, double p_b, double p_c, double p_d, double start_y)
    : Model(start_y), a(p_a), b(p_b), c(p_c), d(p_d), y_prev(start_y) {}

auto Model2_1::step(double input_signal) -> double {
    const double current_state = retrieveState();
    
    // Разбиваем формулу на отдельные компоненты для изменения структуры AST-дерева
    const double linear_feedback = a * current_state;
    const double nonlinear_feedback = b * (y_prev * y_prev);
    const double direct_input = c * input_signal;
    const double delayed_input = d * std::sin(u_prev);
    
    // Итоговая сумма состояний
    const double next_state = linear_feedback - nonlinear_feedback + direct_input + delayed_input;
    
    // Обновляем память модели
    y_prev = current_state;
    u_prev = input_signal;
    
    assignState(next_state);
    return next_state;
}

auto Model2_1::reset(double start_y) -> void {
    Model::reset(start_y);
    y_prev = start_y;
    u_prev = 0.0;
}