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
#include "bitmap_image.hpp"
#include "texture-generation.h"

using namespace std;

struct UV {
    double x;
    double y;
};

struct vec2 {
    int x;
    int y;
};

UV normalize(UV uvCoords, int row, int col, double aspectRatio){
    uvCoords.y /= float(row);
    uvCoords.y = uvCoords.y * 2 - 1;
    
    uvCoords.x /= float(col);
    uvCoords.x = uvCoords.x * 2 - 1;
    uvCoords.x *= aspectRatio; //if testing
    return uvCoords;
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
    
    const char* grayScale[] = {"$","@","B","%","8","&","W","M","#","*","o","a","h","k","b","d","p","q","w","m","Z","O","0","Q","L","C","J","U","Y","X","z","c","v","u","n","x","r","j","f","t","/","|","(",")","1","{","}","[","]","?","-","_","+","~","<",">","i","!","l","I",";",":",",","^","`","."," "};

    struct UV UVcoords;
    struct vec2 intensity;
    
    /* testing \/ */
    if ((row == 0) && (col == 0)) {
        row = 24;
        col = 80;
        testing = true;
    }
    
    bitmap_image image("/Users/syro/Desktop/xcode/guitest/guitest/checker.bmp");
    if (!image)
       {
          printf("Error - Failed to open: input.bmp\n");
         // return 1;
       }
    
    int frameCount = 0;
    int mult;
    int pixel;
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
                UVcoords.y = i;
                UVcoords.y /= float(row);
                UVcoords.y = UVcoords.y * 2 - 1;
//    
                for (int j = 0; j < col; j++) {
                    UVcoords.x = j;
                    UVcoords.x /= float(col);
                    UVcoords.x = UVcoords.x * 2 - 1;
                    UVcoords.x *= aspectRatio;
                    
                    //UVcoords = normalize(UVcoords,row, col, aspectRatio);
                    
                    
                    float length = sqrt(pow(UVcoords.x,2)+pow(UVcoords.y,2));
                    animation = sin((length * M_2_PI) + float(frameCount)/100);
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
            
            long miliseconds = 2500;
            struct timespec tim;
            tim.tv_sec = 0;
            tim.tv_nsec = 50000000;
            
            nanosleep(&tim, NULL);
            //        int keyPressed = 0;
            //        std::cin >> keyPressed;
            //
            //        if (keyPressed > 0) {
            //            running = false;
            //        }
            //startTime.operator-=(currentTime);
            
        }
    }

  return 0 ;

}
