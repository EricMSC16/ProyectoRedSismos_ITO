clc;clear all;close all
file1=fopen("datos_transmisorp1.csv")
mycsvdata=csvread(file1);
s1xm1=[mycsvdata(:,2)]; % X1

file1=fopen("datos_transmisorp2.csv")
mycsvdata=csvread(file1);
s1xm2=[mycsvdata(:,2)]; % X1

figure(1)
plot(s1xm1(1:80,1),"linewidth", 2)
hold on
plot(s1xm2(1:80,1),"linewidth", 2)

%%%% giro
file1=fopen("datos_transmisorp1.csv")
title('Acelerometro X', "fontsize", 16)
mycsvdata=csvread(file1);
gxm1=[mycsvdata(:,5)]; % X1

file1=fopen("datos_transmisorp2.csv")
mycsvdata=csvread(file1);
gxm2=[mycsvdata(:,5)]; % X1

figure(2)
plot(gxm1(5:80,1)-1,"linewidth", 2)
hold on
plot(gxm2(5:80,1)+80,"linewidth", 2)
title('Giroscopio X', "fontsize", 16)