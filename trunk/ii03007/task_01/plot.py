import csv
import matplotlib.pyplot as plt

def to_float(s):
    return float(s.replace(",", "."))

t = []
u = []
y = []

with open("result.csv","r", encoding="utf-8") as f:
    rows = csv.reader(f, delimiter=";")
    next(rows)
    for line in rows:
        if not line:
            continue
        t.append(int(line[0]))
        u.append(to_float(line[1]))
        y.append(to_float(line[2]))

plt.figure(figsize=(9, 5))
plt.plot(t, y, "b-o", markersize=4, label="y(t) — реакция модели")
plt.plot(t, u, "r--", label="u(t) — входной сигнал")
plt.xlabel("t (шаг)")
plt.ylabel("величина")
plt.title("Симуляция task_01, вариант 23")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("simulation.png", dpi=150)
plt.show()
