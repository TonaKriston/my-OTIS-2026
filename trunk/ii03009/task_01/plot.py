import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

if len(sys.argv) < 3:
    print("Ошибка: нужно передать префикс и тип сигнала")
    print("Пример: python plot.py out_linear 0")
    sys.exit(1)

prefix = sys.argv[1]

try:
    signal_type = int(sys.argv[2])
except ValueError:
    print("Ошибка: тип сигнала должен быть числом 0/1/2")
    sys.exit(1)


titles = {
    'out_linear':    'Линейная модель 1.9',
    'out_nonlinear': 'Нелинейная модель 2.9',
    'out_diffeq':    'Диф. уравнение 3.9',
}
title_base = titles.get(prefix, 'Неизвестная модель')


signals = {
    0: 'Ступенчатый',
    1: 'Импульсный',
    2: 'Гармонический (синус)',
}
signal_name = signals.get(signal_type, 'Неизвестный сигнал')

filename = f'{prefix}_result.csv'
if not os.path.exists(filename):
    print(f"Ошибка: файл {filename} не найден")
    sys.exit(1)

df = pd.read_csv(filename)

# Один график
fig, ax = plt.subplots(figsize=(10, 5))
fig.suptitle(f'{title_base} — {signal_name}', fontsize=14)

ax.plot(df['step'], df['y'], label='y(t) — выход', color='blue',  linewidth=2)
ax.plot(df['step'], df['u'], label='u(t) — вход',  color='orange', linestyle='--')

ax.set_xlabel('Шаг (t)')
ax.set_ylabel('Значение')
ax.legend()
ax.grid(True)

plt.tight_layout()


os.makedirs('../doc/Screenshots', exist_ok=True)
save_path = f'../doc/Screenshots/{prefix}_plot.png'
plt.savefig(save_path)
print(f"График сохранён: {save_path}")
plt.show()