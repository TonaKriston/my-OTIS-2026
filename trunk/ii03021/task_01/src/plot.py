"""Create plots for the most recent OTIS simulation run.

Usage: python plot.py <results_directory>
Requires matplotlib: python -m pip install matplotlib
"""
from __future__ import annotations

import csv
import sys
from pathlib import Path


def read_csv(path: Path):
    tau, values = [], []
    with path.open("r", newline="", encoding="utf-8-sig") as stream:
        reader = csv.DictReader(stream)
        for row in reader:
            try:
                tau.append(float(row["tau"]))
                values.append(float(row["y"]))
            except (KeyError, TypeError, ValueError):
                continue
    return tau, values


def main() -> int:
    results = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else Path(__file__).resolve().parent / "results"
    if not results.is_dir():
        print(f"ERROR: results directory does not exist: {results}")
        return 1

    csv_files = sorted(results.glob("*/*.csv"))
    if not csv_files:
        print("No CSV files found; nothing to plot.")
        return 0

    try:
        import matplotlib
        import matplotlib.pyplot as plt
    except ImportError:
        print("ERROR: matplotlib is not installed.")
        print("Install it with: python -m pip install matplotlib")
        return 2

    created = 0
    for csv_path in csv_files:
        tau, values = read_csv(csv_path)
        if not tau:
            print(f"Skipping empty or invalid CSV: {csv_path}")
            continue
            
        model = csv_path.parent.name
        signal = csv_path.stem
        fig, ax = plt.subplots(figsize=(10, 6))
        ax.plot(tau, values, linewidth=2, label="y(tau)")
        ax.set_title(f"{model.capitalize()} model - {signal} input")
        ax.set_xlabel("tau")
        ax.set_ylabel("y")
        ax.grid(True, alpha=0.35)
        ax.legend()
        fig.tight_layout()
        output = results / f"{model}_{signal}.png"
        fig.savefig(output, dpi=130)
        print(f"Plot saved: {output}")
        created += 1
    plt.show()
    
    print(f"Plotting complete. Created {created} plot(s).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
