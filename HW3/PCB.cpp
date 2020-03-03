//EE569 Howework Assignment #3
//Date:		February 23 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ -std=c++11 PCB.cpp -o PCB
// >> ./PCB PCB.raw PCB_result.raw
/*--------------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string>

int Bond(std::vector<unsigned char> pattern);
bool CheckCondPattern(std::vector<unsigned char> &ImgPattern, int bond); // Shrink
bool CheckCondPatternTh(std::vector<unsigned char> &ImgPattern, int bond); // Thin
bool CheckCondPatternSK(std::vector<unsigned char> &ImgPattern, int bond); // Skeleton
bool CheckUnConPattern (std::vector<unsigned char> &pattern); // Shrink / Thin
bool CheckUnConPatternSK (std::vector<unsigned char> &pattern); // Skeleton

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 1; // Greyscale
    int Width = 372;
    int Height = 239;
	int row = 0;
	int col = 0;
    int cnt = 0;

    int starCount = 0;

    // Check for proper syntax
	if (argc < 2){
		std::cout << "Syntax Error - Incorrect Parameter Usage:" << std::endl;
		std::cout << "program_name input_image.raw output_image.raw" << std::endl;
		return 0;
	}
    // Allocate image data array
	unsigned char Imagedata[Height][Width][BytesPerPixel];
    unsigned char ImagedataBinary[Height][Width][BytesPerPixel];
    unsigned char ImagedataR[Height][Width][BytesPerPixel];
    unsigned char Morph[Height][Width][BytesPerPixel];
    unsigned char tempImg[Height][Width][BytesPerPixel];
    unsigned char ImagedataWithoutCircle[Height][Width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

    for(row = 0; row < Height ; row++) {
        for(col = 0; col < Width; col++) {
            if(Imagedata[row][col][0] < 60) { // inorder to filter out really closed circle
                ImagedataBinary[row][col][0] = 0;
                ImagedataR[row][col][0] = 1;
            }
            else {
                ImagedataBinary[row][col][0] = 1;
                ImagedataR[row][col][0] = 0;
            }
        }
    }
    
    int i = 0;
    int j = 0;
    int n = 0;
    int temp = 0;
    bool flag = true;

    std::vector<unsigned char> pattern;

    while(flag) {
        cnt = 0;
    for(row = 0; row < Height; row++) {
        for(col = 1; col < Width; col++) {
            Morph[row][col][0] = 0;
            tempImg[row][col][0] = ImagedataBinary[row][col][0];
        }
    }
    for(row = 1; row < Height-1; row++) {
        for(col = 1; col < Width-1; col++) {
            if(ImagedataBinary[row][col][0] == 1) {
                for(i = -1; i < 2; i++) {
                    for(j = -1; j < 2; j++) {
                        pattern.push_back(ImagedataBinary[row+i][col+j][0]);
                    }
                }
                if(CheckCondPattern(pattern,Bond(pattern))) {
                    Morph[row][col][0] = 1;
                }
                else {
                    Morph[row][col][0] = 0;
                }
                pattern.clear();
            }
        }
    }

    for(row = 1; row < Height-1; row++) {
        for(col = 1;col < Width-1; col++) {
            pattern.clear();
            if(ImagedataBinary[row][col][0] == 1) {
                for(i = -1; i < 2; i++) {
                    for(j = -1; j < 2; j++) {
                        pattern.push_back(Morph[row+i][col+j][0]);
                    }
                }
                if(!CheckUnConPattern(pattern) && Morph[row][col][0]==1) { // when table 2 miss and M = 1
                    ImagedataBinary[row][col][0] = 0; // erase original corresponding pixel
                }
            }
        }
    }
    n++;
    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            if(tempImg[row][col][0]==ImagedataBinary[row][col][0]) {
                cnt++;
            }
        }
    }
    if(cnt == Height * Width) flag = false;
    std::cout << "1st Shrink: " << n  << " times" << std::endl; 
    }

    /* 2rd Shrink counting circle */
    n = 0;
    while(n < 200) {
        cnt = 0;
    for(row = 0; row < Height; row++) {
        for(col = 1; col < Width; col++) {
            Morph[row][col][0] = 0;
            tempImg[row][col][0] = ImagedataR[row][col][0];
        }
    }
    for(row = 1; row < Height-1; row++) {
        for(col = 1; col < Width-1; col++) {
            if(ImagedataR[row][col][0] == 1) {
                for(i = -1; i < 2; i++) {
                    for(j = -1; j < 2; j++) {
                        pattern.push_back(ImagedataR[row+i][col+j][0]);
                    }
                }
                if(CheckCondPattern(pattern,Bond(pattern))) {
                    Morph[row][col][0] = 1;
                }
                else {
                    Morph[row][col][0] = 0;
                }
                pattern.clear();
            }
        }
    }

    for(row = 1; row < Height-1; row++) {
        for(col = 1;col < Width-1; col++) {
            pattern.clear();
            if(ImagedataR[row][col][0] == 1) {
                for(i = -1; i < 2; i++) {
                    for(j = -1; j < 2; j++) {
                        pattern.push_back(Morph[row+i][col+j][0]);
                    }
                }
                if(!CheckUnConPattern(pattern) && Morph[row][col][0]==1) { // when table 2 miss and M = 1
                    ImagedataR[row][col][0] = 0; // erase original corresponding pixel
                }
            }
        }
    }
    // for(row = 0; row < Height; row++) {
    //     for(col = 0; col < Width; col++) {
    //         if(tempImg[row][col][0]==ImagedataR[row][col][0]) {
    //             cnt++;
    //         }
    //     }
    // }
    // if(cnt == Height * Width) flag = false;
    n++;
    std::cout << "2nd Shrink " << n  << " times" << std::endl; 
    }

    std::vector<unsigned char> filter;
    std::pair<int,int> circlePosition;
    int OneCount = 0;
    int CircleCount = 0;

    for(row = 1; row < Height-1; row++) {
        for(col = 1; col < Width-1; col++) {
            ImagedataWithoutCircle[row][col][0] = ImagedataR[row][col][0];
        }
    }

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            for(i = -1; i < 2; i++) { // radius of circle
                for(j = -1; j < 2; j++) {
                    if(ImagedataBinary[row+i][col+j][0]) {
                        OneCount ++;
                    }
                }
            }
            if(OneCount == 1 && ImagedataBinary[row][col][0]==1) {
                // circlePosition.first = row;
                // circlePosition.second = col;
                CircleCount ++;
                //std::cout << "RemoveCircle" << std::endl;
                for(i = -6; i < 7; i++) { // radius of circle
                    for(j = -6; j < 7; j++) {
                        ImagedataWithoutCircle[row+i][col+j][0] = 0;
                    }
                }   
            }
            OneCount = 0;
        }
    }
    /* 3rd Shrink for counting pathways */
    flag = true;
    n = 0;
    while(flag) {
        cnt = 0;
    for(row = 0; row < Height; row++) {
        for(col = 1; col < Width; col++) {
            Morph[row][col][0] = 0;
            tempImg[row][col][0] = ImagedataWithoutCircle[row][col][0];
        }
    }
    for(row = 1; row < Height-1; row++) {
        for(col = 1; col < Width-1; col++) {
            if(ImagedataWithoutCircle[row][col][0] == 1) {
                for(i = -1; i < 2; i++) {
                    for(j = -1; j < 2; j++) {
                        pattern.push_back(ImagedataWithoutCircle[row+i][col+j][0]);
                    }
                }
                if(CheckCondPattern(pattern,Bond(pattern))) {
                    Morph[row][col][0] = 1;
                }
                else {
                    Morph[row][col][0] = 0;
                }
                pattern.clear();
            }
        }
    }

    for(row = 1; row < Height-1; row++) {
        for(col = 1;col < Width-1; col++) {
            pattern.clear();
            if(ImagedataWithoutCircle[row][col][0] == 1) {
                for(i = -1; i < 2; i++) {
                    for(j = -1; j < 2; j++) {
                        pattern.push_back(Morph[row+i][col+j][0]);
                    }
                }
                if(!CheckUnConPattern(pattern) && Morph[row][col][0]==1) { // when table 2 miss and M = 1
                    ImagedataWithoutCircle[row][col][0] = 0; // erase original corresponding pixel
                }
            }
        }
    }
    n++;
    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            if(tempImg[row][col][0]==ImagedataWithoutCircle[row][col][0]) {
                cnt++;
            }
        }
    }
    if(cnt == Height * Width) flag = false;
    std::cout << "3rd Shrink: " << n  << " times" << std::endl; 
    }

    int pathCount = 0;

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col ++) {
            if(ImagedataWithoutCircle[row][col][0] == 1) {
                pathCount ++;
            }
        }
    }



    std::cout << "Circles: " << CircleCount << std::endl;
     std::cout << "Pathways: " << pathCount << std::endl;


    for(row = 0; row < Height ; row++) {
        for(col = 0; col < Width; col++) {
            if(ImagedataBinary[row][col][0] != 0) {
                ImagedataBinary[row][col][0] = 255;
            }

            if(ImagedataR[row][col][0] != 0) {
                ImagedataR[row][col][0] = 255;
            }
            if(ImagedataWithoutCircle[row][col][0]!=0) {
                ImagedataWithoutCircle[row][col][0] = 255;
            }
        }
    }


	if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataR, sizeof(unsigned char), Width * Height * BytesPerPixel, file);
	fclose(file);

	return 0;
}


