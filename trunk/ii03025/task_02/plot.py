import csv

import matplotlib.pyplot as plt

tau = []
y = []

with open("build/simulation_results.csv", "r", encoding="utf-8") as file:
    reader = csv.DictReader(file, delimiter=";")

    for row in reader:
        tau.append(int(row["Step"]))
        y.append(float(row["Y"]))

plt.figure()
plt.plot(tau, y, label="y(t)")
plt.xlabel("τ")
plt.ylabel("Y")
plt.title("Готовый график моделирования")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("result.png", dpi=150)
plt.show()
