//EE569 Howework Assignment #1
//Date:		Janary 13, 2019
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Compiled on MAC cmd with g++
// >> g++ HW1_Problem1.cpp HW1_Problem1
// >> ./HW1_Problem1 Dog.raw DemosaicingDog.raw
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

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 256;
	int Width = 600;
	int Height = 532;
	int RGB_BytePerPixel = 3;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
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
	unsigned char ImagedataOriginal[Height][Width][BytesPerPixel];
	unsigned char Imagedata[Height+4][Width+4][BytesPerPixel];
	unsigned char Demo_Imagedata[Height][Width][RGB_BytePerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	//fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fread(ImagedataOriginal, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	int row = 0;
	int col = 0;

	/* Mirror reflecting -- Image boundary padding */
	for(col = 0; col < Width; col++) {
		for(row = 0; row < Height; row++) {
			Imagedata[row+2][col+2][0] = ImagedataOriginal[row][col][0];
		}
	}

	for(row = 2; row < Height; row++) {
		Imagedata[row][0][0] = ImagedataOriginal[row-2][0][0];
		Imagedata[row][1][0] = ImagedataOriginal[row-2][1][0];
		Imagedata[row][Width+3][0] = ImagedataOriginal[row-2][Width-1][0];
		Imagedata[row][Width+2][0] = ImagedataOriginal[row-2][Width-2][0];
	}

	for(col = 2; col < Width; col++) {
		Imagedata[0][col][0] = ImagedataOriginal[0][col-2][0];
		Imagedata[1][col][0] = ImagedataOriginal[1][col-2][0];
		Imagedata[Height+3][col][0] = ImagedataOriginal[Height-1][col-2][0];
		Imagedata[Height+2][col][0] = ImagedataOriginal[Height-2][col-2][0];
	}

  /******************************************************************
    partten: 1. GRG //all G1 values are at even row and even column
  				BGB
     		    GRG

			 2. GBG //all G2 values are at odd row and odd column
			 	RGR
			 	GBG

			 3. RGR //all B values are at even row and odd column
			 	GBG
			 	RGR

			 4. BGB //all R values are at odd row and even column
			 	GRG
			 	BGB
   ******************************************************************/

	/*for col
		for row
			switch _case // RGB pattern //
			see which pattern
		case 1 R G B value */
	int _case = 0;

	for(col = 2; col < Width+2; col++) {
		for(row = 2; row < Height+2; row++) {

			if(row % 2 == 0 & col % 2 == 0) {
				_case = 1;
			}
			else if(row % 2 == 1 & col % 2 == 1) {
				_case = 2;
			}
			else if(row % 2 == 0 & col % 2 == 1) {
				_case = 3;
			}
			else {
				_case = 4;
			}

			// inital RGB values
			unsigned char R;
			unsigned char G;
			unsigned char B;

			switch(_case) {
				case 1:
				{	
					//getting R values
					unsigned char R1 = Imagedata[row-1][col][0];
					unsigned char R2 = Imagedata[row+1][col][0];

					//getting B values
					unsigned char B1 = Imagedata[row][col-1][0];
					unsigned char B2 = Imagedata[row][col+1][0];

					//calculate R, G, B value
					R = (R1 + R2) / 2;
					B = (B1 + B2) / 2;
					G = Imagedata[row][col][0];
				} break;

				case 2:
				{
					//getting R values
					unsigned char R1 = Imagedata[row][col-1][0];
					unsigned char R2 = Imagedata[row][col+1][0];

					//getting B values
					unsigned char B1 = Imagedata[row-1][col][0];
					unsigned char B2 = Imagedata[row+1][col][0];

					R = (R1 + R2) / 2;
					B = (B1 + B2) / 2;
					G = Imagedata[row][col][0];
				} break;

				case 3:
				{
					//getting R values
					unsigned char R1 = Imagedata[row-1][col-1][0];
					unsigned char R2 = Imagedata[row-1][col+1][0];
					unsigned char R3 = Imagedata[row+1][col-1][0];
					unsigned char R4 = Imagedata[row+1][col+1][0];

					//getting G values
					unsigned char G1 = Imagedata[row-1][col][0];
					unsigned char G2 = Imagedata[row+1][col][0];
					unsigned char G3 = Imagedata[row][col-1][0];
					unsigned char G4 = Imagedata[row][col+1][0];

					R = (R1 + R2 + R3 + R4) / 4;
					G = (G1 + G2 + G3 + G4) / 4;
					B = Imagedata[row][col][0];

				} break;

				case 4:
				{
					//getting B values
					unsigned char B1 = Imagedata[row-1][col-1][0];
					unsigned char B2 = Imagedata[row-1][col+1][0];
					unsigned char B3 = Imagedata[row+1][col-1][0];
					unsigned char B4 = Imagedata[row+1][col+1][0];

					//getting G values
					unsigned char G1 = Imagedata[row-1][col][0];
					unsigned char G2 = Imagedata[row+1][col][0];
					unsigned char G3 = Imagedata[row][col-1][0];
					unsigned char G4 = Imagedata[row][col+1][0];

					G = (G1 + G2 + G3 + G4) / 4;
					B = (B1 + B2 + B3 + B4) / 4;
					R = Imagedata[row][col][0];

				} break;
			}
				//Store RGB value in New Array with size of 600*532*3

				Demo_Imagedata[row-2][col-2][0] = B;
				Demo_Imagedata[row-2][col-2][1] = G;
				Demo_Imagedata[row-2][col-2][2] = R;
		}
			
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Demo_Imagedata, sizeof(unsigned char), Width * Height * RGB_BytePerPixel, file);
	fclose(file);

	return 0;
}


