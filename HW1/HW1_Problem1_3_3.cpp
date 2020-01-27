//EE569 Howework Assignment #1
//Date:		Janary 13, 2019
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Compiled on MAC cmd with g++
// >>$ g++ HW1_Problem1_3_3.cpp -o HW1_Problem1_3_3
// >>$ ./HW1_Problem1_3_3 Toy.raw HistogramMethodB.txt HistToyMethodB.raw
/*-----------------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <unordered_map>
#include <stack>
#include <queue>


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
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_HistogramValue.txt Output_Histogram.raw" << endl;
		return 0;
	}

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
	int n = 0;
	int i = 0;
	int bucket_size = Height * Width / 256;

	//push, pop, top,
	
	unordered_map <int, stack < pair <int, int> > > blue_map;
	queue < pair<int, int> > blue_temp;

	unordered_map <int, stack < pair <int, int> > > green_map;
	queue < pair<int, int> > green_temp;

	unordered_map <int, stack < pair <int, int> > > red_map;
	queue < pair<int, int> > red_temp;
	
	for (col = 0; col < Width; col++) {
		for(row = 0; row < Height; row++) {
			blue_map[ Imagedata[row][col][0] ].push(pair<int, int>(row,col));
			green_map[ Imagedata[row][col][1] ].push(pair<int, int>(row,col));
			red_map[ Imagedata[row][col][2] ].push(pair<int, int>(row,col));
		}
	} 

	// store index contents with bucket over 875 pixels into a temp queue
	for(n = 0; n < 256; n++) {
		while(blue_map[n].size() > bucket_size) {
			blue_temp.push(blue_map[n].top());
	 		blue_map[n].pop();
	 	}
	 	while(green_map[n].size() > bucket_size) {
	 		green_temp.push(green_map[n].top());
	 		green_map[n].pop();
	 	}
	 	while(red_map[n].size() > bucket_size) {
	 		red_temp.push(red_map[n].top());
	 		red_map[n].pop();
	 	}
	}

	// push back index contents to the bucket less than 875 pixel to map
	for(n = 0; n < 256; n++) {
	   	while(blue_map[n].size() < bucket_size) {
	   		blue_map[n].push(blue_temp.front());
	  		blue_temp.pop();
	  	}
	  	while(green_map[n].size() < bucket_size) {
	  		green_map[n].push(green_temp.front());
	  		green_temp.pop();
	  	}
	  	while(red_map[n].size() < bucket_size) {
	  		red_map[n].push(red_temp.front());
	  		red_temp.pop();
	  	}

	}

	for(n = 0; n < 256; n++) {
		while(blue_map[n].size() > 0) {
			// fill pixels with value
			row = blue_map[n].top().first;
			col = blue_map[n].top().second;
			Hist_Imagedata[row][col][0] = n;
			blue_map[n].pop();

			row = green_map[n].top().first;
			col = green_map[n].top().second;
			Hist_Imagedata[row][col][1] = n;
			green_map[n].pop();

			row = red_map[n].top().first;
			col = red_map[n].top().second;
			Hist_Imagedata[row][col][2] = n;
			red_map[n].pop();
		}
	}

	for(col = 0; col < Width; col++) {
		for(row = 0; row < Height; row++) {
			H.Blue[Hist_Imagedata[row][col][0]]++;	
			H.Green[Hist_Imagedata[row][col][1]]++;
			H.Red[Hist_Imagedata[row][col][2]]++;
			
		}
	}

	if (!(file=fopen(argv[2],"w"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	//print Blue Histogram values
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


	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Hist_Imagedata, sizeof(unsigned char), Width * Height * RGB_BytePerPixel, file);
	fclose(file);


	return 0;
}


