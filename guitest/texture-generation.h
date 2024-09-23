//
//  texture-generation.h
//  guitest
//
//  Created by syro Fullerton on 16/09/2024.
//


#ifndef texture_generation_h
#define texture_generation_h
#include "cbmp.h"
#include <string>




const char* grayScale[] = {"$","@","B","%","8","&","W","M","#","*","o","a","h","k","b","d","p","q","w","m","Z","O","0","Q","L","C","J","U","Y","X","z","c","v","u","n","x","r","j","f","t","/","|","(",")","1","{","}","[","]","?","-","_","+","~","<",">","i","!","l","I",";",":",",","^","`","."," "};

double length(double x, double y){
    return sqrt(pow(x,2)+pow(y,2));;
}

struct pixelData{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct vec2 {
    double x;
    double y;
};

struct transform{
    double scaleX;
    double scaleY;
    
    int transX;
    int transY;
};

double normalize(double uvCoordx, double uvCoordy, int row, int col, double aspectRatio){
    struct vec2 uvCoords;
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

std::vector<int> getImageDimensions(BMP *bmpImage){
    const int width = get_width(bmpImage);
    const int height = get_height(bmpImage);
    std::vector<int> dimVec = {width,height};
    return dimVec;
}


std::vector<std::vector<pixelData>> getImageData(BMP *bmpImage, int width, int height){

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
            pixDatVec[i][j].r = r;
            pixDatVec[i][j].g = g;
            pixDatVec[i][j].b = b;
        }
    }
    return pixDatVec;
}

double scaleGlobalval = 0;

void scaleGlobal(double scale){
    scaleGlobalval = scale;
}

transform trans;

void setTransform(double scaleX, double scaleY, int transX, int transY){
    trans.scaleX = scaleX;
    trans.scaleY = scaleY;
    
    trans.transX = transX;
    trans.transY = transY;
}

std::string bitMapView(
                int row, int col, int height, int width, std::vector<std::vector<pixelData>> pixDat,double aspectRatio ,double scale){
    std::string frame;
    int xIndex = 0;
    int yIndex = 0;
    //row /= aspectRatio;
 
    for (int i = 0; i < row; i++) { // height

        double scaleY = double(i) / (double(col)/double(width));
        scaleY /= scale;
        scaleY *= trans.scaleY;
        scaleY += trans.transY;
        scaleY *= aspectRatio;
        yIndex = scaleY;
        
        yIndex = fmod(scaleY, height);
        //yIndex = fmin(yIndex, height); // clamp
        
        for (int j = 0; j < col; j++) { // width
            
            double scaleX = double(j) / (double(col)/double(width));
            scaleX *= trans.scaleX;
            scaleX += trans.transX;
            
            scaleX = fmod(scaleX, width); // wrap
            xIndex = scaleX;
            
            int pixValue = pixDat[xIndex][abs(yIndex-height)].r;
            pixValue = double(pixValue)/ (255.f/66.f);
            
            pixValue -= rand()%30;
            pixValue = fmax(pixValue, 0);
            
            if(pixValue > 25){
                frame += "\033[1;31m"; // set colour
                frame += grayScale[pixValue%66];
                frame += "\033[0m";
            }else{
                frame += "\033[1;34m"  ;
                frame += grayScale[pixValue%66];
                frame += "\033[0m";
            }
            
        }
        frame += '\n';
    }
    return frame;
}





#endif /* texture_generation_h */