/*********FUNCTION PART**************/

int Bond(std::vector<unsigned char> pattern) {
    int b = 0;
    int i = 0;
    b = pattern[0] * 1 + pattern[1] * 2 + pattern[2] * 1 + 
        pattern[3] * 2 + pattern[4] * 0 + pattern[5] * 2 + 
        pattern[6] * 1 + pattern[7] * 2 + pattern[8] * 1;
    if(b > 0 && b <= 11) {
        return b;
    }
    else
        return 0;
}

bool CheckCondPattern(std::vector<unsigned char> &ImgPattern, int bond) {
    std::vector<std::vector<unsigned char>> S1, S2, S3, TK4, STK4, ST5_1, ST5_2, ST6, STK6, STK7, STK8, STK9, STK10, K11;  
    
        switch (bond) {
            case 1: // S
                S1 = {{0,0,1,0,1,0,0,0,0},
                      {1,0,0,0,1,0,0,0,0},
                      {0,0,0,0,1,0,1,0,0},
                      {0,0,0,0,1,0,0,0,1}}; 
                if(ImgPattern == S1[0] || ImgPattern == S1[1] || ImgPattern == S1[2] || ImgPattern == S1[3]) {
                    return true;
                }
                else 
                    break;
            case 2: // S
                S2 = {{0,0,0,0,1,1,0,0,0},
                      {0,1,0,0,1,0,0,0,0},
                      {0,0,0,1,1,0,0,0,0},
                      {0,0,0,0,1,0,0,1,0}}; 
                if(ImgPattern == S2[0] || ImgPattern == S2[1] || ImgPattern == S2[2] || ImgPattern == S2[3]) {
                    return true;
                }
                else
                    break;
            case 3: //S
                S3 = {{0,0,1,0,1,1,0,0,0},
                      {0,1,1,0,1,0,0,0,0},
                      {1,1,0,0,1,0,0,0,0},
                      {1,0,0,1,1,0,0,0,0},
                      {0,0,0,1,1,0,1,0,0},
                      {0,0,0,0,1,0,1,1,0},
                      {0,0,0,0,1,0,0,1,1},
                      {0,0,0,0,1,1,0,0,1}};
                if(ImgPattern == S3[0] || ImgPattern == S3[1] || ImgPattern == S3[2] || ImgPattern == S3[3] ||
                   ImgPattern == S3[4] || ImgPattern == S3[5] || ImgPattern == S3[6] || ImgPattern == S3[7] ) {
                    return true;
                }
                else
                    break;
            case 4:
                STK4 = {{0,0,1,0,1,1,0,0,1},
                       {1,1,1,0,1,0,0,0,0},
                       {1,0,0,1,1,0,1,0,0},
                       {0,0,0,0,1,0,1,1,1}}; 
                if(ImgPattern == STK4[0] || ImgPattern == STK4[1] || ImgPattern == STK4[2] || ImgPattern == STK4[3]) {
                    return true;
                }
                else
                    break;
            case 5:
                ST5_1 = {{1,1,0,0,1,1,0,0,0},
                         {0,1,0,0,1,1,0,0,1},
                         {0,1,1,1,1,0,0,0,0},
                         {0,0,1,0,1,1,0,1,0}};

                ST5_2 = {{0,1,1,0,1,1,0,0,0},
                         {1,1,0,1,1,0,0,0,0},
                         {0,0,0,1,1,0,1,1,0},
                         {0,0,0,0,1,1,0,1,1}};
                if(ImgPattern == ST5_1[0] || ImgPattern == ST5_1[1] || ImgPattern == ST5_1[2] || ImgPattern == ST5_1[3] ||
                   ImgPattern == ST5_2[0] || ImgPattern == ST5_2[1] || ImgPattern == ST5_2[2] || ImgPattern == ST5_2[3] ) {
                    return true;
                }
                else        
                    break;
            case 6:
                ST6 = {{1,1,0,0,1,1,0,0,1},
                       {0,1,1,1,1,0,1,0,0}};

                STK6 = {{1,1,1,0,1,1,0,0,0},
                        {0,1,1,0,1,1,0,0,1},
                        {1,1,1,1,1,0,0,0,0},
                        {1,1,0,1,1,0,1,0,0},
                        {1,0,0,1,1,0,1,1,0},
                        {0,0,0,1,1,0,1,1,1},
                        {0,0,0,0,1,1,1,1,1},
                        {0,0,1,0,1,1,0,1,1}};
                if(ImgPattern == ST6[0] || ImgPattern == ST6[1] || ImgPattern == STK6[0] || ImgPattern == STK6[1] ||
                   ImgPattern == STK6[2] || ImgPattern == STK6[3] || ImgPattern == STK6[4] || ImgPattern == STK6[5] ||
                   ImgPattern == STK6[6] || ImgPattern == STK6[7] ) {
                    return true;
                }
                else
                    break;
            case 7:
                STK7 = {{1,1,1,0,1,1,0,0,1},
                        {1,1,1,1,1,0,1,0,0},
                        {1,0,0,1,1,0,1,1,1},
                        {0,0,1,0,1,1,1,1,1}};
                if(ImgPattern == STK7[0] || ImgPattern == STK7[1] || ImgPattern == STK7[2] || ImgPattern == STK7[3]) {
                    return true;
                }
                else
                    break;
            case 8:
                STK8 = {{0,1,1,0,1,1,0,1,1},
                        {1,1,1,1,1,1,0,0,0},
                        {1,1,0,1,1,0,1,1,0},
                        {0,0,0,1,1,1,1,1,1}};
                if(ImgPattern == STK8[0] || ImgPattern == STK8[1] || ImgPattern == STK8[2] || ImgPattern == STK8[3]) {
                    return true;
                }
                else
                    break;
            case 9:
                STK9 = {{1,1,1,0,1,1,0,1,1},
                        {0,1,1,0,1,1,1,1,1},
                        {1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,0,0,1},
                        {1,1,1,1,1,0,1,1,0},
                        {1,1,0,1,1,0,1,1,1},
                        {1,0,0,1,1,1,1,1,1},
                        {0,0,1,1,1,1,1,1,1}};
                if(ImgPattern == STK9[0] || ImgPattern == STK9[1] || ImgPattern == STK9[2] || ImgPattern == STK9[3] ||
                   ImgPattern == STK9[4] || ImgPattern == STK9[5] || ImgPattern == STK9[6] || ImgPattern == STK9[7] ) {
                    return true;
                }
                else
                    break;
            case 10:
                STK10 = {{1,1,1,0,1,1,1,1,1},
                         {1,1,1,1,1,1,1,0,1},
                         {1,1,1,1,1,0,1,1,1},
                         {1,0,1,1,1,1,1,1,1}};
                if(ImgPattern == STK10[0] || ImgPattern == STK10[1] || ImgPattern == STK10[2] || ImgPattern == STK10[3]) {
                    return true;
                }
                else
                    break;
            default: 
                return false;
        }
        return false;
}

