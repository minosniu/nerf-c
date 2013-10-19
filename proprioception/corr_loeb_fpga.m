%data_len = length(loeb_pps);
% corr_list = [];
% for i = 1:data_len
%     corr_list(i) = corr2(loeb_pps(i, :), fpga_pps(i, :));
% end



fpga_pps = zeros(size(fpga_pps11));
    
for i = 11:20
     fpga_pps = fpga_pps + eval(['fpga_pps', num2str(i)]);
end

loeb_pps = loeb_pps11;

corr2(loeb_pps, fpga_pps)

plot(loeb_pps, fpga_pps, '.')