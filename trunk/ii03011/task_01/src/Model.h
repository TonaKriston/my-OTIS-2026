#ifndef MODEL_H
#define MODEL_H

class Model {
private:
    double a;
    double b;
    double h;

protected:
    double getA() const { return a; }
    double getB() const { return b; }
    double getH() const { return h; }

public:
    Model(double a, double b, double h);
    virtual ~Model() = default;
    virtual double calculate(double y, double u) = 0;
};

class Model11 : public Model {
public:
    Model11(double a, double b);
    double calculate(double y, double u) override;
};

class Model25 : public Model {
public:
    Model25(double a, double b);
    double calculate(double y, double u) override;
};

class Model39 : public Model {
public:
    Model39(double b, double h);
    double calculate(double y, double u) override;
};

#endif