bool CheckCondPatternTh(std::vector<unsigned char> &ImgPattern, int bond) {
    std::vector<std::vector<unsigned char>> S1, S2, S3, TK4, STK4, ST5_1, ST5_2, ST6, STK6, STK7, STK8, STK9, STK10, K11;  
    
        switch (bond) {
            case 4:
                TK4 = {{0,1,0,0,1,1,0,0,0},
                       {0,1,0,1,1,0,0,0,0},
                       {0,0,0,1,1,0,0,1,0},
                       {0,0,0,0,1,1,0,1,0}}; 
                STK4 = {{0,0,1,0,1,1,0,0,1},
                       {1,1,1,0,1,0,0,0,0},
                       {1,0,0,1,1,0,1,0,0},
                       {0,0,0,0,1,0,1,1,1}}; 
                if(ImgPattern == TK4[0] || ImgPattern == TK4[1] || ImgPattern == TK4[2] || ImgPattern == TK4[3] ||
                   ImgPattern == STK4[0] || ImgPattern == STK4[1] || ImgPattern == STK4[2] || ImgPattern == STK4[3]) {
                    return true;
                }
                else
                    break;
            case 5:
                ST5_1 = {{1,1,0,0,1,1,0,0,0},
                         {0,1,0,0,1,1,0,0,1},
                         {0,1,1,1,1,0,0,0,0},
                         {0,0,1,0,1,1,0,1,0}};

                ST5_2 = {{0,1,1,0,1,1,0,0,0},
                         {1,1,0,1,1,0,0,0,0},
                         {0,0,0,1,1,0,1,1,0},
                         {0,0,0,0,1,1,0,1,1}};
                if(ImgPattern == ST5_1[0] || ImgPattern == ST5_1[1] || ImgPattern == ST5_1[2] || ImgPattern == ST5_1[3] ||
                   ImgPattern == ST5_2[0] || ImgPattern == ST5_2[1] || ImgPattern == ST5_2[2] || ImgPattern == ST5_2[3] ) {
                    return true;
                }
                else        
                    break;
            case 6:
                ST6 = {{1,1,0,0,1,1,0,0,1},
                       {0,1,1,1,1,0,1,0,0}};

                STK6 = {{1,1,1,0,1,1,0,0,0},
                        {0,1,1,0,1,1,0,0,1},
                        {1,1,1,1,1,0,0,0,0},
                        {1,1,0,1,1,0,1,0,0},
                        {1,0,0,1,1,0,1,1,0},
                        {0,0,0,1,1,0,1,1,1},
                        {0,0,0,0,1,1,1,1,1},
                        {0,0,1,0,1,1,0,1,1}};
                if(ImgPattern == ST6[0] || ImgPattern == ST6[1] || ImgPattern == STK6[0] || ImgPattern == STK6[1] ||
                   ImgPattern == STK6[2] || ImgPattern == STK6[3] || ImgPattern == STK6[4] || ImgPattern == STK6[5] ||
                   ImgPattern == STK6[6] || ImgPattern == STK6[7] ) {
                    return true;
                }
                else
                    break;
            case 7:
                STK7 = {{1,1,1,0,1,1,0,0,1},
                        {1,1,1,1,1,0,1,0,0},
                        {1,0,0,1,1,0,1,1,1},
                        {0,0,1,0,1,1,1,1,1}};
                if(ImgPattern == STK7[0] || ImgPattern == STK7[1] || ImgPattern == STK7[2] || ImgPattern == STK7[3]) {
                    return true;
                }
                else
                    break;
            case 8:
                STK8 = {{0,1,1,0,1,1,0,1,1},
                        {1,1,1,1,1,1,0,0,0},
                        {1,1,0,1,1,0,1,1,0},
                        {0,0,0,1,1,1,1,1,1}};
                if(ImgPattern == STK8[0] || ImgPattern == STK8[1] || ImgPattern == STK8[2] || ImgPattern == STK8[3]) {
                    return true;
                }
                else
                    break;
            case 9:
                STK9 = {{1,1,1,0,1,1,0,1,1},
                        {0,1,1,0,1,1,1,1,1},
                        {1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,0,0,1},
                        {1,1,1,1,1,0,1,1,0},
                        {1,1,0,1,1,0,1,1,1},
                        {1,0,0,1,1,1,1,1,1},
                        {0,0,1,1,1,1,1,1,1}};
                if(ImgPattern == STK9[0] || ImgPattern == STK9[1] || ImgPattern == STK9[2] || ImgPattern == STK9[3] ||
                   ImgPattern == STK9[4] || ImgPattern == STK9[5] || ImgPattern == STK9[6] || ImgPattern == STK9[7] ) {
                    return true;
                }
                else
                    break;
            case 10:
                STK10 = {{1,1,1,0,1,1,1,1,1},
                         {1,1,1,1,1,1,1,0,1},
                         {1,1,1,1,1,0,1,1,1},
                         {1,0,1,1,1,1,1,1,1}};
                if(ImgPattern == STK10[0] || ImgPattern == STK10[1] || ImgPattern == STK10[2] || ImgPattern == STK10[3]) {
                    return true;
                }
                else
                    break;
            default: 
                return false;
        }
        return false;
}

