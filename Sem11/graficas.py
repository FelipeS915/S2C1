import pandas as pd
import matplotlib.pyplot as plt

# Leer los resultados del archivo
results = pd.read_csv("resultados.txt", header=None)

# Obtener los datos de las columnas 1 y 3
x = results.iloc[:, 0]
y_speedup = results.iloc[:, 2] / results.iloc[0, 2]
y_efficiency = y_speedup / x

# Graficar los datos
plt.figure()
plt.plot(x, y_speedup, 'o-')
plt.grid()
plt.xlabel("Número de procesos")
plt.ylabel("Speedup")
plt.title("Speedup vs Número de procesos")
plt.savefig("speedup.pdf")  # Guardar la gráfica como archivo PDF

plt.figure()
plt.plot(x, y_efficiency, 'o-')
plt.grid()
plt.xlabel("Número de procesos")
plt.ylabel("Parallel Eficiency")
plt.title("Parallel Eficiency vs Número de procesos")
plt.savefig("speedup.pdf")  # Guardar la gráfica como archivo PDF


plt.show()
