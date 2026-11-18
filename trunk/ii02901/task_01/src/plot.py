import pandas as pd
import matplotlib.pyplot as plt

FILE_PATH = 'results.csv' 

df = pd.read_csv(FILE_PATH, sep=';')

input_name = df['Input'].iloc[0]

plt.figure(figsize=(10, 6))

plt.plot(df['tau'], df['u(tau)'], marker='o', linestyle='-', label='u(tau) (Вход)', color='blue')

plt.plot(df['tau'], df['y(tau)'], marker='s', linestyle='-', label='y(tau) (Выход)', color='red')

plt.title(f'Реакция системы при воздействии: {input_name}', fontsize=14, pad=10)
plt.xlabel('tau (время)', fontsize=12)
plt.ylabel('Значение', fontsize=12)

plt.grid(True, linestyle='--', alpha=0.6)
plt.legend(loc='upper left')

plt.tight_layout()
plt.show()