bool CheckCondPatternSK(std::vector<unsigned char> &ImgPattern, int bond) {
    std::vector<std::vector<unsigned char>> S1, S2, S3, TK4, STK4, ST5_1, ST5_2, ST6, STK6, STK7, STK8, STK9, STK10, K11;  
    
        switch (bond) {
            case 4:
                TK4 = {{0,1,0,0,1,1,0,0,0},
                       {0,1,0,1,1,0,0,0,0},
                       {0,0,0,1,1,0,0,1,0},
                       {0,0,0,0,1,1,0,1,0}}; 
                STK4 = {{0,0,1,0,1,1,0,0,1},
                       {1,1,1,0,1,0,0,0,0},
                       {1,0,0,1,1,0,1,0,0},
                       {0,0,0,0,1,0,1,1,1}}; 
                if(ImgPattern == TK4[0] || ImgPattern == TK4[1] || ImgPattern == TK4[2] || ImgPattern == TK4[3] ||
                   ImgPattern == STK4[0] || ImgPattern == STK4[1] || ImgPattern == STK4[2] || ImgPattern == STK4[3]) {
                    return true;
                }
                else
                    break;
            case 5:
                ST5_1 = {{1,1,0,0,1,1,0,0,0},
                         {0,1,0,0,1,1,0,0,1},
                         {0,1,1,1,1,0,0,0,0},
                         {0,0,1,0,1,1,0,1,0}};

                ST5_2 = {{0,1,1,0,1,1,0,0,0},
                         {1,1,0,1,1,0,0,0,0},
                         {0,0,0,1,1,0,1,1,0},
                         {0,0,0,0,1,1,0,1,1}};
                if(ImgPattern == ST5_1[0] || ImgPattern == ST5_1[1] || ImgPattern == ST5_1[2] || ImgPattern == ST5_1[3] ||
                   ImgPattern == ST5_2[0] || ImgPattern == ST5_2[1] || ImgPattern == ST5_2[2] || ImgPattern == ST5_2[3] ) {
                    return true;
                }
                else        
                    break;
            case 6:
                STK6 = {{1,1,1,0,1,1,0,0,0},
                        {0,1,1,0,1,1,0,0,1},
                        {1,1,1,1,1,0,0,0,0},
                        {1,1,0,1,1,0,1,0,0},
                        {1,0,0,1,1,0,1,1,0},
                        {0,0,0,1,1,0,1,1,1},
                        {0,0,0,0,1,1,1,1,1},
                        {0,0,1,0,1,1,0,1,1}};
                if(ImgPattern == STK6[0] || ImgPattern == STK6[1] || ImgPattern == STK6[2] || ImgPattern == STK6[3] || 
                   ImgPattern == STK6[4] || ImgPattern == STK6[5] || ImgPattern == STK6[6] || ImgPattern == STK6[7] ) {
                    return true;
                }
                else
                    break;
            case 7:
                STK7 = {{1,1,1,0,1,1,0,0,1},
                        {1,1,1,1,1,0,1,0,0},
                        {1,0,0,1,1,0,1,1,1},
                        {0,0,1,0,1,1,1,1,1}};
                if(ImgPattern == STK7[0] || ImgPattern == STK7[1] || ImgPattern == STK7[2] || ImgPattern == STK7[3]) {
                    return true;
                }
                else
                    break;
            case 8:
                STK8 = {{0,1,1,0,1,1,0,1,1},
                        {1,1,1,1,1,1,0,0,0},
                        {1,1,0,1,1,0,1,1,0},
                        {0,0,0,1,1,1,1,1,1}};
                if(ImgPattern == STK8[0] || ImgPattern == STK8[1] || ImgPattern == STK8[2] || ImgPattern == STK8[3]) {
                    return true;
                }
                else
                    break;
            case 9:
                STK9 = {{1,1,1,0,1,1,0,1,1},
                        {0,1,1,0,1,1,1,1,1},
                        {1,1,1,1,1,1,1,0,0},
                        {1,1,1,1,1,1,0,0,1},
                        {1,1,1,1,1,0,1,1,0},
                        {1,1,0,1,1,0,1,1,1},
                        {1,0,0,1,1,1,1,1,1},
                        {0,0,1,1,1,1,1,1,1}};
                if(ImgPattern == STK9[0] || ImgPattern == STK9[1] || ImgPattern == STK9[2] || ImgPattern == STK9[3] ||
                   ImgPattern == STK9[4] || ImgPattern == STK9[5] || ImgPattern == STK9[6] || ImgPattern == STK9[7] ) {
                    return true;
                }
                else
                    break;
            case 10:
                STK10 = {{1,1,1,0,1,1,1,1,1},
                         {1,1,1,1,1,1,1,0,1},
                         {1,1,1,1,1,0,1,1,1},
                         {1,0,1,1,1,1,1,1,1}};
                if(ImgPattern == STK10[0] || ImgPattern == STK10[1] || ImgPattern == STK10[2] || ImgPattern == STK10[3]) {
                    return true;
                }
                else
                    break;
            case 11:
                K11 = {{1,1,1,1,1,1,0,1,1},
                       {1,1,1,1,1,1,1,1,0},
                       {1,1,0,1,1,1,1,1,1},
                       {0,1,1,1,1,1,1,1,1}};
                if(ImgPattern == K11[0] || ImgPattern == K11[1] || ImgPattern == K11[2] || ImgPattern == K11[3]) {
                    return true;
                }
                else
                    break;
            default: 
                return false;
        }
        return false;
}

