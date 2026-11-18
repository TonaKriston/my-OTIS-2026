#pragma once

class Model {
private:
    double output_state{0.0};

public:
    Model() noexcept = default;
    
    explicit Model(double init_val) noexcept : output_state(init_val) {}
    
    virtual ~Model() = default;

    virtual auto step(double input_signal) -> double = 0;

    [[nodiscard]] auto retrieveState() const noexcept -> double {
        return output_state;
    }

    auto assignState(double new_val) noexcept -> void {
        output_state = new_val;
    }

    virtual auto reset(double init_val = 0.0) -> void {
        output_state = init_val;
    }
};