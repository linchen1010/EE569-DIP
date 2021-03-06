clear all
clc

TrainImages = ReadAllImg();

% TrainImgReflect{1} = reflectImage(TrainImages{1});
% TrainSubMean{1} = SubtractLocalMean(TrainImgReflect{1},10);

% variable initialization %
laws = LawsFilter();
TrainImgReflect = cell (1,36);
temp = cell (36,1);
TrainImgSubtractMean = cell (1,36);
FilterImg = cell(36,15);
tempVector = zeros([25,1],'double');
FilterImgEnergy = cell(36,25);

for i = 1 : 36
    TrainImgReflect{i} = reflectImg(TrainImages{i},4);
    temp{i} = conv(TrainImgReflect{i});
    TrainImgSubtractMean{i} = subtractMean(double(TrainImages{i}),temp{i});
    for j = 1 : 25
       FilterImg{i,j} = ApplyLawsFilter(reflectImg(TrainImgSubtractMean{i},2),laws{j});
       FilterImgEnergy{i,j} = PosEnergy(FilterImg{i,j});
       tempVector(j,1) = floor(mean2(FilterImgEnergy{i,j}));
    end
       FeatureVector_25D{i,1} = tempVector;
       FeatureVector_15D{i,1} = ReduceTo15D(FeatureVector_25D{i,1});
       F{i,1} = Standardize(FeatureVector_15D{i,1});
end

F_norm = [;F{1}';F{2}';F{3}';F{4}' ;F{5}' ;F{6}' ;F{7}' ;F{8}' ;F{9}' ;F{10}' ;F{11}' ;F{12}' ;F{13}' ;F{14}' ;F{15}' ;F{16}' ;F{17}' ;F{18}'; 
          F{19}'; F{20}' ;F{21}' ;F{22}' ;F{23}' ;F{24}' ;F{25}' ;F{26}' ;F{27}' ;F{28}' ;F{29}' ;F{30}' ;F{31}' ;F{32}' ;F{33}' ;F{34}' ;F{35}' ;F{36}'];

[coeff,score] = pca(F_norm,'NumComponents',3);

cluster = kmeans(score,4);
      
% F_norm = [;F{1}' ;F{2}' ;F{3}' ;F{4}' ;F{5}' ;F{6}' ;F{7}' ;F{8}' ;F{9}' ;F{10}' ;F{11}' ;F{12};;F{13};;F{14};;F{15};;F{16};;F{17};;F{18};
%           ;F{19};;F{21};;F{22};;F{23};;F{24};;F{25};;F{26};;F{27};;F{28};;F{29};;F{30};;F{31};;F{32};;F{33};;F{34};;F{35};;F{36}];





clear TrainImgReflect TrainImgSubtractMean temp tempVector

function TrainImages = ReadAllImg()
    TrainImages = cell(36, 1);
    disp("Retrieve the images data  .. ")
    for i = 1 : 9
        TrainImages{i} = ReadRaw(['blanket',int2str(i),'.raw'],128,128,1);
    end
    for i = 10 : 18
        TrainImages{i} = ReadRaw(['brick',int2str(i-9),'.raw'],128,128,1);
    end
    for i = 19 : 27
        TrainImages{i} = ReadRaw(['grass',int2str(i-18),'.raw'],128,128,1);
    end
    for i = 28 : 36
        TrainImages{i} = ReadRaw(['rice',int2str(i-27),'.raw'],128,128,1);
    end
    disp("Successfully retrieving the images data!!!")
end

% Height = 128, Width = 128, channel = 1 (gray-scale)
function Img = ReadRaw(filename, height, width, channel)
    f = fopen(filename, 'rb');
    if (f == -1)
        disp('Error Input File');
    end
    [temp] = fread(f, inf, 'uint8');
    fclose(f);
    Img = zeros(height, width, channel, 'uint8');
    for i = 1 : height
        for j = 1 : width
            for k = 1 : channel
                Img(i, j, k) = temp(channel * (width * (i - 1) + (j - 1)) + k);
            end
        end
    end 
