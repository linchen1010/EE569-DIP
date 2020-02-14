//EE569 Howework Assignment #2
//Date:		February 07 , 2020
//Name: 	Shi-Lin Chen
//ID: 		2991-9119-97
//Email:	shilinch@usc.edu     
//Submission Date:  
//Compiled on MAC cmd with g++
// Usuage:
// >> g++ MVBQ.cpp -o MVBQ
// >> .MVBQ Rose.raw RoseMVBQ.raw
/*--------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>

std::string MBVQ(unsigned char R, unsigned char G, unsigned char B);
void vertex(std::string mbvq, unsigned char &R, unsigned char &G, unsigned char &B);

int main(int argc, char *argv[]) {
    FILE *file;
    int BytesPerPixel = 3; // Gray-scale
    int Width = 640;
    int Height = 480;
	

    // Check for proper syntax
	if (argc < 2){
		std::cout << "Syntax Error - Incorrect Parameter Usage:" << std::endl;
		std::cout << "program_name input_image.raw output_image.raw" << std::endl;
		return 0;
	}
    // Allocate image data array **
	unsigned char Imagedata[Height][Width][BytesPerPixel];
    unsigned char ImagedataFS[Height][Width][BytesPerPixel];
    unsigned char ImagedataFSResult[Height][Width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		std::cout << "Cannot open file: " << argv[1] << std::endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Width*Height*BytesPerPixel, file);
	fclose(file);

    int row = 0;
    int col = 0;
    unsigned char threshold = 128;
    double errorB = 0;
    double errorG = 0;
    double errorR = 0;
    std::string temp;

    unsigned char tempR;
    unsigned char tempG;
    unsigned char tempB;

    for(row = 0; row < Height; row++) {
        for(col = 0; col < Width; col++) {
            ImagedataFS[row][col][0] = Imagedata[row][col][0];
            ImagedataFS[row][col][1] = Imagedata[row][col][1];
            ImagedataFS[row][col][2] = Imagedata[row][col][2];
        }
    }

    for(row = 0; row < Height; row++) {
        // serpentine scanning
        if(row%2 == 0) { // ---->
            for(col = 0; col < Width; col++) {
                tempR = ImagedataFS[row][col][0];
                tempG = ImagedataFS[row][col][1];
                tempB = ImagedataFS[row][col][2];

                temp = MBVQ(ImagedataFS[row][col][0], ImagedataFS[row][col][1], ImagedataFS[row][col][2]);
                vertex(temp,ImagedataFS[row][col][0], ImagedataFS[row][col][1], ImagedataFS[row][col][2]);
                errorR = tempR - ImagedataFS[row][col][0];
                errorG = tempG - ImagedataFS[row][col][1];
                errorB = tempB - ImagedataFS[row][col][2]; 

                ImagedataFS[row][col+1][0] += floor(errorR * 7/16.0);
                ImagedataFS[row][col+1][1] += floor(errorG * 7/16.0);
                ImagedataFS[row][col+1][2] += floor(errorB * 7/16.0);
                ImagedataFS[row+1][col-1][0] += floor(errorR * 3/16.0);
                ImagedataFS[row+1][col-1][1] += floor(errorG * 3/16.0);
                ImagedataFS[row+1][col-1][2] += floor(errorB * 3/16.0);
                ImagedataFS[row+1][col][0] += floor(errorR * 5/16.0);
                ImagedataFS[row+1][col][1] += floor(errorG * 5/16.0);
                ImagedataFS[row+1][col][2] += floor(errorB * 5/16.0);
                ImagedataFS[row+1][col+1][0] += floor(errorR * 1/16.0);
                ImagedataFS[row+1][col+1][1] += floor(errorG * 1/16.0);
                ImagedataFS[row+1][col+1][2] += floor(errorB * 1/16.0);

                if(ImagedataFS[row][col][2]!=0&&ImagedataFS[row][col][2]!=255)
                std::cout << int(ImagedataFS[row][col][0]) << std::endl;
            }
            
        }
        else{   // <-----
            for(col = Width-1; col >= 0; col--) {
                tempR = ImagedataFS[row][col][0];
                tempG = ImagedataFS[row][col][1];
                tempB = ImagedataFS[row][col][2];

                temp = MBVQ(ImagedataFS[row][col][0], ImagedataFS[row][col][1], ImagedataFS[row][col][2]);
                vertex(temp,ImagedataFS[row][col][0], ImagedataFS[row][col][1], ImagedataFS[row][col][2]);
                
                errorR = tempR - ImagedataFS[row][col][0];
                errorG = tempG - ImagedataFS[row][col][1];
                errorB = tempB - ImagedataFS[row][col][2];
                
                ImagedataFS[row][col-1][0] += floor(errorR * 7/16.0);
                ImagedataFS[row][col-1][1] += floor(errorG * 7/16.0);
                ImagedataFS[row][col-1][2] += floor(errorB * 7/16.0);
                ImagedataFS[row+1][col+1][0] += floor(errorR * 3/16.0);
                ImagedataFS[row+1][col+1][1] += floor(errorG * 3/16.0);
                ImagedataFS[row+1][col+1][2] += floor(errorB * 3/16.0);
                ImagedataFS[row+1][col][0] += floor(errorR * 5/16.0);
                ImagedataFS[row+1][col][1] += floor(errorG * 5/16.0);
                ImagedataFS[row+1][col][2] += floor(errorB * 5/16.0);
                ImagedataFS[row+1][col-1][0] += floor(errorR * 1/16.0);
                ImagedataFS[row+1][col-1][1] += floor(errorG * 1/16.0);
                ImagedataFS[row+1][col-1][2] += floor(errorB * 1/16.0);
            }
        }   
    }
   
    // gray image
    if (!(file=fopen(argv[2],"wb"))) {
		std::cout << "Cannot open file: " << argv[2] << std::endl;
		exit(1);
	}
	fwrite(ImagedataFS, sizeof(unsigned char), Width * Height * BytesPerPixel, file);
	fclose(file);

    return 0;
}

std::string MBVQ(unsigned char R, unsigned char G, unsigned char B) {
    if((R+G) > 255) {
        if((G+B) > 255) {
            if((R+G+B) > 510) {
                return "CMYW";
            } else return "MYGC";
        } else return "RGMY";
    }
    else {
        if(!((G+B) > 255)) {
            if(!((R+G+B) > 255)) {
                return "KRGB";
            } else return "RGBM";    
        } else return "CMGB";
    }
}

void vertex(std::string mbvq, unsigned char &R, unsigned char &G, unsigned char &B) {
// No.1 for CMYW
    if (mbvq == "CMYW") {
        if (B < 128) {
            if (B <= R) {
                if (B <= G) {
                    R = 255; G =255; B = 0; 
                    return; //vertex = 'yellow';
                }
            }
        }       
        if (G < 128) {
            if (G <= B) {
                if (G <= R) {
                    R = 255; G = 0; B = 255; 
                    return;
                } //vertex = 'magenta';
            }
        }
        if (R < 128) {
            if (R <= B) {
                if (R <= G) {
                    R= 0; G = 255; B = 255; 
                    return;
                } //vertex = 'cyan';
            }
        }
        R = 255; G = 255; B = 255;  // white
    }


// No.2 for MYGC
    if (mbvq == "MYGC") {
        if (G >= B) {
            if (R >= B) {
                if (R >= 128) {
                    R = 255; G =255; B = 0; return;//vertex = 'yellow';
                }
                else {
                    R = 0; G = 255; B = 0; return;
                }//vertex = 'green';
            }
        }
        if (G >= R) {
            if (B >= R) {
                if (B >= 128) {
                    R= 0; G = 255; B = 255; return;
                } //vertex = 'cyan'; 
                else {
                    R = 0; G = 255; B = 0; return;
                } //vertex = 'green';
            }
        }
        R = 255; G = 0; B = 255; //vertex = 'magenta';
    }


// No.3 for RGMY
    if (mbvq == "RGMY") {
        if (B > 128) {
            if (R > 128) {
                if (B >= G) {
                    R = 255; G = 0; B = 255; return; //vertex = 'magenta';
                }
                else {
                    R = 255; G =255; B = 0; return; //vertex = 'yellow';
                }
            }
            else {
                if (G > (B + R)) {
                    R = 0; G = 255; B = 0; return; //vertex = 'green';
                }
                else {
                    R = 255; G = 0; B = 255; return; //vertex = 'magenta';
                } 
            }
        }
        else {
            if (R >= 128) {
                if (G >= 128)
                    {R = 255; G =255; B = 0; return;} //vertex = 'yellow';
                else
                    {R = 255; G = 0; B = 0; return;} //vertex = 'red';
            }
            else {
                if (R >= G)
                    {R = 255; G = 0; B = 0; return;} //vertex = 'red';
                else
                    {R = 0; G = 255; B = 0; return;} //vertex = 'green';
            
            }
        }
    }

// No.4 for KRGB
    if (mbvq == "KRGB") {
        if (B > 128) {
            if (B >= R) {
                if (B >= G)
                    {R = 0; G = 0; B = 255; return;}//vertex = 'blue';
            }
        }
        if (G > 128) {
            if (G >= B) {
                if (G >= R)
                    {R = 0; G = 255; B = 0; return;} //vertex = 'green';
            }
        }
        if (R > 128) {
            if (R >= B) {
                if (R >= G)
                    {R = 255; G = 0; B = 0; return;} //vertex = 'red';
            }
        }
        R = 0; G = 0; B = 0; //vertex = 'black';
    }


// No.5 for RGBM
    if (mbvq == "RGBM") {
        if (R > G) {
            if (R >= B) {
                if (B < 128)
                    {R = 255; G = 0; B = 0; return;} //vertex = 'red';
                else
                    {R = 255; G = 0; B = 255; return;} //vertex = 'magenta';
            }
        }
        if (B > G) {
            if (B >= R) {
                if (R < 128)
                   {R = 0; G = 0; B = 255; return;} //vertex = 'blue';
                else
                   {R = 255; G = 0; B = 255; return;} //vertex = 'magenta';
            }
        }
    R = 0; G = 255; B = 0; //vertex = 'green'}
    }


// No.6 for CMGB
    if (mbvq == "CMGB") {
        if (B > 128) {
            if ( R > 128) {
                if (G >= R)
                    {R= 0; G = 255; B = 255; return;} //vertex = 'cyan';
                else
                    {R = 255; G = 0; B = 255; return;} //vertex = 'magenta';
            }
            else {
                if (G > 128)
                    {R= 0; G = 255; B = 255; return;} //vertex = 'cyan';
                else
                    {R = 0; G = 0; B = 255; return;} //vertex = 'blue';
            }
            
        }
        else {
            if ( R > 128) {
                if ( (R - G + B) >= 128)
                    {R = 255; G = 0; B = 255; return;} //vertex = 'magenta';
                else
                    {R = 0; G = 255; B = 0; return;} //vertex = 'green';
            }
            else {
                if (G >= B)
                    {R = 0; G = 255; B = 0; return;} //vertex = 'green';
                else
                    {R = 0; G = 0; B = 255; return;} //vertex = 'blue';
            }
        }
    }
}