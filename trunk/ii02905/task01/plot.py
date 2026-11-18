import csv
import matplotlib.pyplot as plt
import os


files = [
    ("model15_constant.csv", "Model 1.5 - constant"),
    ("model15_impulse.csv", "Model 1.5 - impulse"),
    ("model15_harmonic.csv", "Model 1.5 - harmonic"),

    ("model27_constant.csv", "Model 2.7 - constant"),
    ("model27_impulse.csv", "Model 2.7 - impulse"),
    ("model27_harmonic.csv", "Model 2.7 - harmonic"),

    ("model39_constant.csv", "Model 3.9 - constant"),
    ("model39_impulse.csv", "Model 3.9 - impulse"),
    ("model39_harmonic.csv", "Model 3.9 - harmonic")
]


folder = os.path.dirname(os.path.abspath(__file__))

print("Папка plot.py:")
print(folder)
print()


for filename, title in files:

    filepath = os.path.join(folder, filename)

    print("Файл:", filename)

    if not os.path.exists(filepath):
        print("  ФАЙЛ НЕ НАЙДЕН")
        print("  Ищем здесь:", filepath)
        print()
        continue

    print("  Файл найден!")

    x = []
    y = []

    try:
        with open(filepath, "r", encoding="utf-8-sig", newline="") as file:

            reader = csv.DictReader(file)

            print("  Столбцы:", reader.fieldnames)

            for row in reader:
                x.append(float(row["tau"]))
                y.append(float(row["y"]))

        print("  Точек:", len(x))

        plt.figure()
        plt.plot(x, y)

        plt.title(title)
        plt.xlabel("tau")
        plt.ylabel("y")
        plt.grid(True)
        plt.tight_layout()

        plt.show()

    except Exception as e:
        print("  ОШИБКА:", e)

    print()



