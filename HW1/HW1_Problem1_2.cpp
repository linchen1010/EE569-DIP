//EE569 Howework Assignment #1
//Date:		Janary 13, 2019
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Compiled on MAC cmd with g++
// >> g++ HW1_Problem1_2.cpp HW1_Problem1_2
// >> ./HW1_Problem1_2 Dog.raw Dog_MHC.raw
/*-----------------------------------------*/

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
	unsigned char Imagedata[Height][Width][BytesPerPixel];
	unsigned char Demo_Imagedata[Height][Width][RGB_BytePerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	//fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	int row = 0;
	int col = 0;
	// weight factor
	double a = 0.5;
	double b = 0.625;
	double r = 0.75;


  /******************************************************************
    pattern  1. GRG //all G1 values are at even row and even column 
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
   *******************************************************************/

	/*for col
		for row
			switch _case // RGB pattern //
			see which pattern
		case 1 R G B value */
	int _case = 0;

	for(col = 0; col < Width; col++) {
		//cout << b << endl;
		for(row = 0; row < Height; row++) {

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

			// inital RGB & delta_RGB values
			unsigned char R;
			unsigned char G;
			unsigned char B;
			double R_MHC;
			double G_MHC;
			double B_MHC;

			switch(_case) {
				case 1: //BGB
				{	
					// getting R values
					unsigned char R1 = Imagedata[row-1][col][0];
					unsigned char R2 = Imagedata[row+1][col][0];

					// getting B values
					unsigned char B1 = Imagedata[row][col-1][0];
					unsigned char B2 = Imagedata[row][col+1][0];

					unsigned char G1 = Imagedata[row-1][col-1][0];
					unsigned char G2 = Imagedata[row-1][col+1][0];
					unsigned char G3 = Imagedata[row+1][col-1][0];
					unsigned char G4 = Imagedata[row+1][col+1][0];
					unsigned char G5 = Imagedata[row][col-2][0];
					unsigned char G6 = Imagedata[row][col+2][0];
					unsigned char G7 = Imagedata[row-2][col][0];
					unsigned char G8 = Imagedata[row+2][col][0];

					// calculate R, G, B value
					R = (R1 + R2) / 2;
					B = (B1 + B2) / 2;
					G = Imagedata[row][col][0];

					// red component at green pixel
					R_MHC = 0.125 * ( 4 * (R1+R2) + 5 * G - (G1+G2+G3+G4-0.5*G5-0.5*G6+G7+G8));
					// blue componet at green pixel
					B_MHC = 0.125 * ( 4 * (B1+B2) + 5 * G - (G1+G2+G3+G4+G5+G6-0.5*G7-0.5*G8));
					G_MHC = G;

				} break;

				case 2: //RGR
				{
					//getting R values
					unsigned char R1 = Imagedata[row][col-1][0];
					unsigned char R2 = Imagedata[row][col+1][0];

					//getting B values
					unsigned char B1 = Imagedata[row-1][col][0];
					unsigned char B2 = Imagedata[row+1][col][0];

					unsigned char G1 = Imagedata[row-1][col-1][0];
					unsigned char G2 = Imagedata[row-1][col+1][0];
					unsigned char G3 = Imagedata[row+1][col-1][0];
					unsigned char G4 = Imagedata[row+1][col+1][0];
					unsigned char G5 = Imagedata[row][col-2][0];
					unsigned char G6 = Imagedata[row][col+2][0];
					unsigned char G7 = Imagedata[row-2][col][0];
					unsigned char G8 = Imagedata[row+2][col][0];

					R = (R1 + R2) / 2;
					B = (B1 + B2) / 2;
					G = Imagedata[row][col][0];

					// red component at green pixel 
					R_MHC = 0.125 * ( 4 * (R1+R2) + 5 * G - (G1+G2+G3+G4+G5+G6-0.5*G7-0.5*G8));
					// blue componet at green pixel
					B_MHC = 0.125 * ( 4 * (B1+B2) + 5 * G - (G1+G2+G3+G4-0.5*G5-0.5*G6+G7+G8));
					G_MHC = G;

				} break;

				case 3: //GBG
				{
					// getting R values
					unsigned char R1 = Imagedata[row-1][col-1][0];
					unsigned char R2 = Imagedata[row-1][col+1][0];
					unsigned char R3 = Imagedata[row+1][col-1][0];
					unsigned char R4 = Imagedata[row+1][col+1][0];

					// getting G values
					unsigned char G1 = Imagedata[row-1][col][0];
					unsigned char G2 = Imagedata[row+1][col][0];
					unsigned char G3 = Imagedata[row][col-1][0];
					unsigned char G4 = Imagedata[row][col+1][0];

					unsigned char B1 = Imagedata[row-2][col][0];
					unsigned char B2 = Imagedata[row+2][col][0];
					unsigned char B3 = Imagedata[row][col-2][0];
					unsigned char B4 = Imagedata[row][col+2][0];

					R = (R1 + R2 + R3 + R4) / 4;
					G = (G1 + G2 + G3 + G4) / 4;
					B = Imagedata[row][col][0];

					// green component at blue pixel
					G_MHC = 0.125 * ( 2 * (G1+G2+G3+G4) + 4 * B - (B1+B2+B3+B4));
					// red component at blue pixel
					R_MHC = 0.125 * ( 2 * (R1+R2+R3+R4) + 6 * B - 1.5 * (B1+B2+B3+B4));
					B_MHC = B;

				} break;

				case 4: //GRG
				{
					// getting B values
					unsigned char B1 = Imagedata[row-1][col-1][0];
					unsigned char B2 = Imagedata[row-1][col+1][0];
					unsigned char B3 = Imagedata[row+1][col-1][0];
					unsigned char B4 = Imagedata[row+1][col+1][0];

					// getting G values
					unsigned char G1 = Imagedata[row-1][col][0];
					unsigned char G2 = Imagedata[row+1][col][0];
					unsigned char G3 = Imagedata[row][col-1][0];
					unsigned char G4 = Imagedata[row][col+1][0];

					// getting R values
					unsigned char R1 = Imagedata[row-2][col][0];
					unsigned char R2 = Imagedata[row+2][col][0];
					unsigned char R3 = Imagedata[row][col-2][0];
					unsigned char R4 = Imagedata[row][col+2][0];

					G = (G1 + G2 + G3 + G4) / 4;
					B = (B1 + B2 + B3 + B4) / 4;
					R = Imagedata[row][col][0];

					// green componet at rex piexl
					G_MHC = 0.125 * ( 2 * (G1+G2+G3+G4) + 4 * R - (R1+R2+R3+R4));
					// blue componet at red pixel
					B_MHC = 0.125 * ( 2 * (B1+B2+B3+B4) + 6 * R - 1.5 * (R1+R2+R3+R4));
					R_MHC = R;

				} break;
			}	
			// handle pixel points with overflow value
				if (B_MHC < 0) {
					B_MHC = 0;
				}
				else if (B_MHC > 255) {
					B_MHC = 255;
				}
				if (G_MHC < 0) {
					G_MHC = 0;
				}
				else if (G_MHC > 255) {
					G_MHC = 255;
				}
				if (R_MHC < 0) {
					R_MHC = 0;
				}
				else if (R_MHC > 255) {
					R_MHC = 255;
				}	
			// Store RGB value in Demosaiced Imagedata with size of 600*532*3
				Demo_Imagedata[row][col][0] = B_MHC ;  
				Demo_Imagedata[row][col][1] = G_MHC ; 
				Demo_Imagedata[row][col][2] = R_MHC ;
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


