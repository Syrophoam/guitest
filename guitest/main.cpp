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

int main() {
    std::cout << "\e[8;48;160t"; //set terminal size ?
    bool testing = false;
    
    struct winsize ws;
    
    if( ioctl( 0, TIOCGWINSZ, &ws ) != 0 ){
        fprintf(stderr, "TIOCGWINSZ:%s\n", strerror(errno));
        exit(1);
    }
    
    bool running = true;
    int row = ws.ws_row;
    int col = ws.ws_col;
    
    BMP *bmpLogo = bopen("/Users/syro/Desktop/xcode/guitest/guitest/bitmap/Logo.bmp");
    
    std::vector<int> logoSize = getImageDimensions(bmpLogo);
    int widthLogo = logoSize[0];
    int heightLogo = logoSize[1];
    
    std::vector<std::vector<pixelData>> logodata(widthLogo,std::vector<pixelData>(heightLogo));
    logodata = getImageData(bmpLogo, widthLogo, heightLogo);
    
    bclose(bmpLogo);
    
    BMP *bmpMapFull = bopen("/Users/syro/Desktop/xcode/guitest/guitest/mapBmp.bmp");
    
    std::vector<int> mapSize = getImageDimensions(bmpMapFull);
    int widthMapFull = mapSize[0];
    int heightMapFull = mapSize[1];
    
    std::vector<std::vector<pixelData>> mapData(widthMapFull,std::vector<pixelData>(heightMapFull));
    mapData = getImageData(bmpMapFull, widthMapFull, heightMapFull);
    
    
    
    
    int frameCount = 0;
    
    int fps = 5;  // at 200000000 nano seconds per cycle;
        fps = 20; // at 50000000;
    int currentDisplay = 0;
    
    while (running) {
        frameCount++;
       
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

        switch (currentDisplay) {
            case 0:{ // loading screen
                setTransform(1., 1., 0, 0);
                std::string frameGen = bitMapView(row, col, heightLogo, widthLogo, logodata, aspectRatio ,1.);
                std::cout << frameGen;
                
                std::string anyKey;
                if (frameCount > 5) {
                    std::cout << "press the 'any' key to continue";
                    std::cin >> anyKey;
                    currentDisplay = 1;
                    //destroy(frameGen.begin(),frameGen.end());
                }
                break;
            }
                
            case 1:{
                std::string mapFrame;
                
                int divisions = 4;
                int lat;
                
//                int pos = -(col)/2;
//                
//                for (int i = pos; i < (col*2)+1; i+= 3) {
//                    
//                    if ( (i%(col/divisions)) == 0 ) {
//                        mapFrame += "@";
//                        lat = i;
//                        mapFrame += std::to_string(lat);
//                        
//                    }else{
//                        mapFrame += "   ";
//                    }
//                    if(i == 0){
//                        i++;
//                    }
//                    
//                }
                
//                setTransform(1., 1., 0, 4);
               // mapFrame += bitMapView(row-6, col, heightMapFull, widthMapFull, mapData, aspectRatio ,.8);
                mapFrame += bitMapView(row, col, heightMapFull, widthMapFull, mapData, aspectRatio ,.8);
//                for (int i = 0; i < (col*2); i++) {
//                    mapFrame += "O";
//                }
                
                
                std::cout << mapFrame;
                std::cout << "Enter longitude \n";
                double lon;
                std::cin >> lon;
                currentDisplay = 1;
                break;
            }
                //case so on and so fourth
                
            default:
                break;
        }


            struct timespec tim;
            tim.tv_nsec = 50000000;
            
            nanosleep(&tim, NULL);
            
        
    }

  return 0 ;

}
