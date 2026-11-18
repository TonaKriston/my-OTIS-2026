# -*- coding: utf-8 -*-
import os
import csv
import matplotlib.pyplot as plt

CSV_FILENAME = "results.csv"

# Динамический поиск results.csv в возможных путях запуска
possible_paths = [
    CSV_FILENAME,
    os.path.join("..", CSV_FILENAME),
    os.path.join("build", CSV_FILENAME),
    os.path.join("build", "src", CSV_FILENAME),
    os.path.join(os.path.dirname(__file__), "..", CSV_FILENAME),
    os.path.join(os.path.dirname(__file__), "..", "build", "src", CSV_FILENAME)
]

csv_file = None
for path in possible_paths:
    if os.path.exists(path):
        csv_file = path
        break

if not csv_file:
    raise FileNotFoundError(f"Cannot find {CSV_FILENAME} in any standard location.")

doc_dir = os.path.join(os.path.dirname(__file__), "..", "doc")
os.makedirs(doc_dir, exist_ok=True)

steps = []
inputs_u = []
outputs_y = []
model_title = "Selected Model"
signal_title = "Selected Signal"

with open(csv_file, 'r', encoding='utf-8') as f:
    reader = csv.DictReader(f)
    for r in reader:
        steps.append(int(r['step']))
        inputs_u.append(float(r['u']))
        outputs_y.append(float(r['y']))
        if 'model_name' in r and r['model_name']:
            model_title = r['model_name']
        if 'signal' in r and r['signal']:
            signal_title = r['signal']

# Единый график: входной сигнал и отклик системы
plt.figure(figsize=(9, 5))
plt.plot(steps, inputs_u, 'r--', label=f"Input Signal u ({signal_title})", linewidth=1.5)
plt.plot(steps, outputs_y, 'b-o', markersize=4, label=f"Output State y ({model_title})", linewidth=2)

plt.title(f"Dynamic Response: {model_title} | Input: {signal_title}", fontsize=12)
plt.xlabel("Step (tau)", fontsize=10)
plt.ylabel("Value", fontsize=10)
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()
plt.tight_layout()

save_path = os.path.join(doc_dir, "plot_task01.png")
plt.savefig(save_path, dpi=300)
plt.close()

# Безопасное открытие файла только в интерактивном окружении Windows
if hasattr(os, "startfile"):
    try:
        os.startfile(os.path.abspath(save_path))
    except Exception:
        pass