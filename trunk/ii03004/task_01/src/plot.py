"""Строит графики по CSV-файлам, которые сохраняет программа.

Для каждой модели создаётся один рисунок с тремя графиками
(ступенчатое, импульсное, гармоническое воздействие) и сохраняется в PNG.
"""
import csv
import os
import sys

import matplotlib.pyplot as plt

MODELS = {
    "model_1_4": "Model 1.4 — линейная",
    "model_2_6": "Model 2.6 — нелинейная",
    "model_3_8": "Model 3.8 — дифф. уравнение (Эйлер)",
}
SIGNALS = {
    "step": "Ступенчатое",
    "impulse": "Импульсное",
    "harmonic": "Гармоническое",
}


def read_csv(path):
    tau, u, y = [], [], []
    with open(path, encoding="utf-8") as f:
        for row in csv.DictReader(f):
            tau.append(int(row["tau"]))
            u.append(float(row["u"]))
            y.append(float(row["y"]))
    return tau, u, y


def plot_single(path):
    """Один график для одного CSV-файла (режим 2: одна модель, один сигнал)."""
    name = os.path.basename(path)[:-len(".csv")]
    model_id = name[:len("model_x_x")]
    signal_id = name[len("model_x_x_"):]
    tau, u, y = read_csv(path)

    fig, ax = plt.subplots(figsize=(9, 4))
    ax.plot(tau, y, marker="o", markersize=3, label="y(τ)")
    ax.step(tau, u, where="post", linestyle="--", alpha=0.6, label="u(τ)")
    ax.set_title(MODELS.get(model_id, model_id) + " — "
                 + SIGNALS.get(signal_id, signal_id).lower() + " воздействие")
    ax.set_xlabel("τ")
    ax.grid(True, alpha=0.3)
    ax.legend()
    fig.tight_layout()

    out = name + ".png"
    fig.savefig(out, dpi=120)
    print("График сохранён:", out)
    plt.show()


def main():
    # Если передали имя CSV-файла - строим только его график.
    if len(sys.argv) > 1:
        if os.path.exists(sys.argv[1]):
            plot_single(sys.argv[1])
        else:
            print("Файл не найден:", sys.argv[1])
        return

    created = []
    for model_id, model_title in MODELS.items():
        files = {s: f"{model_id}_{s}.csv" for s in SIGNALS}
        files = {s: p for s, p in files.items() if os.path.exists(p)}
        if not files:
            continue

        fig, axes = plt.subplots(len(files), 1, figsize=(9, 3 * len(files)),
                                 squeeze=False)
        for ax, (signal_id, path) in zip(axes[:, 0], files.items()):
            tau, u, y = read_csv(path)
            ax.plot(tau, y, marker="o", markersize=3, label="y(τ)")
            ax.step(tau, u, where="post", linestyle="--", alpha=0.6,
                    label="u(τ)")
            ax.set_title(SIGNALS[signal_id] + " воздействие")
            ax.set_xlabel("τ")
            ax.grid(True, alpha=0.3)
            ax.legend()

        fig.suptitle(model_title)
        fig.tight_layout()
        out = f"{model_id}.png"
        fig.savefig(out, dpi=120)
        created.append(out)

    if created:
        print("Графики сохранены:", ", ".join(created))
        plt.show()
    else:
        print("CSV-файлы не найдены, сначала запустите программу.")


if __name__ == "__main__":
    main()
