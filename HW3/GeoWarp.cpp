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
	
	std::vector<int> bar;
	double degree; // inorder to compute the x_bar in 1/4 circle

	for(double i = 0; i < 256; i++) {
		degree = asin(i/256);
		bar.push_back(256*cos(degree));
		//std::cout << i << ": "<< bar[i] << std::endl;
	}

	int NewCol;
	int factor;

	//left upside of circle
	int i = 255;
	for(row = 0; row < 256; row++) {
		for(col = 0; col < 256; col++) {

			NewCol = col * bar[i] / 256 + (256-bar[i]);

			ImagedataWarp[row][NewCol][0] = Imagedata[row][col][0];
			ImagedataWarp[row][NewCol][1] = Imagedata[row][col][1];
			ImagedataWarp[row][NewCol][2] = Imagedata[row][col][2];
		}
		i--;   
	}
	
	//left down side of circle
	i = 0;
	for(row = 256; row < Height; row++) {
		for(col = 0; col < 256; col++) {

			NewCol = col * bar[i] / 256 + (256-bar[i]);

			ImagedataWarp[row][NewCol][0] = Imagedata[row][col][0];
			ImagedataWarp[row][NewCol][1] = Imagedata[row][col][1];
			ImagedataWarp[row][NewCol][2] = Imagedata[row][col][2];
		}
		i++;
	}

	//right upside
	i = 255;
	for(row = 0; row < 256; row++) {
		for(col = 0; col < 256; col++) {

			NewCol = col * bar[i] / 256;
			// move to origin, then move back
			ImagedataWarp[row][NewCol+256][0] = Imagedata[row][col+256][0]; 
			ImagedataWarp[row][NewCol+256][1] = Imagedata[row][col+256][1];
			ImagedataWarp[row][NewCol+256][2] = Imagedata[row][col+256][2];
		}
		i--;
	}
	
	//right downside
	i = 0;
	for(row = 256; row < Height; row++) {
		for(col = 0; col < 256; col++) {

			NewCol = col * bar[i] / 256;

			ImagedataWarp[row][NewCol+256][0] = Imagedata[row][col+256][0];
			ImagedataWarp[row][NewCol+256][1] = Imagedata[row][col+256][1];
			ImagedataWarp[row][NewCol+256][2] = Imagedata[row][col+256][2];
		}
		i++;
	}

	int ReverseCol;
	
	i = 255;
	for(row = 0; row < 256; row++) {
		for(col = 0; col < 256; col++) {
			
			NewCol = col * bar[i] / 256 + (256-bar[i]);
			
			ImagedataReverse[row][col][0] = ImagedataWarp[row][NewCol][0];
			ImagedataReverse[row][col][1] = ImagedataWarp[row][NewCol][1];
			ImagedataReverse[row][col][2] = ImagedataWarp[row][NewCol][2];
		}
		i--;
	}

	i = 0;
	for(row = 256; row < Height; row++) {
		for(col = 0; col < 256; col++) {
			
			NewCol = col * bar[i] / 256 + (256-bar[i]);
			
			ImagedataReverse[row][col][0] = ImagedataWarp[row][NewCol][0];
			ImagedataReverse[row][col][1] = ImagedataWarp[row][NewCol][1];
			ImagedataReverse[row][col][2] = ImagedataWarp[row][NewCol][2];
		}
		i++;
	}

	i = 255;
	for(row = 0; row < 256; row++) {
		for(col = 0; col < 256; col++) {

			NewCol = col * bar[i] / 256;

			ImagedataReverse[row][col+256][0] = ImagedataWarp[row][NewCol+256][0];
			ImagedataReverse[row][col+256][1] = ImagedataWarp[row][NewCol+256][1];
			ImagedataReverse[row][col+256][2] = ImagedataWarp[row][NewCol+256][2];
		}
		i--;
	}

	i = 0;
	for(row = 256; row < Height; row++) {
		for(col = 0; col < 256; col++) {

			NewCol = col * bar[i] / 256;

			ImagedataReverse[row][col+256][0] = ImagedataWarp[row][NewCol+256][0];
			ImagedataReverse[row][col+256][1] = ImagedataWarp[row][NewCol+256][1];
			ImagedataReverse[row][col+256][2] = ImagedataWarp[row][NewCol+256][2];
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



