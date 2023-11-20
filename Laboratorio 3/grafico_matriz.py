# Este código de Python se utiliza para crear los gráficos de Speedup de ambas soluciones

import matplotlib.pyplot as plt

# Datos para el eje y
y_data = [1.726401644, 2.362574453, 2.750048452, 2.709555934, 2.613687431, 2.587461859, 2.519443866, 2.434720152, 2.362154171, 2.286254163, 2.217824659, 2.154420555, 2.091687417, 2.032499023, 1.979548626]

# Datos para el eje x
x_data = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]

# Crear el gráfico
plt.plot(x_data, y_data, marker='o', linestyle='-', color='red')

# Añadir etiquetas y título
plt.xlabel('N° Tasks')
plt.ylabel('Speedup ')
plt.title('Comportamiento Speedup - Matriz Privada')

# Mostrar el gráfico
plt.show()


# Nuevos datos para el eje y
y_data_nuevo = [1.707391991, 2.347111756, 2.870922573, 2.888087881, 2.944047333, 2.969741731, 3.001395138, 2.991685936, 2.986784492, 2.999274308, 3.00494495, 2.99923769, 2.998855536, 2.993011812, 2.991617624]

# Crear el gráfico
plt.plot(x_data, y_data_nuevo, marker='o', linestyle='-', color='red')

# Añadir etiquetas y título
plt.xlabel('N°Tasks')
plt.ylabel('Speedup')
plt.title('Comportamiento Speedup - Matriz Compartida')

# Mostrar el gráfico
plt.show()
