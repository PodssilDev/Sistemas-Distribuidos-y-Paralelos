% Lectura de archivos de matriz compartida
s1 = fread(fopen("datosgrideadosr.raw", "r"), "double");
s2 = fread(fopen("datosgrideadosi.raw", "r"), "double");
% Lectura de archivos de matriz privada
s1_mp = fread(fopen("datosgrideadosr_mp.raw", "r"), "double");
s2_mp = fread(fopen("datosgrideadosi_mp.raw", "r"), "double");

% Reshape de los datos reales e imaginarios
re = reshape(s1, 2048, 2048);
re_mp = reshape(s1_mp, 2048, 2048);
im = reshape(s2, 2048, 2048);
im_mp = reshape(s2_mp, 2048, 2048);

% Creación del plano complejo
v = complex(re, im);
v_mp = complex(re_mp, im_mp);

% Calculo de la transformada de Fourier e Inversa de Fourier
I = fftshift(ifft2(v));
I_mp = fftshift(ifft2(v_mp));

% Creación de figura para mostrar resultados
figure;

% Imagen de Matriz Compartida
subplot(1,2,1);
imagesc(abs(I));
colormap('hot');
title('Imagen resultante Matriz Compartida');

% Imagen de Matriz Privada
subplot(1,2,2);
imagesc(abs(I_mp));
colormap('hot');
title('Imagen resultante Matriz Privada');