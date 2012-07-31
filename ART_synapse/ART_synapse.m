% ART(arbitrary rate transmission) synapse

% input: spikes from sensory neuron
% output: spikes to motoneuron
% synapse that enables arbitrary rate transmission required. 
% SN -> (synapse) -> MN



spike_train = [0,1, 0, 1, 0, 0, 1, 0, 0, 0];
postsynaptic_spike_train = [0, 0,0,0, 0, 0, 0, 0, 0, 0];


impulse = 10240;  %initial (=first_pass)

current = 0;
%impulse_decay = 


for k=1:length(spike_train)
    current = 0.875* current + impulse

    if (postsynaptic_spike_train(k)) 
        delta_w = 1024;
    else 
        delta_w = 0;
    end

    if (spike_train(k)) 
        delta_w_ltd = -512;
    else 
        delta_w_ltd = 0;
    end
    % loop 128
    impulse = impulse + delta_w + delta_w_ltd; % - impulse_decay;
   %disp(spike_train(k));
end

