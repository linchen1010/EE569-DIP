clear all
clc

function result = LawsFilter()
kernal = [
            [1 4 6 4 1]   % L5
            [-1 -2 0 2 1] % E5
            [-1 0 2 0 -1] % S5
            [-1 2 0 -2 1] % W5
            [1 -4 6 -4 1] % R5
         ];     
for i = 1 : 5
    for j = 1 : 5
        cell{i,j} = transpose(kernal(i,:))*kernal(j,:);
    end
end

L5 = [1 4 6 4 1];
E5 = [-1 -2 0 2 1];

L5E5 = L5 .* transpose(E5);
E5L5 = E5 .* transpose(L5);
Avg = (L5E5 + E5L5) / 2;


result{1} = cell{1,1}; %LL
result{2} = cell{2,2}; %EE
result{3} = cell{3,3}; %SS
result{4} = cell{4,4}; %WW
result{5} = cell{5,5}; %RR
result{6} = 0.5 * (cell{1,2} + cell{2,1}); %LE/EL
result{7} = 0.5 * (cell{1,3} + cell{3,1}); %LS/SL
result{8} = 0.5 * (cell{1,4} + cell{4,1}); %LW/WL
result{9} = 0.5 * (cell{1,5} + cell{5,1}); %LR/RL
result{10} = 0.5 * (cell{2,3} + cell{3,2}); %WR/RW
result{11} = 0.5 * (cell{2,4} + cell{4,2}); %ES/SE
result{12} = 0.5 * (cell{2,5} + cell{5,2}); %EW/WE
result{13} = 0.5 * (cell{3,4} + cell{4,3}); %ER/RE
result{14} = 0.5 * (cell{3,5} + cell{5,3}); %SW/WS
result{15} = 0.5 * (cell{4,5} + cell{5,4}); %SR/RS
end




