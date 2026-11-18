import os
import pandas as pd
import matplotlib.pyplot as plt

# Путь к файлу с результатами
csv_path = os.path.join("doc", "results.csv")

if not os.path.exists(csv_path):
    print(f"Ошибка: Файл {csv_path} не найден.")
    exit(1)

# Чтение данных из CSV
data = pd.read_csv(csv_path)

# Построение графика
plt.figure(figsize=(10, 6))
plt.plot(data['tau'], data['u'], label='u(tau)', color='orange', linestyle='--', marker='o')
plt.plot(data['tau'], data['y'], label='y(tau)', color='blue', linewidth=2, marker='s')

plt.title('Переходный процесс модели')
plt.xlabel('tau')
plt.ylabel('Амплитуда')
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()

# Сохранение изображения
os.makedirs("doc", exist_ok=True)
plot_path = os.path.join("doc", "plot.png")
plt.savefig(plot_path, dpi=300)
print(f"График сохранен в {plot_path}")

# Автоматический вызов (показ) графика
plt.show()