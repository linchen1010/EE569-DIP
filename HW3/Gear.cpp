//EE569 Howework Assignment #3
//Date:		February 23 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  March 3, 2020
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ -std=c++11 Gear.cpp -o Gear
// >> ./Gear GearTooth.raw GearFinal.raw
/*--------------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#define PI 3.14159265358979323846264338327950288

int Bond(std::vector<unsigned char> pattern);
bool CheckCondPattern(std::vector<unsigned char> &ImgPattern, int bond); // Shrink
bool CheckUnConPattern (std::vector<unsigned char> &pattern); // Shrink / Thin

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 1; // Greyscale
    int Width = 250;
    int Height = 250;
	int row = 0;
	int col = 0;
    int cnt = 0;

    // Check for proper syntax
	if (argc < 2){
		std::cout << "Syntax Error - Incorrect Parameter Usage:" << std::endl;
		std::cout << "program_name input_image.raw output_image.raw" << std::endl;
		return 0;
	}
    // Allocate image data array
	unsigned char Imagedata[Height][Width][BytesPerPixel];
    unsigned char ImagedataBinary[Height][Width][BytesPerPixel]; // ImageBinarize with Shrinking
    unsigned char ImagedataB[Height][Width][BytesPerPixel]; // ImageBinarize without Shrinking
    unsigned char Morph[Height][Width][BytesPerPixel]; // Shrink_help map
    unsigned char tempImg[Height][Width][BytesPerPixel]; // flag
    unsigned char ImagedataR[Height][Width][BytesPerPixel]; // Reverse Binarize Image
    unsigned char ImagedataDetect[Height][Width][BytesPerPixel];
    unsigned char ImagedataCircle[Height][Width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

    for(row = 0; row < Height ; row++) {
        for(col = 0; col < Width; col++) {
            if(Imagedata[row][col][0] < 127) {
                ImagedataBinary[row][col][0] = 0;
                ImagedataB[row][col][0] = 0;
                ImagedataR[row][col][0] = 255;
            }
            else {
                ImagedataBinary[row][col][0] = 1;
                ImagedataB[row][col][0] = 1;
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
        for(col = 0; col < Width; col++) {
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
    //std::cout << "Processing-> " << n  << " times" << std::endl; 
    }

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            if(row == 125 && col == 125)
                ImagedataDetect[row][col][0] = 1;
            else
                ImagedataDetect[row][col][0] = 0;
        }
    }

    double radius = 110; // by calculate from center

    double X_dir, Y_dir;
    int TeethCnt = 0;;
    int tmp = 1;
    int r = 0;

    for(col = 125; col > 0; col --) {
        if(ImagedataB[125][col][0] == 1) {
            r++;
        }
        else break;
    }


    // Detect tooth
    for(double i = 0; i < 360; i+=30) {
        X_dir = radius * cos(i * PI / 180);
        Y_dir = radius * sin(i * PI / 180);
        if(ImagedataB[125+int(X_dir)][125+int(Y_dir)][0]==1) {
            TeethCnt++;
            ImagedataDetect[125+int(X_dir)][125+int(Y_dir)][0] = 1;
        }
        else {
            std::cout << "Miss Teeth position: " << "row: " << 125+int(X_dir) << " ,col: " << 125+int(Y_dir) << std::endl;
        }
        ImagedataCircle[125+int(X_dir)][125+int(Y_dir)][0] = 255;
    }

    ImagedataCircle[125][125][0] = 255;

    std::cout << "TeethCnt: " << TeethCnt << std::endl;
    std::cout << "Radius: " << r << std::endl;

    for(row = 0; row < Height ; row++) {
        for(col = 0; col < Width; col++) {
            if(ImagedataBinary[row][col][0] != 0) {
                ImagedataBinary[row][col][0] = 255;
            }
            if(ImagedataDetect[row][col][0] != 0) {
                ImagedataDetect[row][col][0] = 255;
            }
        }
    }

	if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataCircle, sizeof(unsigned char), Width * Height * BytesPerPixel, file);
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



