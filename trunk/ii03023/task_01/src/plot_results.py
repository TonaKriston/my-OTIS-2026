import os
import matplotlib.pyplot as plt
import pandas as pd


def plot_simulation():
  filename = "simulation_results.csv"
  if not os.path.exists(filename):
    if os.path.exists(f"../{filename}"):
      filename = f"../{filename}"
    elif os.path.exists(f"../../{filename}"):
      filename = f"../../{filename}"

  if not os.path.exists(filename):
    print(
        f"[ERROR] Файл {filename} не найден! Сначала запустите C++ программу."
    )
    return

  try:
    df = pd.read_csv(filename)

    if (
        "Step" not in df.columns
        or "Input" not in df.columns
        or "Value" not in df.columns
    ):
      print(f"[ERROR] CSV файл имеет некорректный формат: {df.columns}")
      return

    steps = df["Step"]
    inputs = df["Input"]
    values = df["Value"]

    plt.figure(figsize=(10, 5))

    # Красный график входного сигнала
    plt.plot(
        steps,
        inputs,
        linestyle="--",
        color="red",
        linewidth=1.5,
        label="Входной сигнал (Input)",
    )
    # Синий график выхода модели
    plt.plot(
        steps,
        values,
        marker="o",
        markersize=3,
        linestyle="-",
        color="blue",
        linewidth=2,
        label="Выход модели (Value)",
    )

    plt.title(
        "Результаты моделирования и входной сигнал (OTIS)",
        fontsize=12,
        fontweight="bold",
    )
    plt.xlabel("Шаги (Step)", fontsize=10)
    plt.ylabel("Значение", fontsize=10)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.legend()
    plt.tight_layout()

    plt.savefig("simulation_plot.png", dpi=300)
    print(
        "[INFO] График сохранен как simulation_plot.png и выводится на экран..."
    )
    plt.show()

  except Exception as e:
    print(f"[ERROR] Не удалось построить график: {e}")


if __name__ == "__main__":
  plot_simulation()