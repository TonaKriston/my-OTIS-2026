#pragma once

#include <string>

//  DynamicModel — абстрактный базовый класс для всех моделей
//  Определяет общий интерфейс: шаг симуляции, сброс, описание,
//  проверка устойчивости.
class DynamicModel
{
public:
    virtual ~DynamicModel() = default;

    // Один шаг симуляции: на входе управление u, на выходе новое y.
    virtual double advance(double u) = 0;

    // Приведение модели в исходное состояние.
    virtual void reset() = 0;

    // Текстовое описание модели.
    virtual std::string describe() const = 0;

    // Проверка устойчивости (для линейных моделей — через корни
    // характеристического уравнения; по умолчанию — устойчива).
    virtual bool verifyStability() const
    {
        return true;
    }

    // Предупреждение о неустойчивости (пустая строка — если всё ок).
    virtual std::string getWarningMessage() const
    {
        return "";
    }
};