#include <gtest/gtest.h>
#include <array>
#include <cmath>

#include "../../task_01/src/model1.6.h"
#include "../../task_01/src/model2.1.h"
#include "model3.7.h"
#include "signal.h"

// ---------- Модель 1.6 ----------

struct Case16 {
    const char* name;
    double a1, a2, a3, b;
    int steps;
    std::array<double, 5> u;   // подменённый входной сигнал
    std::array<double, 5> y;   // эталонные значения (рассчитаны вручную)
};

class Model16 : public ::testing::TestWithParam<Case16> {};

TEST_P(Model16, MatchesHandComputedValues) {
    const Case16 c = GetParam();
    model1_6 m(c.a1, c.a2, c.a3, c.b);
    for (int i = 0; i < c.steps; ++i)
        EXPECT_NEAR(m.stepForward(c.u[i]), c.y[i], 1e-9) << "шаг " << i;
}

static const std::array<Case16, 4> kCases16{{
    // ступень A=2:  y0 = 0.5*2 = 1.0;  y1 = 0.3*1 + 0.5*2 = 1.3;  y2 = 1.59
    {"step_A2", 0.3, 0.2, 0.1, 0.5, 3,
     {2.0, 2.0, 2.0, 0.0, 0.0}, {1.0, 1.3, 1.59, 0.0, 0.0}},
    // импульс A=1:  y0 = 0.5;  y1 = 0.3*0.5 = 0.15;  y2 = 0.3*0.15 + 0.2*0.5 = 0.145
    {"impulse_A1", 0.3, 0.2, 0.1, 0.5, 3,
     {1.0, 0.0, 0.0, 0.0, 0.0}, {0.5, 0.15, 0.145, 0.0, 0.0}},
    // гармоника A=1: u = sin(t), радианы
    {"harmonic_A1", 0.3, 0.2, 0.1, 0.5, 3,
     {0.0, 0.841470984808, 0.909297426826, 0.0, 0.0},
     {0.0, 0.420735492404, 0.580869361134, 0.0, 0.0}},
    // инвариант покоя: нулевой вход и нулевое состояние -> y = 0
    {"rest_invariant", 0.3, 0.2, 0.1, 0.5, 3,
     {0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0}},
}};

INSTANTIATE_TEST_SUITE_P(Trajectories, Model16, ::testing::ValuesIn(kCases16),
    [](const ::testing::TestParamInfo<Case16>& info) {
        return info.param.name;
    });

// clearState возвращает модель в начальное состояние
TEST(Model16Extra, ClearStateRestoresInitialValues) {
    model1_6 m(0.3, 0.2, 0.1, 0.5);
    m.stepForward(2.0);
    m.stepForward(2.0);
    m.clearState();
    model1_6 fresh(0.3, 0.2, 0.1, 0.5);
    EXPECT_DOUBLE_EQ(m.stepForward(2.0), fresh.stepForward(2.0));
}

// Виртуальные методы и виртуальный деструктор: модель через базовый указатель,
// как unique_ptr в main.cpp
TEST(Model16Extra, WorksThroughBasePointer) {
    BaseModel* m = new model1_6(0.3, 0.2, 0.1, 0.5);
    EXPECT_DOUBLE_EQ(m->stepForward(2.0), 1.0);
    EXPECT_DOUBLE_EQ(m->stepForward(2.0), 1.3);
    m->clearState();
    EXPECT_DOUBLE_EQ(m->stepForward(2.0), 1.0);
    delete m;
}

// ---------- Модель 2.1 ----------

struct Case21 {
    const char* name;
    double a, b, c, d;
    int steps;
    std::array<double, 5> u;
    std::array<double, 5> y;
};

class Model21 : public ::testing::TestWithParam<Case21> {};

TEST_P(Model21, MatchesHandComputedValues) {
    const Case21 c = GetParam();
    model2_1 m(c.a, c.b, c.c, c.d);
    for (int i = 0; i < c.steps; ++i)
        EXPECT_NEAR(m.stepForward(c.u[i]), c.y[i], 1e-9) << "шаг " << i;
}

