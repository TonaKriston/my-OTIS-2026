import csv
import matplotlib.pyplot as plt

def to_float(s):
    return float(s.replace(",", "."))

def read_csv(filename):
    t, u, y = [], [], []
    with open(filename, "r", encoding="utf-8") as file:
        reader = csv.reader(file, delimiter=",") 
        next(reader)                       
        for row in reader:
            if not row:                   
                continue
            t.append(int(row[0]))
            u.append(to_float(row[1]))
            y.append(to_float(row[2]))
    return t, u, y

models = ["Model1_7", "Model2_9", "Model3_1"]

for model in models:
    t1, u1, y1 = read_csv(model + "_step.csv")
    t2, u2, y2 = read_csv(model + "_impulse.csv")
    t3, u3, y3 = read_csv(model + "_harmonic.csv")

    plt.figure(figsize=(8, 5))

    plt.plot(t1, y1, "b-o", markersize=3, label="y(t) — ступенчатое")
    plt.plot(t2, y2, "g-^", markersize=3, label="y(t) — импульсное")
    plt.plot(t3, y3, "r--", markersize=3, label="y(t) — гармоническое")
    plt.plot(t1, u1, "k:", markersize=2, label="u(t) — вход")

    plt.xlabel("t (номер шага)")
    plt.ylabel("значение")
    plt.title("График моделирования " + model + ", вариант 7")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    
    plt.savefig("graph_" + model + ".png", dpi=150)     

plt.show()