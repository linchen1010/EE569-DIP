clear all
clc

txtfile = fopen('Histogram.txt','r');
formatSpec = '%d';
fprintf(txtfile,'%d\n');
size = [1,256];
Blue = fscanf(txtfile,formatSpec,size);
Green = fscanf(txtfile,formatSpec,size);
Red = fscanf(txtfile,formatSpec,size);


X = 1:256;

figure(1);
bar(X,Blue);
title("Blue Histogram");
xlabel("Intensity Value");
ylabel("Number of Pixel");
grid on

figure(2);
bar(X,Green);
title("Green Histogram");
xlabel("Intensity Value");
ylabel("Number of Pixel");
grid on

figure(3);
bar(X,Red);
title("Red Histogram");
xlabel("Intensity Value");
ylabel("Number of Pixel");
grid on

fclose(txtfile);

