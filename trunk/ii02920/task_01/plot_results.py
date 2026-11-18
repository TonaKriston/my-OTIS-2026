#!/usr/bin/env python3
"""Построение графика из CSV: python3 plot_results.py result.csv plot.png "Model 1.10"."""

import csv
import math
import sys
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


def read_csv(path):
    """Читает столбцы tau, u_tau, y_tau, созданные программой C++."""
    steps, inputs, outputs = [], [], []
    with Path(path).open(encoding="utf-8-sig", newline="") as source:
        reader = csv.DictReader(source)
        required = {"tau", "u_tau", "y_tau"}
        if reader.fieldnames is None or not required.issubset(reader.fieldnames):
            raise ValueError("CSV must contain tau, u_tau and y_tau columns")
        for row in reader:
            step = int(row["tau"])
            u = float(row["u_tau"])
            y = float(row["y_tau"])
            if not math.isfinite(u) or not math.isfinite(y):
                raise ValueError("CSV contains a non-finite value")
            steps.append(step)
            inputs.append(u)
            outputs.append(y)
    if not steps:
        raise ValueError("CSV contains no data rows")
    return steps, inputs, outputs


def main():
    if len(sys.argv) != 4:
        print('Usage: python3 plot_results.py <csv_file> <output_png> "<title>"')
        return 1
    try:
        steps, inputs, outputs = read_csv(sys.argv[1])
        destination = Path(sys.argv[2])
        destination.parent.mkdir(parents=True, exist_ok=True)

        fig, ax = plt.subplots(figsize=(10, 6))
        ax.plot(steps, outputs, color="#1262b5", linewidth=2,
                marker=".", label="Выход yτ")
        ax.plot(steps, inputs, color="#c45a13", linestyle="--",
                linewidth=1.5, label="Вход uτ")
        ax.set(xlabel="Номер шага τ", ylabel="Значение", title=sys.argv[3])
        ax.grid(alpha=0.3)
        ax.axhline(0, color="#64748b", linewidth=0.7)
        ax.legend()
        fig.tight_layout()
        fig.savefig(destination, dpi=160)
        plt.close(fig)
        print(f"[OK] Saved plot: {destination}")
        return 0
    except (OSError, ValueError, KeyError) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
