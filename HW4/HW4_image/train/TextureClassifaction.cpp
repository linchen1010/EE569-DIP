//EE569 Howework Assignment #3
//Date:		March 16 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  March 22, 2020
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ -std=c++11 Star.cpp -o Star
// >> ./Star stars.raw starCount.raw
/*---------------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <map>
#include "LawsFilter.h"

void readFile(const char* FileName, unsigned char* Imagedata, int Height, int Width, int BytesPerPixel);

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 1; // Grayscale
    int Width = 128;
    int Height = 128;
	int row = 0;
	int col = 0;

    // Check for proper syntax
	if (argc < 2){
		std::cout << "Syntax Error - Incorrect Parameter Usage:" << std::endl;
		std::cout << "program_name input_image.raw output_image.raw" << std::endl;
		return 0;
	}

    // Allocate image data array
	unsigned char Imagedata[Height][Width][BytesPerPixel];
    double tempImg[Height][Width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

    double Avg = 0;
    int WindowSize = 5;
    double temp = 0;
    int FilterCnt = 0;

    double tempAvg = 0;

    for(int x = 0; x < 9; x++) {

        // iterate all train files
        char file1[20]={0};
        FILE *fileName;
        sprintf(file1,"brick%d.raw",x+1);
        fileName = fopen(file1,"rb");
        fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, fileName);

        for(row = 0; row < Height ; row++) {
            for(col = 0; col < Width; col++) {
                tempImg[row][col][0] = Imagedata[row][col][0];
            }
        }
        
        for(row = 2; row < Height - 2; row++) {
            for(col = 2; col < Width - 2; col++) {
                for(int i = -2; i < 3; i++) {
                    for(int j = -2; j < 3; j++) {
                        tempAvg += tempImg[row+i][col+j][0] / (5*5);
                    }
                }
                tempImg[row][col][0] -= tempAvg;
                tempAvg = 0;
            }
        }

        // Img conv Law's filter
        while(FilterCnt < 15) {
            for(row = 2; row < Height-2; row++) {
                for(col = 2; col < Width-2; col++) {
                    for(int i = -2; i < 3; i++) {
                        for(int j = -2; j < 3; j++) {
                            temp += (tempImg[row+i][col+j][0] * LawsFilter[FilterCnt][i+2][j+2]) / (Height * Width);
                        }
                    }
                }
            }
        FilterCnt++;
        }
        std::cout << "mean: " << temp << std::endl;
        temp = 0;
        FilterCnt = 0;
    }

}