<p align="center"> Министерство образования Республики Беларусь</p>
<p align="center">Учреждение образования</p>
<p align="center">“Брестский Государственный технический университет”</p>
<p align="center">Кафедра ИИТ</p>
<br><br><br><br><br><br><br>
<p align="center">Лабораторная работа №1</p>
<p align="center">По дисциплине “Общая теория интеллектуальных систем”</p>
<p align="center">Тема: “Моделирования температуры объекта”</p>
<br><br><br><br><br>
<p align="right">Выполнил:</p>
<p align="right">Студент 2 курса</p>
<p align="right">Группы ИИ-29</p>
<p align="right">Афанасьев Д. Р.</p>
<p align="right">Проверил:</p>
<p align="right">Дворанинович Д. А.</p>
<br><br><br><br><br>
<p align="center">Брест 2026</p>

## Вариант 1

- Линейная модель: **Model 1.1**
- Нелинейная модель: **Model 2.3**
- Дифференциальное уравнение: **Model 3.5**

## Реализованные модели

### Model 1.1

`y(t+1) = a*y(t) + b*u(t)`

### Model 2.3

`y(t+1) = a*y(t) + b*deadzone(u(t))`

$$
\text{where } \text{deadzone}(u) =
\begin{cases}
u - \delta, & u > \delta \\
0, & -\delta \le u \le \delta \\
u + \delta, & u < -\delta
\end{cases}
$$

### Model 3.5

`dy/dt = b*sin(u)`

## Входные воздействия

1. **Ступенчатое:** `u(t) = 1.0`
2. **Импульсное:** `u(0) = 1.0`, `u(t > 0) = 0.0`
3. **Гармоническое:** `u(t) = sin(t)`

## ООП

Создан абстрактный базовый класс `model` с виртуальным методом `step(double y_tau, double u_tay)`.

От него наследуются:
- `Model1_1`
- `Model2_3`
- `Model3_5`

## UML-диаграмма
```mermaid
classDiagram
    class model {
        <<abstract>>
        +step(double y_tau, double u_tau) double
    }

    class model1_1 {
        -a, b : double
        +model1_1(double a, double b)
        +step(double y_tau, double u_tau) double
    }

    class model2_3 {
        -a, b, delta : double
        -deadzone(double u) double
        +model2_3(double a, double b, double delta)
        +step(double y_tau, double u_tau) double
    }

    class model3_5 {
        -b, dt : double
        +model3_5(double b, double dt)
        +step(double y_tau, double u_tau) double
    }

    model <|-- model1_1
    model <|-- model2_3
    model <|-- model3_5
```

## Пример работы программы:

### Сборка

На Windows build.bat:

```bat
@echo off
chcp 65001 >nul
echo ==========================================
echo   Автоматическая сборка и запуск проекта
echo ==========================================

if not exist build mkdir build
cd build

echo.
echo Конфигурация CMake...
echo ------------------------------------------
cmake ../src
if %errorlevel% neq 0 (
    echo Ошибка конфигурации CMake!
    pause
    exit /b 1
)

echo.
echo Компиляция проекта ...
echo ------------------------------------------
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Ошибка компиляции!
    pause
    exit /b 1
)

echo.
echo Запуск симуляции...
echo ------------------------------------------

if exist Release\ii02901_task01.exe (
    Release\ii02901_task01.exe
) else if exist Debug\ii02901_task01.exe (
    Debug\ii02901_task01.exe
) else if exist ii02901_task01.exe (
    ii02901_task01.exe
) else (
    echo Исполняемый файл не найден!
)

echo.
echo ==========================================
echo   Построение графиков ...
echo ==========================================
py ../src/plot.py
if %errorlevel% neq 0 (
    echo [!] Ошибка! Проверьте установленны ли python, matplotlib, pandas.
)

echo   Процесс успешно завершен!
pause
```
### Демонстрация реализации:
![](sim.png)
### Демонстрация графиков:
![](graph.png)

## Результат

Программа:
- запрашивает количество шагов симуляции `n`;
- запрашивает тип модели для симуляции;
- запрашивает коэффициенты для модели;
- симулирует модель;
- выводит результат в виде таблицы;
- сохраняет результат в `.csv` файл;
- строит график через Matplotlib (Python).
