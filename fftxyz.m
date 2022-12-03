prompt = 'Choose a file to perform an FFT on: ';
disp(prompt)

mat = uigetfile('.txt');
raw = readtable(mat);

x = table2array(raw(:,1));   %   Store 1st column
y = table2array(raw(:,2));   %   Store 2nd column
z = table2array(raw(:,3));   %   Store 3rd column
l = length(x);  %   Determine length of array


accel = zeros(l,1);  %   Create a zeros vector for storing acceleration mag

dt = 0.052;  %   Sampling period
fs = 1/dt;  %   Sampling Frequency
t = dt*l; % Total length of time? 
increment = t/l;
xaxis = increment:increment:t;
figure('Name','X, Y, and Z Signals');
plot(xaxis, x)
hold on
plot(xaxis, y)
plot(xaxis, z)
hold off
xlabel('Time Domain (s)')
ylabel('|X,Y,Z(amplitudes)|')


fftx = fft(x);  %   FF transform on x vector
ffty = fft(y);  %   FF transform on y vector
fftz = fft(z);  %   FF transform on z vector

P2x = abs(fftx/l);
P2y = abs(ffty/l);
P2z = abs(fftz/l);

P1x = P2x(1:l/2+1);
P1y = P2y(1:l/2+1);
P1z = P2z(1:l/2+1);

P1x(2:end-1) = 2*P1x(2:end-1);
P1y(2:end-1) = 2*P1y(2:end-1);
P1z(2:end-1) = 2*P1z(2:end-1);

f = fs*(0:(l/2))/l; %   Frequency domain

figure('Name','FFT Signal of 3 amplitudes');
plot(f,P1x)
hold on
plot(f,P1y)
plot(f,P1z)
hold off
title('FFT of X Y and Z at 50ms')
xlabel('Frequency (Hz)')
ylabel('|P1x,y,z(f)|')
