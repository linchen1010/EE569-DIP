clear all
clc

txtfile = fopen('Histogram.txt','r');
formatSpec = '%d';
fprintf(txtfile,'%d\n');
size = [1,256];
Blue = fscanf(txtfile,formatSpec,size);
Green = fscanf(txtfile,formatSpec,size);
Red = fscanf(txtfile,formatSpec,size);
fclose(txtfile);

% cumulative Histogram for 3 channels
txtfile_cum = fopen('CumHistogram.txt','r');
formatSpec = '%d';
fprintf(txtfile,'%d\n');
BlueCum = fscanf(txtfile,formatSpec,size);
GreenCum = fscanf(txtfile,formatSpec,size);
RedCum = fscanf(txtfile,formatSpec,size);
fclose(txtfile_cum)

% total pixel for each channel is 225600
total = 224000;
BluePro = Blue / total;
GreenPro = Green / total;
RedPro = Red / total;

BlueCDF = cumsum(BluePro);
GreenCDF = cumsum(GreenPro);
RedCDF = cumsum(RedPro);

X = 1:256;

figure(1);
bar(X,Blue);
title("Blue Histogram");
xlabel("Intensity Value");
ylabel("Number of Pixel");
xlim([0 255]);
grid on


figure(2);
bar(X,Green);
title("Green Histogram");
xlabel("Intensity Value");
ylabel("Number of Pixel");
xlim([0 255]);
grid on

figure(3);
bar(X,Red);
title("Red Histogram");
xlabel("Intensity Value");
ylabel("Number of Pixel");
xlim([0 255]);
grid on

figure(4);
subplot(131);
plot(X,BlueCDF);
title("Blue");
xlabel("Intensity Value");
ylabel("Number of Pixel");
xlim([0 255]);
grid on


figure(4);
subplot(132);
plot(X,GreenCDF);
title("Green");
xlabel("Intensity Value");
ylabel("Number of Pixel");
xlim([0 255]);
grid on

figure(4);
subplot(133);
plot(X,RedCDF);
title("Red");
xlabel("Intensity Value");
ylabel("Number of Pixel");
xlim([0 255]);
grid on




