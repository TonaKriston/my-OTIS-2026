
import csv
import os
import matplotlib.pyplot as plt

BASE_DIR = os.path.dirname(
    os.path.dirname(os.path.abspath(__file__))
)

graphs = {
    "1": ("model11_step.csv", "Model 1.1 — Step"),
    "2": ("model11_impulse.csv", "Model 1.1 — Impulse"),
    "3": ("model11_harmonic.csv", "Model 1.1 — Harmonic"),
    "4": ("model25_step.csv", "Model 2.5 — Step"),
    "5": ("model25_impulse.csv", "Model 2.5 — Impulse"),
    "6": ("model25_harmonic.csv", "Model 2.5 — Harmonic"),
    "7": ("model39_step.csv", "Model 3.9 — Step"),
    "8": ("model39_impulse.csv", "Model 3.9 — Impulse"),
    "9": ("model39_harmonic.csv", "Model 3.9 — Harmonic")
}


def read_csv(filename):
    t = []
    u = []
    y = []

    with open(
        filename,
        "r",
        encoding="utf-8-sig",
        newline=""
    ) as file:

        reader = csv.DictReader(file)

        for row in reader:
            t.append(float(row["t"]))
            u.append(float(row["u"]))
            y.append(float(row["y"]))

    return t, u, y


def show_graph(number):

    filename, title = graphs[number]
    path = os.path.join(BASE_DIR, filename)

    if not os.path.exists(path):
        print("ОШИБКА: файл не найден!")
        print(path)
        return

    t, u, y = read_csv(path)

    rows_visible = 12
    total_rows = len(t)
    first_row = 0

    fig = plt.figure(figsize=(12, 9))

    fig.suptitle(
        title,
        fontsize=17,
        fontweight="bold"
    )

    ax_graph = fig.add_axes([
        0.10,
        0.53,
        0.80,
        0.38
    ])

    ax_graph.plot(t, y)

    ax_graph.set_title(
        "График выходного сигнала",
        fontsize=13,
        fontweight="bold"
    )

    ax_graph.set_xlabel("t")
    ax_graph.set_ylabel("y")
    ax_graph.grid(True)

    ax_table = fig.add_axes([
        0.25,
        0.08,
        0.55,
        0.42
    ])

    ax_table.axis("off")

    def update_table():

        ax_table.clear()
        ax_table.axis("off")

        last_row = min(
            first_row + rows_visible,
            total_rows
        )

        table_data = [
            ["n", "t", "u", "y"]
        ]

        for i in range(
            first_row,
            last_row
        ):

            table_data.append([
                str(i),
                f"{t[i]:.4f}",
                f"{u[i]:.4f}",
                f"{y[i]:.4f}"
            ])

        table = ax_table.table(
            cellText=table_data,
            loc="center",
            cellLoc="center",
            colWidths=[
                0.12,
                0.22,
                0.22,
                0.32
            ]
        )

        table.auto_set_font_size(False)
        table.set_fontsize(9)

        table.scale(
            1,
            1.35
        )

        for column in range(4):

            cell = table[
                (0, column)
            ]

            cell.set_text_props(
                fontweight="bold"
            )

            cell.set_linewidth(1.5)


    def on_scroll(event):

        nonlocal first_row

        if event.inaxes != ax_table:
            return

        scroll_step = 3

        if event.button == "up":
            first_row -= scroll_step

        elif event.button == "down":
            first_row += scroll_step

        if first_row < 0:
            first_row = 0

        max_first_row = max(
            0,
            total_rows - rows_visible
        )

        if first_row > max_first_row:
            first_row = max_first_row

        update_table()

        fig.canvas.draw_idle()


    update_table()

    fig.canvas.mpl_connect(
        "scroll_event",
        on_scroll
    )

    plt.show()


def main():

    while True:

        print()
        print("Выберите график:")
        print()

        print("1. Model 1.1 — Step")
        print("2. Model 1.1 — Impulse")
        print("3. Model 1.1 — Harmonic")

        print()

        print("4. Model 2.5 — Step")
        print("5. Model 2.5 — Impulse")
        print("6. Model 2.5 — Harmonic")

        print()

        print("7. Model 3.9 — Step")
        print("8. Model 3.9 — Impulse")
        print("9. Model 3.9 — Harmonic")

        print()
        print("0. Выход")
        print()

        choice = input("Ваш выбор: ")

        if choice == "0":
            break

        if choice in graphs:
            show_graph(choice)
        else:
            print("Неверный выбор.")


if __name__ == "__main__":
    main()

