import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("resultados2.txt", header=None)

x = data.iloc[:,1]
y = data.iloc[:,3]

plt.loglog(x,y,'o-')
plt.grid()
plt.xlabel("N")
plt.ylabel("Error relativo")
plt.title("N vs Error relativo en escala log-log")
plt.savefig("error.pdf")
plt.show()

