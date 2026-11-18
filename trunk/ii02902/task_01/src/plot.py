
import sys
sys.path.append(r"D:\lib\site-packages")
import os
import pandas as pd
import matplotlib.pyplot as plt

# Путь к файлу с результатами (лежит в корне проекта)
csv_path = "results.csv"
if not os.path.exists(csv_path):
    print("Файл results.csv не найден! Сначала запустите программу симуляции.")
    exit(1)

# Загружаем данные
df = pd.read_csv(csv_path)

plt.figure(figsize=(10, 5))

# График выходного сигнала y от времени (step)
plt.subplot(1, 2, 1)
plt.plot(df['step'], df['y'], marker='o', color='b', label='Output y')
plt.title(f"Model: {df['model_name'].iloc[0]} ({df['signal'].iloc[0]})+")
plt.xlabel("Шаг (step)")
plt.ylabel("Выход y")
plt.grid(True)
plt.legend()

# График входного сигнала u от времени
plt.subplot(1, 2, 2)
plt.plot(df['step'], df['u'], marker='x', color='r', label='Input u')
plt.title("Входной сигнал")
plt.xlabel("Шаг (step)")
plt.ylabel("Вход u")
plt.grid(True)
plt.legend()

plt.tight_layout()

# Сохраняем графики в папку doc (если она есть) и в корень проекта для отчёта
os.makedirs("doc", exist_ok=True)
plt.savefig("doc/plot.png")
plt.savefig("plot_task01.png")

# Показываем окно с графиком
plt.show()