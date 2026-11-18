#include <gtest/gtest.h>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include "simulation.h"

// Тестовая модель
class TestModel : public Model {
public:
    int calls = 0;

    double nextStep(double u) override {
        calls++;
        return u * 2;
    }

    std::string getName() const override {
        return "TestModel";
    }
};

// Запуск моделирования с тестовым вводом
static std::string runWithInput(
    const std::string& inputText, TestModel& model) {

    std::istringstream input(inputText);
    std::ostringstream output;

    std::streambuf* oldIn = std::cin.rdbuf(input.rdbuf());
    std::streambuf* oldOut = std::cout.rdbuf(output.rdbuf());

    std::remove("simulation_test.csv");

    std::ofstream csvFile("simulation_test.csv");
    runSimulation(model, csvFile);
    csvFile.close();

    std::cin.rdbuf(oldIn);
    std::cout.rdbuf(oldOut);

    std::ifstream file("simulation_test.csv");
    std::string result;
    std::string line;

    while (std::getline(file, line)) {
        result += line + "\n";
    }

    file.close();
    std::remove("simulation_test.csv");

    return result;
}

// Проверка генерации сигналов

TEST(Signal, Step) {
    EXPECT_DOUBLE_EQ(generateSignal(1, 5.0, 7), 5.0);
}

TEST(Signal, PulseFirst) {
    EXPECT_DOUBLE_EQ(generateSignal(2, 5.0, 0), 5.0);
}

TEST(Signal, PulseLater) {
    EXPECT_DOUBLE_EQ(generateSignal(2, 5.0, 3), 0.0);
}

TEST(Signal, Harmonic) {
    EXPECT_NEAR(generateSignal(3, 2.0, 1),
                2.0 * std::sin(1.0), 1e-12);
}

TEST(Signal, Unknown) {
    EXPECT_DOUBLE_EQ(generateSignal(9, 5.0, 0), 0.0);
}

// Проверка runSimulation()

TEST(Simulation, Step) {
    TestModel model;
    std::string csv = runWithInput("1\n5\n3\n", model);

    EXPECT_EQ(model.calls, 3);
    EXPECT_NE(csv.find("TestModel;Step;0;5;10"), std::string::npos);
}

TEST(Simulation, Pulse) {
    TestModel model;
    std::string csv = runWithInput("2\n4\n3\n", model);

    EXPECT_EQ(model.calls, 3);
    EXPECT_NE(csv.find("TestModel;Pulse;0;4;8"), std::string::npos);
    EXPECT_NE(csv.find("TestModel;Pulse;1;0;0"), std::string::npos);
}

TEST(Simulation, Harmonic) {
    TestModel model;
    std::string csv = runWithInput("3\n2\n2\n", model);

    EXPECT_EQ(model.calls, 2);
    EXPECT_NE(csv.find("TestModel;Harmonic;0;0;0"), std::string::npos);
}

TEST(Simulation, InvalidSignal) {
    TestModel model;
    std::string csv = runWithInput("9\n5\n3\n", model);

    EXPECT_EQ(model.calls, 0);
    EXPECT_TRUE(csv.empty());
}

TEST(Simulation, ZeroSteps) {
    TestModel model;
    std::string csv = runWithInput("1\n5\n0\n", model);

    EXPECT_EQ(model.calls, 0);
    EXPECT_TRUE(csv.empty());
}
