#pragma once
#include "DynamicModel.h"
#include "StateDelayModel.h"
#include "SaturationModel.h"
#include "QuadraticDecayODE.h"
#include <memory>
#include <iostream>
//  ModelCreator — фабрика для создания моделей по выбору пользователя
//  Инкапсулирует ввод параметров и создание конкретного наследника.
class ModelCreator
{
public:
    // Создаёт модель по выбору пользователя (1, 2, 3).
    // Запрашивает параметры через консоль.
    static std::unique_ptr<DynamicModel> create(int choice)
    {
        switch (choice)
        {
        case 1:
            return createStateDelay();
        case 2:
            return createSaturation();
        case 3:
            return createQuadraticDecay();
        default:
            std::cerr << "Unknown model number: " << choice << "\n";
            return nullptr;
        }
    }

private:
    // Model 1.5: State Delay
    static std::unique_ptr<DynamicModel> createStateDelay()
    {
        double a1;
        double a2;
        double b;
        int k;
        std::cout << "  a1 = "; std::cin >> a1;
        std::cout << "  a2 = "; std::cin >> a2;
        std::cout << "  b  = "; std::cin >> b;
        std::cout << "  k (delay, >=1) = "; std::cin >> k;
        if (k < 1)
        {
            k = 1;
        }

        return std::make_unique<StateDelayModel>(a1, a2, b, k);
    }

    // Model 2.2: Saturation
    static std::unique_ptr<DynamicModel> createSaturation()
    {
        double a;
        double b;
        double uMin;
        double uMax;
        std::cout << "  a = "; std::cin >> a;
        std::cout << "  b = "; std::cin >> b;
        std::cout << "  Umin = "; std::cin >> uMin;
        std::cout << "  Umax = "; std::cin >> uMax;

        return std::make_unique<SaturationModel>(a, b, uMin, uMax);
    }

    // Model 3.8: Quadratic Decay ODE
    static std::unique_ptr<DynamicModel> createQuadraticDecay()
    {
        double a1;
        double a2;
        double b;
        double dt;
        std::cout << "  a1 = "; std::cin >> a1;
        std::cout << "  a2 = "; std::cin >> a2;
        std::cout << "  b  = "; std::cin >> b;
        std::cout << "  dt = "; std::cin >> dt;

        return std::make_unique<QuadraticDecayODE>(a1, a2, b, dt);
    }
};