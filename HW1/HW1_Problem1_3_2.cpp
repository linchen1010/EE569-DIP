//EE569 Howework Assignment #1
//Date:		Janary 13, 2019
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Compiled on MAC cmd with g++
// >>$ g++ HW1_Problem1_3_2.cpp HW1_Problem1_3_2
// >>$ ./HW1_Problem1_3_2 Toy.raw CumHistogram.txt HistToy.raw
/*-----------------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

struct Histogram {
	double Red[256];
	double Green[256];
	double Blue[256];
	// cumlative probability
	double CumProRed[256];
	double CumProGreen[256]; 
	double CumProBlue[256];
	// cumlative probability Value 
	unsigned char CpValueRed[256];
	unsigned char CpValueGreen[256];
	unsigned char CpValueBlue[256];

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
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_HistogramValue.txt Output_Histogram.raw" << endl;
		return 0;
	}
	
	/*
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
	}*/

	unsigned char Imagedata[Height][Width][RGB_BytePerPixel];
	unsigned char Hist_Imagedata[Height][Width][RGB_BytePerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*RGB_BytePerPixel, file);
	fclose(file);
	///////////////////////////////////////////////////////
	int col = 0;
	int row = 0;
	double totalPixel = 0;

	for(col = 0; col < Width; col++) {
		for(row = 0; row < Height; row++) {
			H.Blue[Imagedata[row][col][0]]++;	
			H.Green[Imagedata[row][col][1]]++;
			H.Red[Imagedata[row][col][2]]++;
			totalPixel++;
		}
	}
	if (!(file=fopen(argv[2],"w"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	int n = 0;
	double cumHistogramBlue = 0;
	double cumHistogramGreen = 0;
	double cumHistogramRed = 0;

	// print Blue Histogram values
	for(n = 0; n < 256; n++){
		cumHistogramBlue += H.Blue[n];
		H.CumProBlue[n] = cumHistogramBlue / totalPixel;
		H.CpValueBlue[n] = floor(255*H.CumProBlue[n]);
		fprintf(file,"%.1f\n",cumHistogramBlue);
	}
	fprintf(file,"\n");

	// print Green Histogram values
	for(n = 0; n < 256; n++){
		cumHistogramGreen += H.Green[n];
		H.CumProGreen[n] = cumHistogramGreen / totalPixel;
		H.CpValueGreen[n] = floor(255*H.CumProGreen[n]);
		fprintf(file,"%.1f\n",cumHistogramGreen);
	}
	fprintf(file,"\n");

	// print Red Histogram values
	for(n = 0; n < 256; n++){
		cumHistogramRed += H.Red[n];
		H.CumProRed[n] = cumHistogramRed / totalPixel;
		H.CpValueRed[n] = floor(255*H.CumProRed[n]);
		fprintf(file,"%.1f\n",cumHistogramRed);
	}

	fclose(file);

	for(col = 0; col < Width; col++) {
		for(row = 0; row < Height; row++) {
			Hist_Imagedata[row][col][0] = H.CpValueBlue[Imagedata[row][col][0]];
			Hist_Imagedata[row][col][1] = H.CpValueGreen[Imagedata[row][col][1]];
			Hist_Imagedata[row][col][2] = H.CpValueRed[Imagedata[row][col][2]];
		}
	}

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Hist_Imagedata, sizeof(unsigned char), Width * Height * RGB_BytePerPixel, file);
	fclose(file);

	//cout << "total pixels for each channel:" << total << endl;
	//total = 224000

	return 0;
}


