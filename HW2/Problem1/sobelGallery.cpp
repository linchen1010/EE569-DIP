//EE569 Howework Assignment #2
//Date:		February 07 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ sobel.cpp -o sobel
// >> sobel 
/*--------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 3; // RGB
    int Width = 481;
    int Height = 321;
	std::vector<double> tempX; // vector that used to find min,max to do minmax normalization
	std::vector<double> tempY;

    // Check for proper syntax
	if (argc < 2){
		std::cout << "Syntax Error - Incorrect Parameter Usage:" << std::endl;
		std::cout << "program_name input_image.raw output_image.raw" << std::endl;
		return 0;
	}
    // Allocate image data array **
	unsigned char Imagedata[Height][Width][BytesPerPixel];
    unsigned char ImageGray[Height][Width][1];
	double ImageXgradient[Height][Width][1];
	double ImageYgradient[Height][Width][1];
	unsigned char ImageMagnitude[Height][Width][1];
	unsigned char ImageSobel[Height][Width][1];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);
	////////////////////////////////////////////////////////////////////////////////////
    int row = 0;
	int col = 0;
	// Convert RGB to gray-scale image
	for(col = 0; col < Width; col++) {
		for(row = 0; row < Height; row++) {
			ImageGray[row][col][0] = 0.2989 * Imagedata[row][col][2] + // R
									 0.5870 * Imagedata[row][col][1] + // G
									 0.1140 * Imagedata[row][col][0];  // B
		}
	}

	// Gx, Gy are sobel filter
	double Gx[3][3][1] = { {-1,0,1}, {-2,0,2}, {-1,0,1}};
	double Gy[3][3][1] = { {1,2,1}, {0,0,0}, {-1,-2,-1}};
	// i,j using for for-loop index of kernel
	int i = 0;
	int j = 0;
	double min_X = 0;
	double max_X = 0;
	double min_Y = 0;
	double max_Y = 0;

	for(row = 0; row < Height; row++) {
		for(col = 0; col < Width; col++) {
			for(i = -1; i < 2; i++) { 	// convoulution & boundary part 
				for(j = -1; j < 2; j++) {
					if(col == 0 && row == 0) {
						ImageXgradient[row][col][0] += double(ImageGray[row+1+i][col+1+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row+1+i][col+1+j][0]) * Gy[i+1][j+1][0];
					}
					else if(col == Width-1 && row == 0) {
						ImageXgradient[row][col][0] += double(ImageGray[row+1+i][col-1+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row+1+i][col-1+j][0]) * Gy[i+1][j+1][0];
					}
					else if(col == 0 && row == Height - 1) {
						ImageXgradient[row][col][0] += double(ImageGray[row-1+i][col+1+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row-1+i][col-1+j][0]) * Gy[i+1][j+1][0];
					}
					else if(col == Width -1 && row == Height - 1 ) {
						ImageXgradient[row][col][0] += double(ImageGray[row-1+i][col-1+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row-1+i][col-1+j][0]) * Gy[i+1][j+1][0];
					}
					else if(row == 0 ) {
						ImageXgradient[row][col][0] += double(ImageGray[row+1+i][col+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row+1+i][col+j][0]) * Gy[i+1][j+1][0];
					}
					else if(col == 0) {
						ImageXgradient[row][col][0] += double(ImageGray[row+i][col+1+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row+i][col+1+j][0]) * Gy[i+1][j+1][0];	
					}
					else if(row == Height - 1 ) {
						ImageXgradient[row][col][0] += double(ImageGray[row-1+i][col+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row-1+i][col+j][0]) * Gy[i+1][j+1][0];
					}
					else if(col == Width - 1) {
						ImageXgradient[row][col][0] += double(ImageGray[row+i][col-1+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row+i][col-1+j][0]) * Gy[i+1][j+1][0];	
					}
					else{
						ImageXgradient[row][col][0] += double(ImageGray[row+i][col+j][0]) * Gx[i+1][j+1][0];
						ImageYgradient[row][col][0] += double(ImageGray[row+i][col+j][0]) * Gy[i+1][j+1][0];
					}
				}
			} 
				tempX.push_back(ImageXgradient[row][col][0]);
				tempY.push_back(ImageYgradient[row][col][0]);
		}
	}
	// find min & max of gradient to do minmax normalization
	std::sort(tempX.begin(),tempX.end());
	std::sort(tempY.begin(),tempY.end());
	min_X = tempX[0];
	max_X = tempX[tempX.size()-1];
	min_Y = tempY[0];
	max_Y = tempY[tempY.size()-1];
	tempX.clear();
	tempY.clear();

	unsigned char NormalX[Height][Width][1];
	unsigned char NormalY[Height][Width][1];

	// mixmax normalization to x-gradient & y-gradient 
	for(row = 0; row < Height; row++) {
		for(col = 0; col < Width; col++) {
			NormalX[row][col][0] = 255*(ImageXgradient[row][col][0] - min_X)/(max_X-min_X);
			NormalY[row][col][0] = 255*(ImageYgradient[row][col][0] - min_Y)/(max_Y-min_Y);
		}
	}

	int gradient[1352];
	double cumGraMag;
	double cumGraPro[256];
	double temp;
	int temp_cnt;
	double threshold = 0.1*(max_X-min_X);

	// magnitude of gradient
	for(row = 0; row < Height; row++) {
		for(col = 0; col < Width; col++) {
			temp = abs(ImageXgradient[row][col][0]) + abs(ImageYgradient[row][col][0]);
			//ImageSobel[row][col][0] = abs(ImageXgradient[row][col][0]) + abs(ImageYgradient[row][col][0]);
			ImageMagnitude[row][col][0] = temp;
			if(temp > threshold) {
				ImageSobel[row][col][0] = 255;
			}
			else {
				ImageSobel[row][col][0] = 0;
			}
		}
	}

	// gray image
    if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImageGray, sizeof(unsigned char), Width * Height * 1, file);
	fclose(file);

	// x-gradient
	if (!(file=fopen(argv[3],"wb"))) {
		std::cout << "Cannot open file: " << argv[3] << std::endl;
		exit(1);
	}
	fwrite(NormalX, sizeof(unsigned char), Width * Height * 1, file);
	fclose(file);

	// y-gradient 
	if (!(file=fopen(argv[4],"wb"))) {
		std::cout << "Cannot open file: " << argv[4] << std::endl;
		exit(1);
	}
	fwrite(NormalY, sizeof(unsigned char), Width * Height * 1, file);
	fclose(file);

	// gradient magnitude

	if (!(file=fopen(argv[5],"wb"))) {
		std::cout << "Cannot open file: " << argv[5] << std::endl;
		exit(1);
	}
	fwrite(ImageMagnitude, sizeof(unsigned char), Width * Height * 1, file);
	fclose(file);
	

	return 0;
}