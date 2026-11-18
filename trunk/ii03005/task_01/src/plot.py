import csv
from pathlib import Path

import matplotlib.pyplot as plt


DATA_DIRECTORY = Path(__file__).resolve().parent
CSV_PATH = DATA_DIRECTORY / "results.csv"
GRAPH_PATH = DATA_DIRECTORY / "graph.png"


def plot_graph():
    t = []
    u = []
    y = []

    with CSV_PATH.open("r", encoding="utf-8") as file:
        reader = csv.reader(file, delimiter=";")
        next(reader)

        for row in reader:
            if not row:
                continue

            t.append(float(row[0]))
            u.append(float(row[1]))
            y.append(float(row[2]))

    _, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 7))

    ax1.plot(t, y, "b-", label="y(t) — выход")
    ax1.set_ylabel("y")
    ax1.set_title("Выход модели")
    ax1.grid(True)
    ax1.legend()

    ax2.plot(t, u, "r-", label="u(t) — вход")
    ax2.set_xlabel("t")
    ax2.set_ylabel("u")
    ax2.set_title("Вход модели")
    ax2.grid(True)
    ax2.legend()

    plt.tight_layout()
    plt.savefig(GRAPH_PATH, dpi=150)
    plt.show()


if __name__ == "__main__":
    plot_graph()
