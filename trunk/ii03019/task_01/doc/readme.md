<p align="center">Министерство образования Республики Беларусь</p>
<p align="center">Учреждение образования</p>
<p align="center">«Брестский государственный технический университет»</p>
<p align="center">Кафедра ИИТ</p>
<br><br><br><br><br>
<p align="center"><b>Лабораторная работа №1</b></p>
<p align="center"><b>По дисциплине: «Общая теория интеллектуальных систем»</b></p>
<p align="center"><b>Тема: «Моделирование динамических систем»</b></p>
<br><br><br><br>
<p align="right">Выполнил:</p>
<p align="right">Студент 2 курса</p>
<p align="right">Группы ИИ-30</p>
<p align="right">Сарайкин Н. Н.</p>
<p align="right">Проверил:</p>
<p align="right">Дворанинович Д. А.</p>
<br><br><br><br>
<p align="center">Брест 2026</p>

---

## Вариант 11

- Линейная модель: **Model 1.1**
- Нелинейная модель: **Model 2.5**
- Дифференциальное уравнение: **Model 3.9**

## Реализованные модели

| Блок | Модель | Тип | Название |
|:--:|:--:|:--:|---|
| 1 | 1.1 | линейная дискретная | Standard First-Order Inertial System |
| 2 | 2.5 | нелинейная дискретная | Signum Friction and Exponential Growth |
| 3 | 3.9 | дифференциальное уравнение | Bounded Saturation Rate |

## Входные воздействия

1. Ступенчатое: `u(t) = A`
2. Импульсное: `u(0) = A`, `u(t > 0) = 0`
3. Гармоническое: `u(t) = A * sin(t)`

## ООП

Создан абстрактный базовый класс `Model` с чисто виртуальным методом `step(double u)`.

От него наследуются классы конкретных моделей:
- `Model1_1`
- `Model2_5`
- `Model3_9`

## UML Диаграмма классов

```mermaid
classDiagram
    class Model {
        <<abstract>>
        #double y
        +Model(initial_y: double)
        +~Model()*
        +step(u: double)* double
        +getState() double
        +reset(initial_y: double) void
    }

    class Model1_1 {
        -double a
        -double b
        +Model1_1(a_val: double, b_val: double, initial_y: double)
        +step(u: double) double
    }

    class Model2_5 {
        -double a
        -double b
        -sign(val: double)\$ double
        +Model2_5(a_val: double, b_val: double, initial_y: double)
        +step(u: double) double
    }

    class Model3_9 {
        -double b
        -double dt
        +Model3_9(b_val: double, dt_val: double, initial_y: double)
        +step(u: double) double
    }

    class InputSignal {
        +getSignal(type: SignalType, tau: int)\$ double
        +getSignalName(type: SignalType)\$ string
    }

    Model <|-- Model1_1
    Model <|-- Model2_5
    Model <|-- Model3_9
    InputSignal ..> Model : generates input u
```

## Пример работы программы

### Сборка и запуск

Сборка осуществляется с помощью файла `build.bat`:

```bat
@echo off
setlocal
chcp 65001 >nul

if not exist "%~dp0CMakeLists.txt" (
    echo ERROR CMakeLists not found in this folder
    pause
    exit /b 1
)

cd /d "%~dp0"

echo STARTING BUILD PROCESS

echo STAGE 1 Configuration
cmake -S . -B build -G "MinGW Makefiles"
if errorlevel 1 goto error

echo STAGE 2 Compilation
cmake --build build
if errorlevel 1 goto error

echo STAGE 3 Execution
if exist "build\src\task_01.exe" (
    "build\src\task_01.exe"
) else if exist "build\task_01.exe" (
    "build\task_01.exe"
) else (
    echo WARNING Executable not found in standard paths
)

echo STAGE 4 Plotting
py src\plot.py
if errorlevel 1 goto error

echo SUCCESS Build completed successfully
pause
goto end

:error
echo ERROR Build failed
pause
exit /b 1

:end
endlocal
```

После сборки исполняемый файл запускается автоматически либо вручную из каталога `build/src`.

### Демонстрация исполняемого файла task_01.exe:

![](work.png)

При запуске исполняемого файла мы получаем результат `results.csv`, после чего автоматически запускается `src/plot.py` для визуализации ответа:

### Пример графика переходных процессов
![](plot_task01.png)

## Вывод

Программа:
- моделирует все три системы варианта;
- симулирует отклик на все три типа входных воздействий (ступенчатое, импульсное, гармоническое);
- позволяет задать количество шагов `n`;
- выводит результаты в табличном виде;
- сохраняет результаты в `results.csv`;
- визуализирует графики через Python/Matplotlib.