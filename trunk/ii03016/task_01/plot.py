import csv
import os
import matplotlib.pyplot as plt

CSV_FILE = "results.csv"
OUT_DIR = "doc/graphs"

os.makedirs(OUT_DIR, exist_ok=True)

blocks = []           # список блоков
current = None        # текущий блок

with open(CSV_FILE, "r", encoding="utf-8") as f:
    for line in f:
        line = line.strip()
        if not line:
            continue

        # Начало нового блока: "Model,<имя>,Signal,<тип>"
        if line.startswith("Model,"):
            if current is not None:
                blocks.append(current)
            parts = line.split(",")
            current = {
                "model": parts[1],
                "signal": parts[3],
                "t": [],
                "u": [],
                "y": []
            }
      
        elif line.startswith("t,u(t),y(t)"):
            continue
        # Строки с данными: "t,u,y"
        elif current is not None:
            parts = line.split(",")
            if len(parts) == 3:
                current["t"].append(float(parts[0]))
                current["u"].append(float(parts[1]))
                current["y"].append(float(parts[2]))


if current is not None:
    blocks.append(current)

for i, b in enumerate(blocks):
    # Формируем безопасное имя файла
    model_short = b["model"].split("(")[0].strip().replace(" ", "_")
    signal = b["signal"]
    filename = f"plot_{model_short}_{signal}.png"
    filepath = os.path.join(OUT_DIR, filename)

    # Создаём график
    plt.figure(figsize=(10, 6))
    plt.plot(b["t"], b["y"], marker="o", linewidth=2, label="y(t)")
    plt.plot(b["t"], b["u"], marker="s", linestyle="--", alpha=0.6, label="u(t)")

    plt.title(f'{b["model"]} | Signal: {signal}')
    plt.xlabel("t (шаг симуляции)")
    plt.ylabel("Значение")
    plt.grid(True, alpha=0.3)
    plt.legend()

    # Сохраняем
    plt.tight_layout()
    plt.savefig(filepath, dpi=120)
    plt.close()

    print(f"Сохранён график: {filepath}")

print(f"\nВсего графиков: {len(blocks)}")
print(f"Все сохранены в папку: {OUT_DIR}")