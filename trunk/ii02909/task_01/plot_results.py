#!/usr/bin/env python3
"""
Скрипт для построения графиков по результатам симуляции.
Читает result.csv (генерируется C++ программой) и сохраняет PNG.

Использование:
    python plot_results.py <csv_file> <output_png> "<title>"

Пример:
    python plot_results.py result.csv screenshots/07_plot_model_1_4.png "Model 1.4 — Step, a=0.9"
"""

import sys
import csv
from pathlib import Path
import matplotlib
matplotlib.use('Agg')  # без графического окна, только сохранение в файл
import matplotlib.pyplot as plt


def read_csv(path):
    """Читает result.csv с колонками tau, u_tau, y_tau."""
    # Валидация пути: защита от path traversal
    p = Path(path).resolve()
    cwd = Path.cwd().resolve()
    if not p.is_relative_to(cwd):
        raise ValueError(f"Path outside working directory: {path}")
    if not p.is_file():
        raise FileNotFoundError(f"CSV file not found: {path}")

    taus, us, ys = [], [], []
    with p.open('r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            taus.append(int(row['tau']))
            us.append(float(row['u_tau']))
            ys.append(float(row['y_tau']))
    return taus, us, ys


def main():
    if len(sys.argv) < 4:
        print("Usage: python plot_results.py <csv_file> <output_png> \"<title>\"")
        sys.exit(1)

    csv_file = sys.argv[1]
    out_png  = sys.argv[2]
    title    = sys.argv[3]

    taus, us, ys = read_csv(csv_file)

    # --- График ---
    fig, ax = plt.subplots(figsize=(10, 6))

    # Основной график y(tau)
    ax.plot(taus, ys, marker='o', linewidth=2, markersize=6,
            color='#1f77b4', label='y (выход)')

    # Вход u(tau) как вспомогательный пунктир
    ax.plot(taus, us, linestyle='--', linewidth=1.5,
            color='#ff7f0e', alpha=0.7, label='u (вход)')

    # Оформление
    ax.set_xlabel('Шаг симуляции τ', fontsize=12)
    ax.set_ylabel('Значение', fontsize=12)
    ax.set_title(title, fontsize=13, fontweight='bold')
    ax.grid(True, linestyle=':', alpha=0.6)
    ax.axhline(y=0, color='black', linewidth=0.8, alpha=0.4)
    ax.legend(loc='best', fontsize=11)

    # Ограничение по X, чтобы не было пустых полей
    ax.set_xlim(min(taus) - 0.5, max(taus) + 0.5)

    plt.tight_layout()
    plt.savefig(out_png, dpi=150, bbox_inches='tight')
    plt.close(fig)

    print(f"[OK] Saved plot: {out_png}")


if __name__ == "__main__":
    main()