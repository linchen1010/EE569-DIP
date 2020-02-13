//EE569 Howework Assignment #2
//Date:		February 07 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ Dithering_FixTh.cpp -o Dithering_FixTh
// >> ./Dihtering_FixTh LightHouse.raw LightHoseFixThreshold.raw
/*--------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 1; // Gray-scale
    int Width = 750;
    int Height = 500;
	

    // Check for proper syntax
	if (argc < 2){
		std::cout << "Syntax Error - Incorrect Parameter Usage:" << std::endl;
		std::cout << "program_name input_image.raw output_image.raw" << std::endl;
		return 0;
	}
    // Allocate image data array **
	unsigned char Imagedata[Height][Width][BytesPerPixel];
    double ImagedataFS[Height][Width][BytesPerPixel];
    unsigned char ImagedataFSResult[Height][Width][BytesPerPixel];
    double tempImage[Height][Width][BytesPerPixel];
    unsigned char ImagedataJJN[Height][Width][BytesPerPixel];
    unsigned char ImagedataStucki[Height][Width][BytesPerPixel];

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
    double error = 0;

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            ImagedataFS[row][col][0] = Imagedata[row][col][0];
            tempImage[row][col][0] = Imagedata[row][col][0];
        }
    }

    for(row = 0; row < Height; row++) {
        // serpentine scanning
        if(row%2 == 0) { // ---->
            for(col = 0; col < Width; col++) {
                if(ImagedataFS[row][col][0] > threshold) {
                    error = Imagedata[row][col][0] - 255;
                }
                else {
                    error = Imagedata[row][col][0] - 0;
                }
                ImagedataFS[row][col+1][0] += floor(error * 7/16.0);
                ImagedataFS[row+1][col-1][0] += floor(error * 3/16.0);
                ImagedataFS[row+1][col][0] += floor(error * 5/16.0);
                ImagedataFS[row+1][col+1][0] += floor(error * 1/16.0);

                if(ImagedataFS[row][col][0] < threshold) {
                    ImagedataFSResult[row][col][0] = 0;
                }
                else {
                    ImagedataFSResult[row][col][0] = 255;
                }  
            }
        }
        else{   // <-----
            for(col = Width-1; col >= 0; col--) {
                if(ImagedataFS[row][col][0] > threshold) {
                    error = ImagedataFS[row][col][0] - 255;
                }
                else {
                    error = ImagedataFS[row][col][0] - 0;
                }
                ImagedataFS[row][col-1][0] += floor(error * 7/16.0);
                ImagedataFS[row+1][col+1][0] += floor(error * 3/16.0);
                ImagedataFS[row+1][col][0] += floor(error * 5/16.0);
                ImagedataFS[row+1][col-1][0] += floor(error * 1/16.0);
                
                if(ImagedataFS[row][col][0] < threshold) {
                    ImagedataFSResult[row][col][0] = 0;
                }  
                else {
                    ImagedataFSResult[row][col][0] = 255;
                }  
            }
        }   
    }

    // JJN
    for(row = 0; row < Height; row++) {
        if(row%2 == 0) {
            for(col = 0; col < Width; col++) {
                if(tempImage[row][col][0] > threshold) {
                    error = tempImage[row][col][0] - 255;
                }
                else {
                    error = tempImage[row][col][0] - 0;
                }
                tempImage[row][col+1][0] += (error * 7 / 48);
                tempImage[row][col+2][0] += (error * 5 / 48);
                tempImage[row+1][col-2][0] += (error * 3 / 48);
                tempImage[row+1][col-1][0] += (error * 5 / 48);
                tempImage[row+1][col][0] += (error * 7 / 48);
                tempImage[row+1][col+1][0] += (error * 5 / 48);
                tempImage[row+1][col+2][0] += (error * 3 / 48);
                tempImage[row+2][col-2][0] += (error * 1 / 48);
                tempImage[row+2][col-1][0] += (error * 3 / 48);
                tempImage[row+2][col][0] += (error * 5 / 48);
                tempImage[row+2][col+1][0] += (error * 3 / 48);
                tempImage[row+2][col+2][0] += (error * 1 / 48);

                if(tempImage[row][col][0] < threshold) {
                    ImagedataJJN[row][col][0] = 0;
                }
                else {
                    ImagedataJJN[row][col][0] = 255;
                }  
            }
        }
        else{
            for(col = Width-1; col >= 0; col--) {
                if(tempImage[row][col][0] > threshold) {
                    error = tempImage[row][col][0] - 255;
                }
                else {
                    error = tempImage[row][col][0] - 0;
                }
                tempImage[row][col-1][0] += (error * 7/48);
                tempImage[row][col-2][0] += (error * 5/48);
                tempImage[row+1][col-2][0] += (error * 3/48);
                tempImage[row+1][col-1][0] += (error * 5/48);
                tempImage[row+1][col][0] += (error * 7/48);
                tempImage[row+1][col+1][0] += (error * 5/48);
                tempImage[row+1][col+2][0] += (error * 3/48);
                tempImage[row+2][col-2][0] += (error * 1/48);
                tempImage[row+2][col-1][0] += (error * 3/48);
                tempImage[row+2][col][0] += (error * 5/48);
                tempImage[row+2][col+1][0] += (error * 3/48);
                tempImage[row+2][col+2][0] += (error * 1/48);

                if(tempImage[row][col][0] < threshold) {
                    ImagedataJJN[row][col][0] = 0;
                }
                else {
                    ImagedataJJN[row][col][0] = 255;
                }  
            }
        }   
    }

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            tempImage[row][col][0] = Imagedata[row][col][0];
        }
    }

    //Stucki
    for(row = 0; row < Height; row++) {
        if(row%2 == 0) {
            for(col = 0; col < Width; col++) {
                if(tempImage[row][col][0] > threshold) {
                    error = tempImage[row][col][0] - 255;
                }
                else {
                    error = tempImage[row][col][0] - 0;
                }
                tempImage[row][col+1][0] += (error * 8/42);
                tempImage[row][col+2][0] += (error * 4/42);
                tempImage[row+1][col-2][0] += (error * 2/42);
                tempImage[row+1][col-1][0] += (error * 4/42);
                tempImage[row+1][col][0] += (error * 8/42);
                tempImage[row+1][col+1][0] += (error * 4/42);
                tempImage[row+1][col+2][0] += (error * 2/42);
                tempImage[row+2][col-2][0] += (error * 1/42);
                tempImage[row+2][col-1][0] += (error * 2/42);
                tempImage[row+2][col][0] += (error * 4/42);
                tempImage[row+2][col+1][0] += (error * 2/42);
                tempImage[row+2][col+2][0] += (error * 1/42);

                if(tempImage[row][col][0] < threshold) {
                    ImagedataStucki[row][col][0] = 0;
                }
                else {
                    ImagedataStucki[row][col][0] = 255;
                }  
            }
        }
        else{
            for(col = Width-1; col >= 0; col--) {
                if(tempImage[row][col][0] > threshold) {
                    error = tempImage[row][col][0] - 255;
                }
                else {
                    error = tempImage[row][col][0] - 0;
                }
                tempImage[row][col-1][0] += (error * 8/42);
                tempImage[row][col-2][0] += (error * 4/42);
                tempImage[row+1][col-2][0] += (error * 2/42);
                tempImage[row+1][col-1][0] += (error * 4/42);
                tempImage[row+1][col][0] += (error * 8/42);
                tempImage[row+1][col+1][0] += (error * 4/42);
                tempImage[row+1][col+2][0] += (error * 2/42);
                tempImage[row+2][col-2][0] += (error * 1/42);
                tempImage[row+2][col-1][0] += (error * 2/42);
                tempImage[row+2][col][0] += (error * 4/42);
                tempImage[row+2][col+1][0] += (error * 2/42);
                tempImage[row+2][col+2][0] += (error * 1/42);

                if(tempImage[row][col][0] < threshold) {
                    ImagedataStucki[row][col][0] = 0;
                }
                else {
                    ImagedataStucki[row][col][0] = 255;
                }  
            }
        }   
    }

    // gray image
    if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataFSResult, sizeof(unsigned char), Width * Height * 1, file);
	fclose(file);

    if (!(file=fopen(argv[3],"wb"))) {
		std::cout << "Cannot open file: " << argv[3] << std::endl;
		exit(1);
	}
	fwrite(ImagedataJJN, sizeof(unsigned char), Width * Height * 1, file);
	fclose(file);

    if (!(file=fopen(argv[4],"wb"))) {
		std::cout << "Cannot open file: " << argv[4] << std::endl;
		exit(1);
	}
	fwrite(ImagedataStucki, sizeof(unsigned char), Width * Height * 1, file);
	fclose(file);

    return 0;
}