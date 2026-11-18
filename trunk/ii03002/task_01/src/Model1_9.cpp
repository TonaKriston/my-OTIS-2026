#include "Model1_9.h"

Model1_9::Model1_9(double p_a1, double p_a2, double p_a3, double p_b1, double init_val)
    : Model(init_val),
      a1_coeff(p_a1), a2_coeff(p_a2), a3_coeff(p_a3), b1_coeff(p_b1),
      state_minus_1(init_val), state_minus_2(init_val) {}

auto Model1_9::step(double input_signal) -> double {
    const double current_y = retrieveState();
    
    // Разбиваем уравнение для изменения структуры синтаксического дерева (AST)
    const double term_1 = a1_coeff * current_y;
    const double term_2 = a2_coeff * state_minus_1;
    const double term_3 = a3_coeff * state_minus_2;
    const double input_term = b1_coeff * input_signal;
    
    const double next_y = term_1 + term_2 + term_3 + input_term;
    
    // Сдвиг состояний в памяти
    state_minus_2 = state_minus_1;
    state_minus_1 = current_y;
    
    assignState(next_y);
    return next_y;
}

auto Model1_9::reset(double init_val) -> void {
    Model::reset(init_val);
    state_minus_1 = init_val;
    state_minus_2 = init_val;
}