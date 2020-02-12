//EE569 Howework Assignment #2
//Date:		February 07 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ Dithering_RanTh.cpp -o Dithering_RanTh
// >> ./Dithering_RanTh LightHose.raw LightHoseRanTh.raw
/*--------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <random>

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 1; // Gray-scale
    int Width = 750;
    int Height = 500;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,255);



    // Check for proper syntax
	if (argc < 2){
		std::cout << "Syntax Error - Incorrect Parameter Usage:" << std::endl;
		std::cout << "program_name input_image.raw output_image.raw" << std::endl;
		return 0;
	}
    // Allocate image data array **
	unsigned char Imagedata[Height][Width][BytesPerPixel];
    unsigned char ImagedataRanTh[Height][Width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

    int row = 0;
    int col = 0;
    int threshold = 128;

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            if(0 <= Imagedata[row][col][0] && Imagedata[row][col][0] < distribution(generator)) {
                ImagedataRanTh[row][col][0] = 0;
            }
            else if(distribution(generator) <= Imagedata[row][col][0] && Imagedata[row][col][0] < 255){
                ImagedataRanTh[row][col][0] = 255;
            }
        }
    }

    // gray image
    if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataRanTh, sizeof(unsigned char), Width * Height * 1, file);
	fclose(file);

    return 0;
}