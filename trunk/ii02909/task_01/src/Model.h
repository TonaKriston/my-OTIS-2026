#pragma once

#include <string>

// Абстрактный базовый класс для всех моделей
class Model {
public:
    virtual ~Model() = default;

    // Один шаг симуляции: подаём вход u, получаем выход y
    virtual double next(double u) = 0;

    // Сброс состояния модели в начальное
    virtual void reset() = 0;

    // Имя модели (для вывода в таблицу / отчёты)
    virtual std::string name() const = 0;

    // ---- Проверка устойчивости (для линейных моделей) ----
    // По умолчанию модель считается устойчивой (для нелинейных / дифуров)
    virtual bool isStable() const { return true; }

    // Текст предупреждения (пустая строка, если всё ок)
    virtual std::string stabilityWarning() const { return ""; }
};