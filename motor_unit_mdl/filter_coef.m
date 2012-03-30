function [num, den] = filter_coef(mag1, mag2, trise, tfall)
%#codegen

% Shadmehr muscle model gives the h(t) of active_state
% h(t) = a*exp(-t/c) + b*exp(-t/d)
% H(s) = a/(s+c) + b/(s+d)
% This func: H(s) -> H(z) 

% a = 48144*128;
% b = 45845*128;
coder.extrinsic('tf', 'c2d', 'tfdata', 'dfilt.df2sos', 'dfilt', 'df2sos','filter'); 

a = mag1;
b = mag2;
c = 1/trise;  % tf, tr in seconds. 
d = 1/tfall;
SAMPLING_RATE = 1024; % in Hz
num = [0 (a-b) (a*d - b*c)];
den = [1 (c+d) c*d];
tf_a = tf(num, den);

tf_a_digital = c2d(tf_a, 1/SAMPLING_RATE, 'zoh');
[num den] = tfdata(tf_a_digital, 'v');
str = sprintf('[%s]', num2str(num));
disp(str);

str = sprintf('[%s]', num2str(den));
disp(str);
% 
% a1=den(1)
% a2=den(2)
% a3=den(3)
% b1=num(1)
% b2=num(2)
% b3=num(3)
% 
% %hd=dfilt.df2sos(b1,a1,b2,a2,b3,a3)
% bcoeff = [b1 b2 b3]
% acoeff = [a1 a2 a3]
% 
% y=[0;0]  % type definition for output
% y = filtfilt(bcoeff, acoeff,  x) 
