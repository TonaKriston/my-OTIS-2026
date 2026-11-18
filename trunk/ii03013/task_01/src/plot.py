
import csv
import os
import sys
from pathlib import Path

import matplotlib.pyplot as plt


# Папка проекта
project_dir = Path(__file__).resolve().parent.parent

# Папка, из которой разрешено читать CSV
allowed_dir = (project_dir / "build" / "Release").resolve()


# Проверяем, передан ли CSV-файл
if len(sys.argv) < 2:
    print("Ошибка: CSV-файл не указан.")
    sys.exit(1)

# Получаем только имя файла, не принимая путь от пользователя
csv_name = Path(sys.argv[1]).name

if not csv_name.lower().endswith(".csv"):
    print("Ошибка: необходимо указать CSV-файл.")
    sys.exit(1)

csv_file = (allowed_dir / csv_name).resolve()

# Проверяем, что файл находится в разрешённой папке
if csv_file.parent != allowed_dir:
    print("Ошибка: недопустимый путь к CSV-файлу.")
    sys.exit(1)

if not csv_file.is_file():
    print(f"Ошибка: CSV-файл не найден: {csv_file}")
    sys.exit(1)


# Папка для графика
doc_dir = project_dir / "doc"
doc_dir.mkdir(parents=True, exist_ok=True)

# Файл с графиком
output_file = doc_dir / "graph.png"


# Читаем CSV
x = []
models = {}

try:
    with open(csv_file, "r", encoding="utf-8", newline="") as file:
        reader = csv.DictReader(file)

        columns = reader.fieldnames

        if not columns or len(columns) < 2:
            print("Ошибка: CSV должен содержать X и хотя бы один столбец модели.")
            sys.exit(1)

        # Первый столбец — X
        x_column = columns[0]

        # Остальные столбцы — модели
        for column in columns[1:]:
            models[column] = []

        for row in reader:
            x.append(float(row[x_column]))

            for column in models:
                models[column].append(float(row[column]))

except (OSError, csv.Error, ValueError, TypeError) as error:
    print(f"Ошибка чтения CSV: {error}")
    sys.exit(1)


# Строим график
plt.figure(figsize=(10, 6))

for name, values in models.items():
    plt.plot(x, values, marker="o", label=name)

plt.xlabel(x_column)
plt.ylabel("Значение")
plt.title("Результаты моделирования")
plt.grid(True)
plt.legend()

# Сохраняем график
plt.savefig(output_file, dpi=300, bbox_inches="tight")

print(f"График сохранён: {output_file}")

plt.show()
