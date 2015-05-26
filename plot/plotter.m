x = [16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37];
y = [0.002350 0.005242 0.010104 0.019701 0.039020 0.077579 0.155503 0.311102 0.620266 1.241629 2.482091 4.962573 9.930329 19.813534 39.710128 79.435027 158.890091 315.784731 633.643474 1271.370375 2543.361080 5084.677121];

plot(x,y, "marker", 'o', "color", 'r', "linestyle", ':');
xlabel("bits");
ylabel("seg");
title("Tempo de busca por tamanho da chave");
print -djpg image.jpg

semilogy(x,y, "marker", 'o', "color", 'r',"linestyle", ':');
xlabel("bits");
ylabel("seg");
title("Tempo de busca por tamanho da chave");
print -djpg image_log.jpg
