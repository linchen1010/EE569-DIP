//EE569 Howework Assignment #3
//Date:		February 23 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ -std=c++11 Star.cpp -o Star
// >> ./Star stars.raw starCount.raw
/*---------------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <map>

unsigned char findMin(std::set<unsigned char> my_set);

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 1; // Greyscale
    int Width = 640;
    int Height = 480;
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
    unsigned char ImagedataBinary[Height][Width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

     for(row = 0; row < Height ; row++) {
        for(col = 0; col < Width; col++) {
            if(Imagedata[row][col][0] < 128) {
                ImagedataBinary[row][col][0] = 0;
            }
            else {
                ImagedataBinary[row][col][0] = 1;
            }
        }
    }
    int label = 2;

    std::vector<unsigned char> L_Filter;
    int i = 0;
    int j = 0;

    std::map<unsigned char,std::set<unsigned char>> LabelEq;

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            if(ImagedataBinary[row][col][0] != 0) {
                L_Filter.push_back(ImagedataBinary[row-1][col-1][0]);
                L_Filter.push_back(ImagedataBinary[row-1][col][0]);
                L_Filter.push_back(ImagedataBinary[row-1][col+1][0]);
                L_Filter.push_back(ImagedataBinary[row][col-1][0]);
                std::sort(L_Filter.begin(),L_Filter.end());
                if(L_Filter[3] == 0) { // L_filter are all background(0), assign label to current pixel
                    ImagedataBinary[row][col][0] = label;
                    label++;
                }
                else {
                    while(i<4) {
                        if(L_Filter[i] == 0) { // find the min label which is not background
                            i++;
                        }
                        else {
                            ImagedataBinary[row][col][0] = L_Filter[i];
                            i = 0;
                            break;
                        }
                    }
                }
                L_Filter.clear();
            }
        }
    }

    // connected componet labeling 
    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            if(ImagedataBinary[row][col][0] != 0) {
                for(i = -1; i < 2; i++) {
                    for(j = -1; j < 2; j++) {
                        if(ImagedataBinary[row+i][col+j][0] != 0) {
                            LabelEq[ImagedataBinary[row][col][0]].insert(ImagedataBinary[row+i][col+j][0]); 
                        }
                    }
                }
            }
        }
    }

    std::set<unsigned char> starCount;

    // for(auto it = LabelEq.begin(); it != LabelEq.end(); ++it)   {
    //     std::cout << int(it->first) <<  ": ";
    //     for(auto set_it = it->second.begin(); set_it != it->second.end(); ++set_it) {
    //         std::cout << int(*set_it) << " ";
    //     }
    //      std::cout << "\n";
    // } 

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            if(ImagedataBinary[row][col][0]!=0)
            ImagedataBinary[row][col][0] = findMin(LabelEq[ImagedataBinary[row][col][0]]);
        }
    }

    int starFreq[256] = {0};

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            if(ImagedataBinary[row][col][0]!=0) {
            starFreq[int(ImagedataBinary[row][col][0])]++;
            }
        }
    }

    int cnt = 0;

    for(i = 0; i < 255; i++) {
        if(starFreq[i] > 1) {
            //std::cout << i << ": " << starFreq[i] << std::endl;
            starCount.insert(starFreq[i]);
            cnt++;
        }
    }

    int tempCnt = 0;

    std::map<int, int> StarSizeFreq;

    for(i = 0; i < 255; i++) {
        if(starFreq[i]!=0)
        StarSizeFreq[starFreq[i]]++;
    }

    std::cout << "starCount: "<< cnt << std::endl;
    std::cout << "Different sizes: " << starCount.size() << std::endl;
    
    for(auto it = StarSizeFreq.begin(); it != StarSizeFreq.end(); it++) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataBinary, sizeof(unsigned char), Width * Height * BytesPerPixel, file);
	fclose(file);

	return 0;


}

unsigned char findMin(std::set<unsigned char> my_set) 
{ 
  
    // Get the minimum element 
    unsigned char min_element; 
    if (!my_set.empty()) 
        min_element = *my_set.begin(); 
  
    // return the minimum element 
    return min_element; 
} 