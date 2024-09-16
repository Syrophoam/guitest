#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
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

struct pixelData{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

const char* grayScale[] = {"$","@","B","%","8","&","W","M","#","*","o","a","h","k","b","d","p","q","w","m","Z","O","0","Q","L","C","J","U","Y","X","z","c","v","u","n","x","r","j","f","t","/","|","(",")","1","{","}","[","]","?","-","_","+","~","<",">","i","!","l","I",";",":",",","^","`","."," "};

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
    
    BMP *bmpImage = bopen(("/Users/syro/Desktop/xcode/guitest/guitest/checkGimp.bmp"));
  
    const int width = get_width(bmpImage);
    const int height = get_height(bmpImage);
    
    
    pixelData pixDat[width*height];
    
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
            pixDat[index].r = r;
            pixDat[index].g = g;
            pixDat[index].b = b;
        }
    }
    
    struct UV UVcoords;
    struct vec2 intensity;
    
    /* testing \/ */
    if ((row == 0) && (col == 0)) {
        row = 24;
        col = 80;
        testing = true;
    }
    
   
    int frameCount = 0;
    int mult;
    int pixel;
    int fps = 5;  // at 200000000 nano seconds per cycle;
        fps = 20; // at 50000000;
    int frame = 0;
    
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
        
        float animation = 0;
        if(frameCount > 50){
            
            destroy(frame.begin(), frame.end());
            std::string frame;
            for (int k = 0; k < row; k++) {
                for (int l = 0; l < col; l++) {
                    frame += "@";
                }
            }
            std::string name;
            std::cout << "Name?" << '\n';
            std::cin >> name;
            if (name == "World") {
                std::cout << "Hello World";
                return 1;
            }
        }else{
            
            for (int i = 0; i < row; i++) {
                UVcoords.y = double(i);
                UVcoords.y = normalize(-1,UVcoords.y,row, col, aspectRatio);

                for (int j = 0; j < col; j++) {
                    UVcoords.x = double(j);
                    UVcoords.x = normalize(UVcoords.x,-1,row, col, aspectRatio);
                    
                    float length = sqrt(pow(UVcoords.x,2)+pow(UVcoords.y,2));
                    animation = sin((length * M_2_PI * 3) + float(frameCount)/100);
                    animation = animation * 0.5 + 0.5;
                    animation *= 66;
                    
                    
                    animation *= double(abs(frameCount-50))/50;
                    
                    if(frameCount > 50){
                        destroy(frame.begin(), frame.end());
                        std::string frame;
                        for (int k = 0; k < row; k++) {
                            for (int l = 0; l < col; l++) {
                                frame += "@";
                            }
                        }
                    }else{
                        frame += grayScale[ int(animation)%66 ];
                    }
                    
                }
                frame += "\n";
            }
            
            std::cout << frame;
            destroy(frame.begin(), frame.end());
            
            struct timespec tim;
            tim.tv_nsec = 50000000;
            
            nanosleep(&tim, NULL);
     
        }
    }

  return 0 ;

}
