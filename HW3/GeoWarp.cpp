//EE569 Howework Assignment #3
//Date:		February 23 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ GeoWarp.cpp -o GeoWarp
// >> ./GeoWarp
/*--------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <algorithm>
#include <vector>

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 3; // RGB
    int Width = 512;
    int Height = 512;
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
	unsigned char ImagedataWarp[Height][Width][BytesPerPixel];
	unsigned char ImagedataReverse[Height][Width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);
	
	std::vector<int> bar; // Circle horizonal length
	double degree; // inorder to compute the x_bar in 1/4 circle

	for(double i = 0; i < 256; i++) {
		degree = asin(i/256);
		bar.push_back(256*cos(degree));
		//std::cout << i << ": "<< bar[i] << std::endl;
	}

	int NewCol, NewCol_2; // NewCol in order to map, NewCol = a * col
	int i;

	/** Warping Part **/
	i = 255;
	for(row = 0; row < 256; row++) {
		for(col = 0; col < 256; col++) {

			NewCol = col * bar[i] / 256 + (256-bar[i]);
			NewCol_2 = col * bar[i] / 256;
			// Left-up Circle
			ImagedataWarp[row][NewCol][0] = Imagedata[row][col][0];
			ImagedataWarp[row][NewCol][1] = Imagedata[row][col][1];
			ImagedataWarp[row][NewCol][2] = Imagedata[row][col][2];
			// Right-up Circle
			ImagedataWarp[row][NewCol_2+256][0] = Imagedata[row][col+256][0]; 
			ImagedataWarp[row][NewCol_2+256][1] = Imagedata[row][col+256][1];
			ImagedataWarp[row][NewCol_2+256][2] = Imagedata[row][col+256][2];
		}
		i--;   
	}
	
	i = 0;
	for(row = 256; row < Height; row++) {
		for(col = 0; col < 256; col++) {

			NewCol = col * bar[i] / 256 + (256-bar[i]);
			NewCol_2 = col * bar[i] / 256;

			// Left-down Circle
			ImagedataWarp[row][NewCol][0] = Imagedata[row][col][0];
			ImagedataWarp[row][NewCol][1] = Imagedata[row][col][1];
			ImagedataWarp[row][NewCol][2] = Imagedata[row][col][2];
			// Right-down Circle
			ImagedataWarp[row][NewCol_2+256][0] = Imagedata[row][col+256][0];
			ImagedataWarp[row][NewCol_2+256][1] = Imagedata[row][col+256][1];
			ImagedataWarp[row][NewCol_2+256][2] = Imagedata[row][col+256][2];
		}
		i++;
	}

	int ReverseCol, ReverseCol_2;
	
	/** Reverse Part **/
	i = 255;
	for(row = 0; row < 256; row++) {
		for(col = 0; col < 256; col++) {
			
			ReverseCol = col * bar[i] / 256 + (256-bar[i]);
			ReverseCol_2 = col * bar[i] / 256;

			// Left-up Circle
			ImagedataReverse[row][col][0] = ImagedataWarp[row][ReverseCol][0];
			ImagedataReverse[row][col][1] = ImagedataWarp[row][ReverseCol][1];
			ImagedataReverse[row][col][2] = ImagedataWarp[row][ReverseCol][2];
			// Right-up Circle
			ImagedataReverse[row][col+256][0] = ImagedataWarp[row][ReverseCol_2+256][0];
			ImagedataReverse[row][col+256][1] = ImagedataWarp[row][ReverseCol_2+256][1];
			ImagedataReverse[row][col+256][2] = ImagedataWarp[row][ReverseCol_2+256][2];
		}
		i--;
	}

	i = 0;
	for(row = 256; row < Height; row++) {
		for(col = 0; col < 256; col++) {
			
			ReverseCol = col * bar[i] / 256 + (256-bar[i]);
			ReverseCol_2 = col * bar[i] / 256;

			// Left-down Circle
			ImagedataReverse[row][col][0] = ImagedataWarp[row][ReverseCol][0];
			ImagedataReverse[row][col][1] = ImagedataWarp[row][ReverseCol][1];
			ImagedataReverse[row][col][2] = ImagedataWarp[row][ReverseCol][2];
			// Right-down Circle
			ImagedataReverse[row][col+256][0] = ImagedataWarp[row][ReverseCol_2+256][0];
			ImagedataReverse[row][col+256][1] = ImagedataWarp[row][ReverseCol_2+256][1];
			ImagedataReverse[row][col+256][2] = ImagedataWarp[row][ReverseCol_2+256][2];
		}
		i++;
	}

	

	if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataWarp, sizeof(unsigned char), Width * Height * 3, file);
	fclose(file);

	if (!(file=fopen(argv[3],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataReverse, sizeof(unsigned char), Width * Height * 3, file);
	fclose(file);

	return 0;
}



