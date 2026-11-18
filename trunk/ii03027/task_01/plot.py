import pandas as pd
import matplotlib.pyplot as plt
import os
import tkinter as tk
from tkinter import ttk

csv_path = 'build/simulation_results.csv'
if not os.path.exists(csv_path):
    csv_path = 'simulation_results.csv'

try:
    df = pd.read_csv(csv_path)
except FileNotFoundError:
    print("Ошибка: Файл simulation_results.csv не найден.")
    exit()

def update_table(event=None):
    selected_signal = combo_signal.get()
    
    for i in tree.get_children():
        tree.delete(i)
    
    if selected_signal == "All_signals":
        filtered_df = df
    else:
        filtered_df = df[df['Signal'] == selected_signal]
    
    for index, (_, row) in enumerate(filtered_df[['Model', 'Signal', 'Step', 'u', 'y']].iterrows()):
        tag = 'evenrow' if index % 2 == 0 else 'oddrow'
        tree.insert('', 'end', values=list(row), tags=(tag,))
    
    root.title(f"Результаты симуляции | Показаны: {selected_signal}")

def show_plot():
    selected_signal = combo_signal.get()
    model_name = df['Model'].iloc[0]
    
    if selected_signal == "All_signals":
        plot_data = df
    else:
        plot_data = df[df['Signal'] == selected_signal]

    fig, ax = plt.subplots(figsize=(10, 6), num=f"График: {selected_signal}")
    fig.suptitle(f'Модель: {model_name} | Сигнал: {selected_signal}', fontsize=16)

    if selected_signal == "All_signals":
        for signal in plot_data['Signal'].unique():
            sig_data = plot_data[plot_data['Signal'] == signal]
            ax.plot(sig_data['Step'], sig_data['y'], label=signal, linewidth=2)
    else:
        ax.plot(plot_data['Step'], plot_data['y'], label=selected_signal, linewidth=2.5, color='red')

    ax.set_xlabel('Шаг симуляции (t)', fontsize=12)
    ax.set_ylabel('Выход y(t)', fontsize=12)
    ax.legend(fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.savefig(f'doc/graphs/plot_{selected_signal}.png') 
    plt.show()

root = tk.Tk()
root.title("Результаты симуляции")
root.geometry("1000x700")
root.configure(bg="#f0f0f0")

control_frame = tk.Frame(root, bg="#f0f0f0", pady=10)
control_frame.pack(fill='x', padx=10)

tk.Label(control_frame, text="Выберите сигнал:", bg="#f0f0f0", font=("Arial", 11)).pack(side='left', padx=(0, 10))

combo_signal = ttk.Combobox(control_frame, values=["All_signals", "Step", "Impulse", "Harmonic"], state="readonly", font=("Arial", 11))
combo_signal.current(0)
combo_signal.pack(side='left', padx=(0, 20))

combo_signal.bind("<<ComboboxSelected>>", update_table)

btn_show = tk.Button(control_frame, text="Показать график", command=show_plot, font=("Arial", 11, "bold"), bg="#4CAF50", fg="white", relief="flat", padx=10)
btn_show.pack(side='left')

frame = ttk.Frame(root)
frame.pack(fill='both', expand=True, padx=10, pady=10)

scrollbar = ttk.Scrollbar(frame)
scrollbar.pack(side='right', fill='y')

style = ttk.Style()
style.theme_use("clam")
style.configure("Treeview.Heading", font=('Arial', 11, 'bold'), background="#4a4a4a", foreground="white")
style.map("Treeview.Heading", background=[('active', '#3a3a3a')])
style.configure("Treeview", font=('Arial', 10), rowheight=25)

tree = ttk.Treeview(frame, yscrollcommand=scrollbar.set, style="Treeview")
tree.pack(side='left', fill='both', expand=True)
scrollbar.config(command=tree.yview)

columns = ['Model', 'Signal', 'Step', 'u', 'y']
tree['columns'] = columns
tree['show'] = 'headings'

for col in columns:
    tree.heading(col, text=col)
    tree.column(col, width=120, anchor='center')

tree.tag_configure('evenrow', background='#ffffff')
tree.tag_configure('oddrow', background='#f9f9f9')

update_table()

print("Окно открыто. Выберите сигнал в списке, чтобы обновить таблицу.")
root.mainloop()