end

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
        result{i,j} = transpose(kernal(i,:))*kernal(j,:);
    end
end
%     result{1} = cell{1,1}; %LL
%     result{2} = cell{2,2}; %EE
%     result{3} = cell{3,3}; %SS
%     result{4} = cell{4,4}; %WW
%     result{5} = cell{5,5}; %RR
%     result{6} = 0.5 * (cell{1,2} + cell{2,1}); %LE/EL
%     result{7} = 0.5 * (cell{1,3} + cell{3,1}); %LS/SL
%     result{8} = 0.5 * (cell{1,4} + cell{4,1}); %LW/WL
%     result{9} = 0.5 * (cell{1,5} + cell{5,1}); %LR/RL
%     result{10} = 0.5 * (cell{2,3} + cell{3,2}); %WR/RW
%     result{11} = 0.5 * (cell{2,4} + cell{4,2}); %ES/SE
%     result{12} = 0.5 * (cell{2,5} + cell{5,2}); %EW/WE
%     result{13} = 0.5 * (cell{3,4} + cell{4,3}); %ER/RE
%     result{14} = 0.5 * (cell{3,5} + cell{5,3}); %SW/WS
%     result{15} = 0.5 * (cell{4,5} + cell{5,4}); %SR/RS
end

function result = conv (image)
    result = zeros(128,'double');
    for i=5:size(image,1)-4
        for j=5:size(image,2)-4
            image(i,j)=convlocal(image,i,j);
        end
    end
    for i=5:size(image,1)-4
        for j=5:size(image,2)-4
            result(i-4,j-4)=image(i,j);
        end
    end
    
end

function result = convlocal(image,x,y)
    meanKernal = fspecial('average', 9);
    rate = meanKernal(1,1);
    pixel = 0; 
    for i = -4 : 4
        for j = -4 : 4
           pixel = pixel + rate*image(x+i,y+j) ;
        end
    end
    result  =  pixel; 
end

function result = subtractMean(image, LocalMeanImage)
    for i = 1 : size(image, 1)
        for j = 1 : size(image, 2)
            result(i, j) = image(i, j) - LocalMeanImage(i, j);
        end
    end
end

function result = reflectImg(image,boarder)
    result = padarray(image,[boarder boarder],'symmetric');
end

function result = ApplyLawsFilter(image, filter)
    temp = conv2(image,filter,'same');
    result = zeros(128,'double');
    for i = 1 : 128
        for j = 1 : 128
            result(i,j) = temp(i+2,j+2);
        end
    end
end

function result = PosEnergy(image)
    temp = zeros(size(image),'double');
    for i = 1 : size(image,1)
        for j = 1 : size(image,2)
            temp(i,j) = abs(image(i,j));
        end
    end
    result = temp;
end

function result = ReduceTo15D(vector)
    ReducedVector = zeros(15,1);
    
    ReducedVector(1) = vector(1);
    ReducedVector(2) = vector(7);
    ReducedVector(3) = vector(13);
    ReducedVector(4) = vector(19);
    ReducedVector(5) = vector(25);
    ReducedVector(6) = (vector(2)+vector(6))/2;
    ReducedVector(7) = (vector(3)+vector(11))/2;
    ReducedVector(8) = (vector(8)+vector(12))/2;
    ReducedVector(9) = (vector(4)+vector(16))/2;
    ReducedVector(10) = (vector(9)+vector(17))/2;
    ReducedVector(11) = (vector(21)+vector(5))/2;
    ReducedVector(12) = (vector(14)+vector(18))/2;
    ReducedVector(13) = (vector(10)+vector(22))/2;
    ReducedVector(14) = (vector(23)+vector(15))/2;
    ReducedVector(15) = (vector(24)+vector(20))/2;
    result = ReducedVector; 
end

function result = Standardize(vector)
    result = zeros([15 1],'double');
    result = (vector(:,1)-mean(vector(:,1)))/std(vector(:,1));
end