/* Shrink & Thin Unconditional LUT */
bool CheckUnConPattern (std::vector<unsigned char> &pattern) {
    // Shrinking LUT
    std::vector<std::vector<unsigned char>> st_= {{0,0,1,0,1,0,0,0,0},
							                     {1,0,0,0,1,0,0,0,0},
							                     {0,0,0,0,1,0,0,1,0},
							                     {0,0,0,0,1,1,0,0,0},
							                     {0,0,1,0,1,1,0,0,0},
							                     {0,1,1,0,1,0,0,0,0},
							                     {1,1,0,0,1,0,0,0,0},
							                     {1,0,0,1,1,0,0,0,0},
							                     {0,0,0,1,1,0,1,0,0},
							                     {0,0,0,0,1,0,1,1,0},
							                     {0,0,0,0,1,0,0,1,1},
							                     {0,0,0,0,1,1,0,0,1},
							                     {0,1,1,1,1,0,0,0,0},
							                     {1,1,0,0,1,1,0,0,0},
							                     {0,1,0,0,1,1,0,0,1},
							                     {0,0,1,0,1,1,0,1,0},
							                     {0,2,1,0,1,2,1,0,0},
							                     {1,2,0,2,1,0,0,0,1},
							                     {0,0,1,2,1,0,1,2,0},
							                     {1,0,0,0,1,2,0,2,1},
							                     {1,1,3,1,1,3,3,3,3},
							                     {3,1,0,1,1,1,3,0,0},
							                     {0,1,3,1,1,1,0,0,3},
							                     {0,0,3,1,1,1,0,1,3},
							                     {3,0,0,1,1,1,3,1,0},
							                     {3,1,3,1,1,0,0,1,0},
							                     {0,1,0,1,1,0,3,1,3},
							                     {0,1,0,0,1,1,3,1,3},
							                     {3,1,3,0,1,1,0,1,0},
							                     {1,3,1,3,1,3,2,2,2},
							                     {1,3,2,3,1,2,1,3,2},
							                     {2,2,2,3,1,3,1,3,1},
							                     {2,3,1,2,1,3,2,3,1},
							                     {3,1,0,0,1,1,1,0,3},
							                     {0,1,3,1,1,0,3,0,1},
							                     {3,0,1,1,1,0,0,1,3},
							                     {1,0,3,0,1,1,3,1,0}};

    int i = 0;
    int index = 0;
    bool ABC_Flag = false;
    bool ABC = false;
    bool D_Flag = false;

    int n = 0;

    for(i = 0;i < st_.size(); i++) { // First, see if pattern have match to the LUT
        
        ABC = false;
        ABC_Flag = false;
        D_Flag = false;
         //without ABCD
        if(pattern == st_[i]) {
             //std::cout << "0" << std::endl;
            return true;
        }

        for(index = 0; index < st_[0].size(); index++) { // IF not, see LUT with A,B,C and D
            
            if(st_[i][index] == 2) {
                ABC = true; // contain ABC
                st_[i][index] = pattern[index];
                if(pattern[index] == 1) { // IF AorBorC, ABC_Flag is ture, otherwise, ABC_Flag is false
                    ABC_Flag = true;
                }
            }
            else if(st_[i][index] == 3) { // IF D, D_FLAG is true
                st_[i][index] = pattern[index];
                D_Flag = true;
            }
        }

        if(ABC && D_Flag) {
            if(pattern == st_[i] && ABC_Flag && D_Flag) { // pattern with ABC / D
                //std::cout << "ABC and D---" ;
                return true;
            }
        }
        else if(ABC) {
            if(pattern == st_[i] && ABC_Flag) {
                //std::cout << "ABC";
                return true;
            }
        }
        else if(D_Flag){
            if(pattern == st_[i]) { // pattern with D
                //std::cout << "D" ;
                return true;
            }
        }
    }
    return false;

}

