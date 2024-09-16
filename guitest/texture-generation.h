//
//  texture-generation.h
//  guitest
//
//  Created by syro Fullerton on 16/09/2024.
//


#ifndef texture_generation_h
#define texture_generation_h

const char* grayScale[] = {"$","@","B","%","8","&","W","M","#","*","o","a","h","k","b","d","p","q","w","m","Z","O","0","Q","L","C","J","U","Y","X","z","c","v","u","n","x","r","j","f","t","/","|","(",")","1","{","}","[","]","?","-","_","+","~","<",">","i","!","l","I",";",":",",","^","`","."," "};

double length(double x, double y){
    return sqrt(pow(x,2)+pow(y,2));;
}

struct pixelData{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


//functions should return a single frame ready for std::cout

std::string bitMapView(int row, int col, int height, int width, std::vector<std::vector<pixelData>> pixDat){
    std::string frame;
    int xIndex = 0;
    int yIndex = 0;
 
    for (int i = 0; i < row*2; i+=2) { // height

        double scaleY = double(i) / (double(col)/double(width));
        yIndex = scaleY;
        yIndex = fmin(yIndex, height);
        for (int j = 0; j < col; j++) { // width
            
            double scaleX = double(j) / (double(col)/double(width));
            xIndex = scaleX;
            
            int pixValue = pixDat[xIndex][abs(yIndex-height)].r;
            //pixValue += rand()%10;
            
            
            if(pixValue%66 > 33){
                frame += "\033[1;31m";
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
