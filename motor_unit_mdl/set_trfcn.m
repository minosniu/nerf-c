function set_trfcn(fname)
open_system('motor_unit');
%open_system('test/twitch');
set_param('motor_unit','Lock','off')

%add_block('built-in/MATLAB Fcn',['test/',fname], 'matlabfcn',fname,'name',fname)
% NORMAL big motor unit
set_param('motor_unit/NORMAL/big motor unit/Fuglevand twitch', 'sys', 'zpk([0], [A_twitch A_twitch], P*e*T_twitch*A/tau_twitch, T_twitch)');
set_param('motor_unit/NORMAL/big motor unit/active state', 'sys', '0*zpk([0], A_rise, mag1, T) + zpk([0], exp(-T/0.04),  mag2*8, T)');

% NORMAL med motor unit
set_param('motor_unit/NORMAL/med motor unit/Fuglevand twitch', 'sys', 'zpk([0], [A_twitch A_twitch], P*e*T_twitch*A/tau_twitch, T_twitch)');
set_param('motor_unit/NORMAL/med motor unit/active state', 'sys', '0*zpk([0], A_rise, mag1, T) + zpk([0], exp(-T/0.06),  mag2*3.5, T)')

% NORMAL small motor unit
set_param('motor_unit/NORMAL/small motor unit/Fuglevand twitch', 'sys', 'zpk([0], [A_twitch A_twitch], P*e*T_twitch*A/tau_twitch, T_twitch)');
set_param('motor_unit/NORMAL/small motor unit/active state', 'sys', '0*zpk([0], A_rise, mag1, T) + zpk([0], exp(-T/0.09),  mag2, T)');

% Spastic big motor unit
set_param('motor_unit/Spastic/big motor unit/Fuglevand twitch', 'sys', 'zpk([0], [A_twitch A_twitch], P*e*T_twitch*A/tau_twitch, T_twitch)');
set_param('motor_unit/Spastic/big motor unit/active state', 'sys', '0*zpk([0], A_rise, mag1, T) + zpk([0], exp(-T/0.04),  mag2*8, T)');

% Spastic med motor unit
set_param('motor_unit/Spastic/med motor unit/Fuglevand twitch', 'sys', 'zpk([0], [A_twitch A_twitch], P*e*T_twitch*A/tau_twitch, T_twitch)');
set_param('motor_unit/Spastic/med motor unit/active state', 'sys', '0*zpk([0], A_rise, mag1, T) + zpk([0], exp(-T/0.06),  mag2*3.5, T)');

% Spastic small motor unit
set_param('motor_unit/Spastic/small motor unit/Fuglevand twitch', 'sys', 'zpk([0], [A_twitch A_twitch], P*e*T_twitch*A/tau_twitch, T_twitch)');
set_param('motor_unit/Spastic/small motor unit/active state', 'sys', '0*zpk([0], A_rise, mag1, T) + zpk([0], exp(-T/0.09),  mag2, T)');



save_system('motor_unit');

