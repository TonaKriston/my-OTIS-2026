import sys
import csv
import matplotlib
matplotlib.use('Agg') 
import matplotlib.pyplot as plt
import os

def read_csv(path):
    taus, us, ys = [], [], []
    with open(path, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f, delimiter=';') 
        for row in reader:
            taus.append(int(row['Step']))
            us.append(float(row['Input u']))
            ys.append(float(row['Output y']))
    return taus, us, ys

def main():
    if len(sys.argv) < 3:
        print("Usage: python plot_results.py <csv_file> <output_png>")
        sys.exit(1)
    
    csv_file = sys.argv[1]
    out_png = sys.argv[2]
    
    try:
        taus, us, ys = read_csv(csv_file)
    except Exception as e:
        print(f"Error reading CSV: {e}")
        sys.exit(1)

    fig, ax = plt.subplots(figsize=(10, 6))
    
    ax.plot(taus, ys, marker='o', linewidth=2, markersize=6, color='#2E86AB', label='y (Output)')
    ax.plot(taus, us, linestyle='--', linewidth=1.5, color='#EE6C4D', alpha=0.7, label='u (Input)')
    
    ax.set_xlabel('Simulation step (tau)', fontsize=12)
    ax.set_ylabel('Value', fontsize=12)
    ax.set_title('Simulation Result', fontsize=13, fontweight='bold')
    ax.grid(True, linestyle=':', alpha=0.6)
    ax.legend(loc='best', fontsize=11)
    
    plt.tight_layout()
    plt.savefig(out_png, dpi=150, bbox_inches='tight')
    plt.close(fig)
    print(f"[OK] Saved plot: {out_png}")

if __name__ == "__main__":
    main()