//EE569 Howework Assignment #2
//Date:		February 07 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ ColorHalf.cpp -o ColorHalf
// >> ./ColorHalf Rose.raw RoseED.raw
/*--------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 3; // Gray-scale
    int Width = 640;
    int Height = 480;
	

    // Check for proper syntax
	if (argc < 2){
		std::cout << "Syntax Error - Incorrect Parameter Usage:" << std::endl;
		std::cout << "program_name input_image.raw output_image.raw" << std::endl;
		return 0;
	}
    // Allocate image data array **
	unsigned char Imagedata[Height][Width][BytesPerPixel];
    unsigned char ImagedataFS[Height][Width][BytesPerPixel];
    unsigned char ImagedataFSResult[Height][Width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

    int row = 0;
    int col = 0;
    unsigned char threshold = 128;
    double errorB = 0;
    double errorG = 0;
    double errorR = 0;


    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            ImagedataFS[row][col][0] = 255 - Imagedata[row][col][0];
            ImagedataFS[row][col][1] = 255 - Imagedata[row][col][1];
            ImagedataFS[row][col][2] = 255 - Imagedata[row][col][2];
        }
    }

    for(row = 0; row < Height; row++) {
        // serpentine scanning
        if(row%2 == 0) { // ---->
            for(col = 0; col < Width; col++) {
                if(ImagedataFS[row][col][0] > threshold) {
                    errorB = ImagedataFS[row][col][0] - 255;
                }
                else {
                    errorB = ImagedataFS[row][col][0] - 0;
                }
                if(ImagedataFS[row][col][1] > threshold) {
                    errorG = ImagedataFS[row][col][1] - 255;
                }
                else {
                    errorG = ImagedataFS[row][col][1] - 0;
                }
                if(ImagedataFS[row][col][2] > threshold) {
                    errorR = ImagedataFS[row][col][2] - 255;
                }
                else {
                    errorR = ImagedataFS[row][col][2] - 0;
                }
                ImagedataFS[row][col+1][0] += floor(errorB * 7/16.0);
                ImagedataFS[row][col+1][1] += floor(errorG * 7/16.0);
                ImagedataFS[row][col+1][2] += floor(errorR * 7/16.0);
                ImagedataFS[row+1][col-1][0] += floor(errorB * 3/16.0);
                ImagedataFS[row+1][col-1][1] += floor(errorG * 3/16.0);
                ImagedataFS[row+1][col-1][2] += floor(errorR * 3/16.0);
                ImagedataFS[row+1][col][0] += floor(errorB * 5/16.0);
                ImagedataFS[row+1][col][1] += floor(errorG * 5/16.0);
                ImagedataFS[row+1][col][2] += floor(errorR * 5/16.0);
                ImagedataFS[row+1][col+1][0] += floor(errorB * 1/16.0);
                ImagedataFS[row+1][col+1][1] += floor(errorG * 1/16.0);
                ImagedataFS[row+1][col+1][2] += floor(errorR * 1/16.0);

                if(ImagedataFS[row][col][0] < threshold) {
                    ImagedataFSResult[row][col][0] = 0;
                }  
                else {
                    ImagedataFSResult[row][col][0] = 255;
                }
                if(ImagedataFS[row][col][1] < threshold) {
                    ImagedataFSResult[row][col][1] = 0;
                }  
                else {
                    ImagedataFSResult[row][col][1] = 255;
                }  
                if(ImagedataFS[row][col][2] < threshold) {
                    ImagedataFSResult[row][col][2] = 0;
                }  
                else {
                    ImagedataFSResult[row][col][2] = 255;
                }    
            }
        }
        else{   // <-----
            for(col = Width-1; col >= 0; col--) {
                if(ImagedataFS[row][col][0] > threshold) {
                    errorB = ImagedataFS[row][col][0] - 255;
                }
                else {
                    errorB = ImagedataFS[row][col][0] - 0;
                }
                if(ImagedataFS[row][col][1] > threshold) {
                    errorG = ImagedataFS[row][col][1] - 255;
                }
                else {
                    errorG = ImagedataFS[row][col][1] - 0;
                }
                if(ImagedataFS[row][col][2] > threshold) {
                    errorR = ImagedataFS[row][col][2] - 255;
                }
                else {
                    errorR = ImagedataFS[row][col][2] - 0;
                }
                ImagedataFS[row][col+1][0] += floor(errorB * 7/16.0);
                ImagedataFS[row][col+1][1] += floor(errorG * 7/16.0);
                ImagedataFS[row][col+1][2] += floor(errorR * 7/16.0);
                ImagedataFS[row+1][col-1][0] += floor(errorB * 3/16.0);
                ImagedataFS[row+1][col-1][1] += floor(errorG * 3/16.0);
                ImagedataFS[row+1][col-1][2] += floor(errorR * 3/16.0);
                ImagedataFS[row+1][col][0] += floor(errorB * 5/16.0);
                ImagedataFS[row+1][col][1] += floor(errorG * 5/16.0);
                ImagedataFS[row+1][col][2] += floor(errorR * 5/16.0);
                ImagedataFS[row+1][col+1][0] += floor(errorB * 1/16.0);
                ImagedataFS[row+1][col+1][1] += floor(errorG * 1/16.0);
                ImagedataFS[row+1][col+1][2] += floor(errorR * 1/16.0);
                
                if(ImagedataFS[row][col][0] < threshold) {
                    ImagedataFSResult[row][col][0] = 0;
                }  
                else {
                    ImagedataFSResult[row][col][0] = 255;
                }
                if(ImagedataFS[row][col][1] < threshold) {
                    ImagedataFSResult[row][col][1] = 0;
                }  
                else {
                    ImagedataFSResult[row][col][1] = 255;
                }  
                if(ImagedataFS[row][col][2] < threshold) {
                    ImagedataFSResult[row][col][2] = 0;
                }  
                else {
                    ImagedataFSResult[row][col][2] = 255;
                }    
            }
        }   
    }

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            ImagedataFSResult[row][col][0] = 255 - ImagedataFSResult[row][col][0];
            ImagedataFSResult[row][col][1] = 255 - ImagedataFSResult[row][col][1];
            ImagedataFSResult[row][col][2] = 255 - ImagedataFSResult[row][col][2];
        }
    }

   
    // gray image
    if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataFSResult, sizeof(unsigned char), Width * Height * BytesPerPixel, file);
	fclose(file);

    return 0;
}