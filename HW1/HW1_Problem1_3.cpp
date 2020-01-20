//EE569 Howework Assignment #1
//Date:		Janary 13, 2019
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Compiled on MAC cmd with g++
// >> g++ HW1_Problem1_3.cpp HW1_Problem1_3
// >> ./HW1_Problem1_3 Toy.raw Histogram.txt
/*------------------------------*/
// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

struct Histogram {
	int Red[256];
	int Green[256];
	int Blue[256];
};Histogram H;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 256;
	int Width = 560;
	int Height = 400;
	int RGB_BytePerPixel = 3;


	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_HistogramValue.txt [BytesPerPixel = 1] [Size = 256]" << endl;
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

	unsigned char Imagedata[Height][Width][RGB_BytePerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	//fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fread(Imagedata, sizeof(unsigned char), Width*Height*RGB_BytePerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	int col = 0;
	int row = 0;
	int total = 0;

	for(col = 0; col < Width; col++) {
		for(row = 0; row < Height; row++) {
			H.Blue[Imagedata[row][col][0]]++;	
			H.Green[Imagedata[row][col][1]]++;
			H.Red[Imagedata[row][col][2]]++;
			total++;
		}
	}
	if (!(file=fopen(argv[2],"w"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	int n = 0;

	// print Blue Histogram values
	for(n = 0;n < 256; n++){
		fprintf(file,"%d\n",H.Blue[n]);
	}
	fprintf(file,"\n");
	// print Green Histogram values
	for(n = 0;n < 256; n++){
		fprintf(file,"%d\n",H.Green[n]);
	}
	fprintf(file,"\n");
	// print Red Histogram values
	for(n = 0;n < 256; n++){
		fprintf(file,"%d\n",H.Red[n]);
	}

	fclose(file);

	//cout << "total pixels for each channel:" << total << endl;
	//total = 225600

	return 0;
}


