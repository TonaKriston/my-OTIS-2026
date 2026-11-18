#pragma once

class Model {
private:
    double m_y{0.0};

protected:
    void setY(double next_val) noexcept { 
        m_y = next_val; 
    }

public:
    Model() = default;
    explicit Model(double start_val) : m_y(start_val) {}
    virtual ~Model() = default;

    virtual double step(double input_u) = 0;

    [[nodiscard]] double getY() const noexcept { 
        return m_y; 
    }
    
    virtual void reset(double start_val = 0.0) { 
        m_y = start_val; 
    }
};