/* Skeleton Table Check */
bool CheckUnConPatternSK(std::vector<unsigned char> &pattern) {
    std::vector<std::vector<unsigned char>> k_= {{0,0,0,0,1,0,0,0,1},
                                                {0,0,0,0,1,0,1,0,0},
                                                {0,0,1,0,1,0,0,0,0},
                                                {1,0,0,0,1,0,0,0,0},
                                                {0,0,0,0,1,0,0,1,0},
                                                {0,0,0,0,1,1,0,0,0},
                                                {0,0,0,1,1,0,0,0,0},
                                                {0,1,0,0,1,0,0,0,0},
                                                {0,1,0,0,1,1,0,0,0},
                                                {0,1,0,1,1,0,0,0,0},
                                                {0,0,0,0,1,1,0,1,0},
                                                {0,0,0,1,1,0,0,1,0},
                                                {1,1,3,1,1,3,3,3,3},
                                                {3,3,3,3,1,1,3,1,1},
                                                {3,1,3,1,1,1,3,3,3},
                                                {3,1,3,1,1,3,3,1,3},
                                                {3,3,3,1,1,1,3,1,3},
                                                {3,1,3,3,1,1,3,1,3},
                                                {1,3,1,3,1,3,2,2,2},
                                                {1,3,2,3,1,2,1,3,2},
                                                {2,2,2,3,1,3,1,3,1},
                                                {2,3,1,2,1,3,2,3,1},
                                                {3,1,0,0,1,1,1,0,3},
                                                {0,1,3,1,1,0,3,0,1},
                                                {3,0,1,1,1,0,0,1,3},
                                                {1,0,3,0,1,1,3,1,0}};
    int i = 0;
    int index = 0;
    bool ABC_Flag = false;
    bool ABC = false;
    bool D_Flag = false;

    int n = 0;

    for(i = 0;i < k_.size(); i++) { // First, see if pattern have match to the LUT
        
        ABC = false;
        ABC_Flag = false;
        D_Flag = false;
         //without ABCD
        if(pattern == k_[i]) {
             //std::cout << "0" << std::endl;
            return true;
        }

        for(index = 0; index < k_[0].size(); index++) { // IF not, see LUT with A,B,C and D
            
            if(k_[i][index] == 2) {
                ABC = true; // contain ABC
                k_[i][index] = pattern[index];
                if(pattern[index] == 1) { // IF AorBorC, ABC_Flag is ture, otherwise, ABC_Flag is false
                    ABC_Flag = true;
                }
            }
            else if(k_[i][index] == 3) { // IF D, D_FLAG is true
                k_[i][index] = pattern[index];
                D_Flag = true;
            }
        }

        if(ABC && D_Flag) {
            if(pattern == k_[i] && ABC_Flag && D_Flag) { // pattern with ABC / D
                return true;
            }
        }
        else if(ABC) {
            if(pattern == k_[i] && ABC_Flag) {
                return true;
            }
        }
        else if(D_Flag){
            if(pattern == k_[i]) { // pattern with D
                return true;
            }
        }
    }
    return false;
}



