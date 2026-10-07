#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

class Model { //абстрактный класс для наследования
public:
    virtual double step(double u) = 0; //подсчёт "у" на текущем шаге
    virtual void reset() = 0; //сброс данных перед новым запуском
    virtual ~Model() = default;
};

class LinearModel : public Model { //1.3 y[tau+1] = a1*y[tau] + a2*y[tau-1] + b*u[tau]
private:
    double a1;
    double a2;
    double b;
    double y_prev = 0; // y[tau-1]
    double y_curr = 0; // y[tau]

public:
    LinearModel(double a1_, double a2_, double b_) : a1(a1_), a2(a2_), b(b_) {
        //конструктор установки параметров уравнения 
    }

    double step(double u) override { //вычисление значения на одном шаге
        double y_next = a1 * y_curr + a2 * y_prev + b * u;
        y_prev = y_curr;
        y_curr = y_next;
        return y_curr;
    }

    void reset() override {
        y_prev = 0;
        y_curr = 0;
    }

    //уравнение 1.3 y[tau+1] = a1*y[tau] + a2*y[tau-1] + b*u[tau] приводим к характеристическому (y[tau] = z^tau) виду:
    //получаем z^2 - a1*z - a2 = 0
    //находим корни и выясняем, что система устойчива, если |z1| < 1 и |z2| < 1
    bool isStable() const { //проверка коэффициентов на устойчивость
        double disc = a1 * a1 + 4 * a2; //дискриминант
        if (disc >= 0) { //елси дискриминант больше нуля, то корни вещественные
            double z1 = (a1 + sqrt(disc)) / 2;
            double z2 = (a1 - sqrt(disc)) / 2;
            return fabs(z1) < 1 && fabs(z2) < 1;
        }
        else { //если дискриминант меньше нуля, то корни комплексные
            return sqrt(-a2) < 1;
        }
    }
};

class NonlinearModel : public Model { //2.10 y[tau+1] = a*tanh(y[tau]) + b*u[tau]^3
private:
    double a;
    double b;
    double y_curr = 0;

public:
    NonlinearModel(double a_, double b_) : a(a_), b(b_) {
       
    }

    double step(double u) override {
        y_curr = a * tanh(y_curr) + b * u * u * u;
        return y_curr;
    }

    void reset() override {
        y_curr = 0;
    }

};

//при решении методом Эйлера приводим уравнение к виду y = y + b*u*dt
class DifferentialModel : public Model { //3.2 dy/dt = b*u
private:
    double b;
    double dt;
    double y_curr = 0;

public:
    DifferentialModel(double b_, double dt_) : b(b_), dt(dt_) {
       
    }

    double step(double u) override {
        y_curr = y_curr + b * u * dt;
        return y_curr;
    }

    void reset() override {
        y_curr = 0;
    }

};

double generateSignal(int type, int tau) { //генерация входного "u"
    switch (type) {
    case 0: return 1.0; //ступенчатое
    case 1: return (tau == 0) ? 1.0 : 0.0; //импульсное
    case 2: return sin(tau); //гармоническое
    default: return 0.0;
    }
}

void runSimulation(Model* model, int n, int signalType, const string& filename) { //запуск модели и вывод результатов
    ofstream fout(filename);
    fout << fixed << setprecision(6); //настройка формата вывода

    model->reset(); //используем указатель абстрактного родительского класса, т.к. он может хранить ссылку на любой класс-наследник

    for (int tau = 0; tau < n; tau++) {
        double u = generateSignal(signalType, tau);
        double y = model->step(u);

        fout << tau << "," << u << "," << y << "\n";
    }

    fout.close();
}


int main() {
    int n;

    cout << "Enter number of inerations: "; cin >> n;
    if (n <= 0) {
        cout << "Wrong value"; exit(1);
    }

    double a;
    double b;
    double c;
    cout << "Enter linear coefficients:\na1: "; cin >> a;
    cout << "a2: "; cin >> b;
    cout << "b: "; cin >> c;

    LinearModel lin(a, b, c);
    if (!lin.isStable()) {
        cout << "\nDivirgent process\n";
    }
    runSimulation(&lin, n, 0, "linear_step.csv");
    runSimulation(&lin, n, 1, "linear_impulse.csv");
    runSimulation(&lin, n, 2, "linear_harmonic.csv");

    cout << "All lineaar processes have been completed\n\n";

    cout << "Enter nonlinear coefficients:\na: "; cin >> a;
    cout << "b: "; cin >> b;

    NonlinearModel nonlin(a, b);
    runSimulation(&nonlin, n, 0, "nonlinear_step.csv");
    runSimulation(&nonlin, n, 1, "nonlinear_impulse.csv");
    runSimulation(&nonlin, n, 2, "nonlinear_harmonic.csv");

    cout << "All nonlineaar processes have been completed\n\n";

    cout << "Enter differential coefficients:\nb: "; cin >> a;
    cout << "dt: "; cin >> b;

    DifferentialModel dif(a, b);
    runSimulation(&dif, n, 0, "dif_step.csv");
    runSimulation(&dif, n, 1, "dif_impulse.csv");
    runSimulation(&dif, n, 2, "dif_harmonic.csv");

    //я знаю, что жёсткий путь - это нехорошо, но у меня иначе не запускается :(
    system(R"("C:\Program Files\gnuplot\bin\gnuplot.exe\" linear_step.gp)");
    system(R"("C:\Program Files\gnuplot\bin\gnuplot.exe\" linear_impulse.gp)");
    system(R"("C:\Program Files\gnuplot\bin\gnuplot.exe\" linear_harmonic.gp)");

    system(R"("C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" nonlinear_step.gp)");
    system(R"("C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" nonlinear_impulse.gp)");
    system(R"("C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" nonlinear_harmonic.gp)");

    system(R"("C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" dif_step.gp)");
    system(R"("C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" dif_impulse.gp)");
    system(R"("C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" dif_harmonic.gp)");

    return 0;
}
