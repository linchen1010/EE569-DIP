//EE569 Howework Assignment #1
//Date:		Janary 13, 2019
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Compiled on MAC cmd with g++
// >> g++ HW1_Problem2_b.cpp -o HW1_Problem2_b
// >> ./HW1_Problem2_a Corn_noisy.raw Corn_bilateral.raw

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;
const int Width = 320;
const int Height = 320;
double sigma_c = 0.05;
double sigma_s = 4;

double window (unsigned char image[Height][Width], int i, int j, int k, int l, double sigma_s, double sigma_r) {

 	double result = exp ( - ((pow(i-k,2)+pow(j-l,2)) / (2*pow(sigma_s,2))) - pow(abs(image[i][j]-image[k][l]),2) / (2*pow(sigma_r,2)));
 	return result;
 }

double myfunct (double a, double b) {

	return a;
}

int main(int argc, char *argv[])

{ 
	// Define file pointer and variables
	FILE *file;
	FILE *file2;
	int BytesPerPixel = 1;
	//int Width = 320;
	//int Height = 320;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw Corn_bilateral.raw" << endl;
		return 0;
	}

	// Allocate image data array **
	unsigned char Imagedata [Height][Width];
	unsigned char DenoiseImagedata [Height][Width]; // Image with  Uniform weight function filtering 
	unsigned char Denoise2Imagedata [Height][Width][BytesPerPixel]; // Image with Gaussian weight function filtering
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	//fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

	int row = 0;
	int col = 0;
	int n = 0;
	// s = 2 // r = 50
	double sigma_s = 9;
	double sigma_r = 50;
	double temp_w = 0;
	double tempImageValue = 0;
	double temp = 0;

	int i, j, k, l;
	
	
	for(row = 0; row < Height; row++) {
		for(col = 0; col < Width; col++) {
			temp_w = 0;
			tempImageValue = 0;
			for(n = -1; n < 2; n++) { // filter
				i = row;
				j = col;
				temp_w += window(Imagedata, i, j, row-1, col+n,sigma_s,sigma_r);
				temp_w += window(Imagedata, i, j, row, col+n,sigma_s,sigma_r);
				temp_w += window(Imagedata, i, j, row+1, col+n,sigma_s,sigma_r);

				tempImageValue += Imagedata[row-1][col+n] * window(Imagedata, i, j,row-1, col+n, sigma_s, sigma_r) ;
				tempImageValue += Imagedata[row][col+n] * window(Imagedata, i, j, row, col+n, sigma_s, sigma_r) ;
				tempImageValue += Imagedata[row+1][col+n] * window(Imagedata, i, j, row+1, col+n, sigma_s, sigma_r) ;
			}
			DenoiseImagedata[row][col] = tempImageValue/temp_w;
			temp = tempImageValue/temp_w;
			if(temp > 255) {
				cout << temp << endl;
			}
		}

	}
	


	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(DenoiseImagedata, sizeof(unsigned char), Width * Height * BytesPerPixel, file);
	

	fclose(file);

	return 0;
}