static const std::array<Case21, 3> kCases21{{
    // ступень A=1:  y0 = 1.0;  y1 = 0.4*1 + 1 + 0.5*sin(1) = 1.820735;  y2 = 2.099030
    {"step_A1", 0.4, 0.05, 1.0, 0.5, 3,
     {1.0, 1.0, 1.0, 0.0, 0.0}, {1.0, 1.820735492404, 2.099029689366, 0.0, 0.0}},
    // импульс A=1:  y0 = 1.0;  y1 = 0.4*1 + 0.5*sin(1) = 0.820735;  y2 = 0.278294
    {"impulse_A1", 0.4, 0.05, 1.0, 0.5, 3,
     {1.0, 0.0, 0.0, 0.0, 0.0}, {1.0, 0.820735492404, 0.278294196962, 0.0, 0.0}},
    // квадратичный член: a=0, b=1, c=2, d=0, u={1,0,0,0,0} ->
    // y = {2; 0; -4; 0; -16}; квадрат прошлого выхода виден по росту
    {"quadratic_brake", 0.0, 1.0, 2.0, 0.0, 5,
     {1.0, 0.0, 0.0, 0.0, 0.0}, {2.0, 0.0, -4.0, 0.0, -16.0}},
}};

INSTANTIATE_TEST_SUITE_P(Trajectories, Model21, ::testing::ValuesIn(kCases21),
    [](const ::testing::TestParamInfo<Case21>& info) {
        return info.param.name;
    });

TEST(Model21Extra, ClearStateRestoresInitialValues) {
    model2_1 m(0.4, 0.05, 1.0, 0.5);
    m.stepForward(1.0);
    m.stepForward(1.0);
    m.clearState();
    model2_1 fresh(0.4, 0.05, 1.0, 0.5);
    EXPECT_DOUBLE_EQ(m.stepForward(1.0), fresh.stepForward(1.0));
}

// ---------- Модель 3.7 ----------

struct Case37 {
    const char* name;
    double a, b, h;
    int steps;
    std::array<double, 5> u;
    std::array<double, 5> y;
};

class Model37 : public ::testing::TestWithParam<Case37> {};

TEST_P(Model37, MatchesHandComputedValues) {
    const Case37 c = GetParam();
    model3_7 m(c.a, c.b, c.h);
    for (int i = 0; i < c.steps; ++i)
        EXPECT_NEAR(m.stepForward(c.u[i]), c.y[i], 1e-9) << "шаг " << i;
}

static const std::array<Case37, 3> kCases37{{
    // ступень A=1, e^0.2 = 1.221403:  y0 = 0.5;  y1 = 0.694649;  y2 = 0.770426
    {"step_A1", 0.2, 1.0, 0.5, 3,
     {1.0, 1.0, 1.0, 0.0, 0.0}, {0.5, 0.694649310460, 0.770426018585, 0.0, 0.0}},
    // импульс A=1:  y0 = 0.5;  y1 = 0.194649;  y2 = 0.075777
    {"impulse_A1", 0.2, 1.0, 0.5, 3,
     {1.0, 0.0, 0.0, 0.0, 0.0}, {0.5, 0.194649310460, 0.075776708125, 0.0, 0.0}},
    // инвариант покоя: u = 0, y0 = 0 -> y = 0
    {"rest_invariant", 0.2, 1.0, 0.5, 3,
     {0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0}},
}};

INSTANTIATE_TEST_SUITE_P(Trajectories, Model37, ::testing::ValuesIn(kCases37),
    [](const ::testing::TestParamInfo<Case37>& info) {
        return info.param.name;
    });

// Схема Эйлера решает именно своё уравнение: при a=0, b=0 точное решение
// dy/dt = -y с y(0) = 1 равно y(t) = e^-t
TEST(Model37Extra, EulerSchemeMatchesAnalyticSolution) {
    model3_7 m(0.0, 0.0, 0.01, 1.0);   // y0 = 1 через необязательный параметр
    double y = 1.0;
    for (int i = 0; i < 100; ++i)
        y = m.stepForward(0.0);
    // 100 шагов Эйлера с h = 0.01: y = (1 - h)^100
    EXPECT_NEAR(y, std::exp(100.0 * std::log(0.99)), 1e-12);
    // аналитическое значение при t = 1: e^-1 (разница ~ h/2 - точность первого порядка)
    EXPECT_NEAR(y, std::exp(-1.0), 0.01);
}

// clearState возвращает в начальное значение y0, а не в ноль
TEST(Model37Extra, ClearStateRestoresStartingValue) {
    model3_7 m(0.2, 1.0, 0.5, 2.0);
    m.stepForward(1.0);
    m.clearState();
    model3_7 fresh(0.2, 1.0, 0.5, 2.0);
    EXPECT_DOUBLE_EQ(m.stepForward(1.0), fresh.stepForward(1.0));
    EXPECT_DOUBLE_EQ(m.stepForward(1.0), fresh.stepForward(1.0));
}