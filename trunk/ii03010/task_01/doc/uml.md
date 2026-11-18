# UML-диаграмма классов

```mermaid
classDiagram
    class InputSignal {
        <<abstract>>
        +valueAt(tau) double
    }
    class ConstantSignal {
        -value_ double
        +valueAt(tau) double
    }
    class ImpulseSignal {
        +valueAt(tau) double
    }
    class HarmonicSignal {
        +valueAt(tau) double
    }

    class Model {
        <<abstract>>
        +nextValue(tau, outputHistory, inputHistory) double
    }
    class DelayedInputModel {
        -a_ double
        -b_ double
        -delay_ size_t
        +nextValue(tau, outputHistory, inputHistory) double
    }
    class RelayModel {
        -a_ double
        -b_ double
        -epsilon_ double
        +nextValue(tau, outputHistory, inputHistory) double
    }
    class HarmonicDifferentialModel {
        -b_ double
        -timeStep_ double
        +nextValue(tau, outputHistory, inputHistory) double
    }
    class SimulationRow {
        +step size_t
        +input double
        +currentOutput double
        +nextOutput double
    }

    InputSignal <|-- ConstantSignal
    InputSignal <|-- ImpulseSignal
    InputSignal <|-- HarmonicSignal
    Model <|-- DelayedInputModel
    Model <|-- RelayModel
    Model <|-- HarmonicDifferentialModel
```

Функция `simulate` получает модель и входной сигнал через ссылки на базовые классы и формирует последовательность `SimulationRow`. Функции `printTable` и `writeSvgPlot` выводят эту последовательность в консоль и в SVG соответственно. Это свободные функции, поэтому на диаграмме нет отдельного класса симулятора.
