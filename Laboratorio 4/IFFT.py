import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Laboratorio 4 - Sistemas Distribuidos y Paralelos
# Autor: John Serrano Carrasco

# Leer el archivo CSV
df = pd.read_csv('results.csv', header=None, names=['index', 'real', 'imag'])

# Obtener el tamaño del arreglo N
N = 2048

# Inicializar arreglos
arreglo_real = np.zeros((N, N), dtype=np.complex64)
arreglo_imag = np.zeros((N, N), dtype=np.complex64)

# Asignar valores a los arreglos
for i, row in df.iterrows():
    index_2d = np.unravel_index(int(row['index'] - 1), (N, N))
    arreglo_real[index_2d] = np.complex64(row['real'])
    arreglo_imag[index_2d] = np.complex64(row['imag'])

# Crear el arreglo complejo
grid = arreglo_real + 1j * arreglo_imag

# Calcular la transformada de Fourier inversa (dirty image)
image = np.fft.ifftshift(np.fft.ifft2(np.fft.fftshift(grid)))


fig , ax = plt.subplots(nrows=1, ncols=2, figsize=(15,10))
ax[0].imshow(np.abs(grid), origin = "lower", cmap="twilight_shifted")
ax[1].imshow(image.real,origin ="lower", cmap="magma")
fig.suptitle('Results (Hltau)', fontsize=17)
ax[0].set_title("Gridded data image")
ax[1].set_title("Dirty image")
plt.show()