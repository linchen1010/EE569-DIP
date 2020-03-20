clc
clear all

TrainImages = cell(12, 1);
   for i = 1 : 12
      textureImages{i} = ReadRaw([int2str(i),'.raw'],128,128,1);
      %textureImages{i} = Shrink(textureImages{i});
   end




function Img = ReadRaw(filename, height, width, channel)
    f = fopen(filename, 'rb');
    if (f == -1)
        printError('Error');
    end
    [array, length] = fread(f, inf, 'uint8');
    fclose(f);
    if (length ~= height * width * channel) 
        printError('size is not matched');
    end
    Img = zeros(height, width, channel, 'uint8');
    for i = 1 : height
        for j = 1 : width
            for k = 1 : channel
                Img(i, j, k) = array(channel * (width * (i - 1) + (j - 1)) + k);
            end
        end
    end
    % imshow(image,[0 255]);
end