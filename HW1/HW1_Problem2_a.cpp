//EE569 Howework Assignment #1
//Date:		Janary 13, 2019
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Compiled on MAC cmd with g++
// >> g++ HW1_Problem2_a.cpp -o HW1_Problem2_a
// >> ./HW1_Problem2_a Corn_noisy.raw Corn_uniform.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{ 
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Width = 320;
	int Height = 320;
	int RGB_BytePerPixel = 3;
	int Size = 256;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw" << endl;
		return 0;
	}
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			Size = atoi(argv[4]);
		}
	}

	// Allocate image data array **
	unsigned char Imagedata [Height][Width][BytesPerPixel];
	unsigned char Imagedata_m [Height+2][Width+2][BytesPerPixel]; //Mirror reflecting
	unsigned char DenoiseImagedata [Height][Width][BytesPerPixel];
	unsigned char Denoise2Imagedata [Height][Width][BytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	//fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

	unsigned char uniformFilter[3][3][1] = {1,1,1,
											1,1,1,
											1,1,1};

	int row = 0;
	int col = 0;
	int n = 0;

	for(row = 0; row < Height; row++) {
		for(col = 0; col < Width; col++) {
			for(n = -1; n < 2; n++) {
				DenoiseImagedata[row][col][0] += (Imagedata[row-1][col+n][0] * uniformFilter[0][n+1][0]) / 9;
				DenoiseImagedata[row][col][0] += (Imagedata[row][col+n][0] * uniformFilter[1][n+1][0])  / 9;
				DenoiseImagedata[row][col][0] += (Imagedata[row+1][col+n][0] * uniformFilter[2][n+1][0]) / 9;

			}
			for(n = -2; n < 3; n++) {
				Denoise2Imagedata[row][col][0] += Imagedata[row+n][col-2][0];
			}
		}
	}

	cout << uniformFilter[1][1][0] << endl; 





	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(DenoiseImagedata, sizeof(unsigned char), Width * Height * BytesPerPixel, file);
	fclose(file);

	return 0;
}