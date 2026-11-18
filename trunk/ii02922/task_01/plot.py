import matplotlib.pyplot as plt
import csv
import os
import sys

# Включаем поддержку кириллицы в графиках
plt.rcParams['font.sans-serif'] = ['Arial', 'DejaVu Sans'] 
plt.rcParams['axes.unicode_minus'] = False

def plot_graph(filename):
    t = []
    u = []
    y = []

    # Читаем CSV файл
    try:
        with open(filename, mode='r', encoding='utf-8') as file:
            reader = csv.DictReader(file)
            for row in reader:
                t.append(float(row['t']))
                u.append(float(row['u']))
                y.append(float(row['y']))
    except FileNotFoundError:
        print(f"Ошибка: Файл {filename} не найден!")
        return

    # Создаём окно графика
    plt.figure(figsize=(10, 6))

    # Рисуем линию выходного сигнала y(t)
    plt.plot(t, y, label='Выходной сигнал y(t)', color='red', linewidth=2.5, marker='o', markersize=4)
    
    # Рисуем линию входного сигнала u(t) (пунктиром, чтобы было видно воздействие)
    plt.plot(t, u, label='Входной сигнал u(t)', color='blue', linestyle='--', alpha=0.7)

    # Настройки графика
    plt.title(f'Переходный процесс: {filename}', fontsize=14, fontweight='bold')
    plt.xlabel('Время (t / шаги)', fontsize=12)
    plt.ylabel('Значение', fontsize=12)
    plt.grid(True, linestyle=':', alpha=0.7)
    plt.legend(fontsize=11)
    
    # Показываем график
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # Ищем все .csv файлы в текущей папке
    csv_files = [f for f in os.listdir('.') if f.endswith('.csv')]
    
    if not csv_files:
        print("В этой папке нет .csv файлов. Сначала запустите C++ программу.")
    else:
        print("\n=== Найденные файлы с результатами ===")
        for i, f in enumerate(csv_files):
            print(f"  {i + 1}. {f}")
        
        try:
            choice = int(input("\nВведите номер файла для построения графика: "))
            if 1 <= choice <= len(csv_files):
                plot_graph(csv_files[choice - 1])
            else:
                print("Неверный номер.")
        except ValueError:
            print("Нужно ввести число.")