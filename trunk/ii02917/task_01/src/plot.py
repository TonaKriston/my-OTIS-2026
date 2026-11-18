import csv
import matplotlib.pyplot as plt

def load_simulation_data(filepath):
    dataset = {}
    active_section = None
    
    with open(filepath, mode='r', encoding='utf-8') as f:
        reader = csv.reader(f)
        for chunk in reader:
            if not chunk:
                continue
            first_element = chunk[0].strip()
            if first_element.startswith('#'):
                active_section = first_element.lstrip('# ').strip()
                dataset[active_section] = {'steps': [], 'input_signals': [], 'output_signals': []}
                continue
            if 'step' in first_element or 'tau' in first_element:
                continue
            if active_section and len(chunk) == 3:
                try:
                    dataset[active_section]['steps'].append(int(chunk[0]))
                    dataset[active_section]['input_signals'].append(float(chunk[1]))
                    dataset[active_section]['output_signals'].append(float(chunk[2]))
                except ValueError:
                    pass
    return dataset

def generate_visual_reports(filepath):
    records = load_simulation_data(filepath)
    canvas, subplots = plt.subplots(nrows=3, ncols=1, figsize=(10, 11))
    palette = ['#1f77b4', '#2ca02c', '#d62728']
    
    for position, (title, metrics) in enumerate(records.items()):
        if position >= len(subplots):
            break
        current_axis = subplots[position]
        current_axis.plot(metrics['steps'], metrics['output_signals'], color=palette[position], linestyle='-', linewidth=2.5, label='Выходной сигнал y(t)')
        current_axis.plot(metrics['steps'], metrics['input_signals'], color='#7f7f7f', linestyle=':', linewidth=1.2, label='Входное воздействие u(t)')
        current_axis.set_title(title, fontsize=11, fontweight='medium', pad=8)
        current_axis.legend(loc='upper right', frameon=True, facecolor='#f9f9f9')
        current_axis.grid(visible=True, linestyle='--', alpha=0.5)
        current_axis.set_xlabel('Шаги времени (итерации)')
        current_axis.set_ylabel('Амплитуда сигналов')
        
    canvas.suptitle('Результаты моделирования: Вариант 14 (Системы 1.4, 2.8, 3.2)', fontsize=13, fontweight='bold', y=0.98)
    plt.tight_layout()
    canvas.savefig('simulation_plots_v14.png', dpi=200, bbox_inches='tight')
    print("Графический отчет успешно сохранен в файл simulation_plots_v14.png")
    plt.show()

if __name__ == '__main__':
    generate_visual_reports('results.csv')
