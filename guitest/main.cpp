#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // surely i dont need all these
#include <vector>
#include <chrono>
#include <math.h>
#include <fstream>

#include "texture-generation.h"
#include "cbmp.h"


using namespace std;

struct UV {
    double x;
    double y;
};

struct vec2 {
    int x;
    int y;
};


double normalize(double uvCoordx, double uvCoordy, int row, int col, double aspectRatio){
    struct UV uvCoords;
        uvCoords.x = uvCoordx;
        uvCoords.y = uvCoordy;
    
    if (uvCoordy > -1) {
        uvCoords.y /= float(row);
        uvCoords.y = uvCoords.y * 2 - 1;
        return uvCoords.y;
    }
    
    if (uvCoordx > -1) {
        uvCoords.x /= float(col);
        uvCoords.x = uvCoords.x * 2 - 1;
        uvCoords.x *= aspectRatio;
        return uvCoords.x;
    }
    return  -1;
    
}

int scale(int value, int initMin, int initMax, int newMin, int newMax){
    //implrement
    return 0;
}

int main() {
    bool testing = false;
    
    struct winsize ws;
    
    if( ioctl( 0, TIOCGWINSZ, &ws ) != 0 ){
        fprintf(stderr, "TIOCGWINSZ:%s\n", strerror(errno));
        exit(1);
    }
    
    bool running = true;
    int row = ws.ws_row;
    int col = ws.ws_col;
    
    BMP *bmpImage = bopen("/Users/syro/Desktop/xcode/guitest/guitest/AFX.bmp");
  
    const int width = get_width(bmpImage);
    const int height = get_height(bmpImage);
    
    
    pixelData pixDat[width][height];

    vector< vector<uint8_t> > xy(width,std::vector<uint8_t>(height));
    std::vector<std::vector<pixelData>> pixDatVec(width,std::vector<pixelData>(height));
    
    
    unsigned char r;
    unsigned char g;
    unsigned char b;
    int index = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            index++;
            int x = i;
            int y = j;
            int index = y * bmpImage->width + x;
            r = bmpImage->pixels[index].red;
            g = bmpImage->pixels[index].green;
            b = bmpImage->pixels[index].blue;
            pixDat[i][j].r = r;
            pixDat[i][j].g = g;
            pixDat[i][j].b = b;
            pixDatVec[i][j].r = r;
            pixDatVec[i][j].g = g;
            pixDatVec[i][j].b = b;
        }
    }
    
    /* testing \/ */
    if ((row == 0) && (col == 0)) {
        row = 24;
        col = 80;
        testing = true;
    }
    
   
    int frameCount = 0;
    int mult;
    
    int fps = 5;  // at 200000000 nano seconds per cycle;
        fps = 20; // at 50000000;
    
    
    while (running) {
        frameCount++;
        mult = frameCount%20;
        std::string frame;
        
        if( ioctl( 0, TIOCGWINSZ, &ws ) != 0 ){
            fprintf(stderr, "TIOCGWINSZ:%s\n", strerror(errno));
            exit(1);
        }
        row = ws.ws_row;
        col = ws.ws_col;
        
        
        int xPix = ws.ws_xpixel;
        int yPix = ws.ws_ypixel;

        double aspectRatio = double(xPix)/double(yPix);
        
        
        if ((row == 0) && (col == 0)) {
            row = 24;
            col = 80;
        }
      
        UV scale;
        scale.x = double(xPix)/double(width);
        scale.y = double(yPix)/double(height);
   
        for (int i = 0; i < row*2; i+=2) { // height (a)
            for (int j = 0; j < col; j++) { // width

            }
            frame += '\n';
        }
        
        std::string frameGen = bitMapView(row, col, height, width, pixDatVec);
        
        std::cout << frameGen;
        
        //cout << "\033[1;32mbold red text\033[0m\n";
        
            destroy(frameGen.begin(), frameGen.end());
            
            struct timespec tim;
            tim.tv_nsec = 50000000;
            
            nanosleep(&tim, NULL);
     
        
    }

  return 0 ;

}
