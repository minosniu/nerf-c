function [A_twitch T_twitch tau_twitch P e] = twitch_filter()

T_twitch = 1/1024;
P = 1;
tau_twitch = 0.016;
e = exp(1);
A_twitch = exp(-T_twitch/tau_twitch);
%tf = zpk([0], [A A], P*e*T*A/tau, T);



