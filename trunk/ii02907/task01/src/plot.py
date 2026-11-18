import csv
import os
import matplotlib.pyplot as plt

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FILES = [
    ("model15_constant.csv", "Model 1.5 — constant input"),
    ("model15_impulse.csv", "Model 1.5 — impulse input"),
    ("model15_harmonic.csv", "Model 1.5 — harmonic input"),
    ("model27_constant.csv", "Model 2.7 — constant input"),
    ("model27_impulse.csv", "Model 2.7 — impulse input"),
    ("model27_harmonic.csv", "Model 2.7 — harmonic input"),
    ("model39_constant.csv", "Model 3.9 — constant input"),
    ("model39_impulse.csv", "Model 3.9 — impulse input"),
    ("model39_harmonic.csv", "Model 3.9 — harmonic input"),
]

for filename, title in FILES:
    path = os.path.join(HERE, filename)
    if not os.path.exists(path):
        print(f"Missing {filename}; run task_01.exe first.")
        continue
    with open(path, newline="", encoding="utf-8-sig") as f:
        rows = list(csv.DictReader(f))
    t = [float(row["tau"]) for row in rows]
    y = [float(row["y"]) for row in rows]
    plt.figure()
    plt.plot(t, y)
    plt.title(title)
    plt.xlabel("Time")
    plt.ylabel("y(t)")
    plt.grid(True)
    plt.tight_layout()
    output_dir = os.path.join(HERE, "doc")
    os.makedirs(output_dir, exist_ok=True)
    output_path = os.path.join(output_dir, filename.replace(".csv", ".png"))
    plt.savefig(output_path, dpi=150)
    print(f"Saved {output_path}")
plt.show()
