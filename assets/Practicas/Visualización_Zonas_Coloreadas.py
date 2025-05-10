import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import ListedColormap, BoundaryNorm

# Leer el archivo CSV
data = pd.read_csv("red_colores.csv")

# Definir un mapa de colores numéricos
color_map = {"red": 2, "blue": 1, "yellow": 0}

# Crear una matriz 2D para almacenar los colores
L = int(data['x'].max() + 1)  # Suponiendo que la red es cuadrada
color_matrix = np.zeros((L, L))

# Asignar valores de color a cada posición
for _, row in data.iterrows():
    color_matrix[int(row['y']), int(row['x'])] = color_map[row['color']]

# Configurar un mapa de colores para la visualización
# Crear una paleta personalizada que incluya azul, amarillo y rojo
cmap = ListedColormap(["yellow", "blue", "red"])
bounds = [0, 1, 2, 3]  # Límites de los valores en el color_map
norm = BoundaryNorm(bounds, cmap.N)  # Normalización discreta

# Crear el gráfico de zonas coloreadas
plt.figure(figsize=(10, 8))
plt.pcolormesh(color_matrix, cmap=cmap, norm=norm, shading='flat')  # Aquí se usa shading='flat'
plt.colorbar(label="Sitios", ticks=[0.5, 1.5, 2.5], format=lambda x, _: ["Pequeños", "Medianos", "Grandes"][int(x)])
plt.title("Distribución de los Sitios de la Red")
plt.xlabel("X")
plt.ylabel("Y")

# Guardar como PNG
plt.savefig("zonas_coloreadas_red.png", dpi=300)  # Cambié a dpi=300 para mejorar la resolución
print("Imagen guardada como 'zonas_coloreadas_red.png'")

