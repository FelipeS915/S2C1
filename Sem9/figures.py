import pandas as pd
import matplotlib.pyplot as plt
import os
import psutil

physical_cores = psutil.cpu_count(logical=False)
total_cores = os.cpu_count()

# Leer los resultados del archivo
results = pd.read_csv("results.txt")

# Graficar speedup
plt.figure()
for policy in results["policy"].unique():
    policy_results = results[results["policy"] == policy]
    plt.plot(policy_results["threads"].to_numpy(), policy_results["speedup"].to_numpy(), 'o-', label=f"Política {policy}")

plt.grid()
plt.xlabel("Número de threads")
plt.ylabel("Speedup")
plt.title(f"Speedup vs Threads (Cores físicos: {physical_cores}, Total: {total_cores})")
plt.legend()
plt.savefig("speedup.pdf")  # Guardar la gráfica de speedup

# Graficar eficiencia paralela
plt.figure()
for policy in results["policy"].unique():
    policy_results = results[results["policy"] == policy]
    plt.plot(policy_results["threads"].to_numpy(), policy_results["parallel_efficiency"].to_numpy(), 'o-', label=f"Política {policy}")
plt.grid()
plt.xlabel("Número de threads")
plt.ylabel("Eficiencia paralela")
plt.title(f"Eficiencia paralela vs Threads (Cores físicos: {physical_cores}, Total: {total_cores})")
plt.legend()
plt.savefig("parallel_efficiency.pdf")  # Guardar la gráfica de eficiencia paralela

plt.show()
