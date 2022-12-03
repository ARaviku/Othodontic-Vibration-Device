%% vibration.m
% 10/5/2018
% This code analyzes the freqeuncy content of vibration for each axis
%% import data
filename = uigetfile('.txt');
data = importdata(filename);
%% convert raw values to accelerations (g)
x = data(:,2)/16384;
y = data(:,3)/16384;
z = data(:,4)/16384;
%% remove offset
x = x-mean(x);
y = y-mean(y);
z = z-mean(z);
%% sampling rate (s)
time = data(:,1);
time_diff = diff(time);
avg_dt = mean(time_diff);
dt = avg_dt*1e-3;
%% time vector (s)
t = dt*(0:length(data)-1);
%% time domain plots
figure(1)
hold on
plot(t,x)
plot(t,y)
plot(t,z)
xlabel('time (s)')
ylabel('acceleration (g)')
legend('x','y','z')
%% blackman window
b = blackman(20);
b1 = b(1:10);
b2 = b(11:end);
x(1:10) = x(1:10).*b1;
y(1:10) = y(1:10).*b1;
z(1:10) = z(1:10).*b1;
x(end-9:end) = x(end-9:end).*b2;
y(end-9:end) = y(end-9:end).*b2;
z(end-9:end) = z(end-9:end).*b2;
%% FFT
xFFT = fft(x,2048);
yFFT = fft(y,2048);
zFFT = fft(z,2048);
%% frequency vector
fs = 1/dt;
f = (0:length(xFFT)-1).*fs/length(xFFT);
%% FFT magnitude plot
figure(2)
hold on
half = 1:length(xFFT)/2;
plot(f(half),2*abs(xFFT(half))/length(data))
plot(f(half),2*abs(yFFT(half))/length(data))
plot(f(half),2*abs(zFFT(half))/length(data))
xlabel('frequency (Hz)')
ylabel('|FFT|')
legend('x','y','z')



