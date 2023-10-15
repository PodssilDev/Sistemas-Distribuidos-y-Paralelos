f1 = fopen("datosgrideadosrm.raw", "r");
f2 = fopen("datosgrideadosim.raw", "r");

s1 = fread(f1, "double");
s2 = fread(f2, "double");

re = reshape(s1, 2048, 2048);
im = reshape(s2, 2048, 2048);

v = complex(re, im);

I =  fftshift(ifft2(v));

figure;imagesc(abs(I));
colormap('hot');
title('Imagen resultante');