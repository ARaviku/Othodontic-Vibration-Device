prompt = 'Choose a file to perform an FFT on: ';
disp(prompt)

mat = uigetfile('.txt');
raw = readtable(mat);

x = table2array(raw(:,1));   %   Store 1st column
y = table2array(raw(:,2));   %   Store 2nd column
z = table2array(raw(:,3));   %   Store 3rd column
l = length(x);  %   Determine length of array


accel = zeros(l,1);  %   Create a zeros vector for storing acceleration mag

for i = 1:l % Retrieve a value from each axis, get magnitude
    x1 = x(i,:);
    y1 = y(i,:);
    z1 = z(i,:);
    accel(i,1) = sqrt((x1^2)+(y1^2)+(z1^2));
end

dt = 0.05;  %   Sampling period
t = dt*l; % Total length of time? 
fs = 1/dt;  %   Sampling Frequency

fftaccel = fft(accel);  %   FF transform on magnitude vector

P2 = abs(fftaccel/l);
P1 = P2(1:l/2+1);
P1(2:end-1) = 2*P1(2:end-1);

f = fs*(0:(l/2))/l; %   Frequency domain

figure('Name','FFT Signal of magnitude');
plot(f,P1) 
title('FFT of total Magnitude at 50ms')
xlabel('Frequency (Hz)')
ylabel('|P1(f)|')