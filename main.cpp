#include "BMP.hpp"

int main(){
    // BMP out("images/cubes.bmp");
    // BMP out("images/rick.bmp");
    // BMP out("images/sky.bmp");
    // BMP out("images/dog.bmp");
    // BMP out(100, 100, Color(255, 0, 0, 100));

    Canvas image(1000, 1000, Color(100, 200, 100, 255));
    // Canvas image = out.getCanvas();

    for(unsigned int i = 0; i < 100; ++i){
        for(unsigned int j = 0; j < 100; ++j){
            image.setPixel(i, j, Color(i * 0.5, j * 0.5, 0, i * 2.5));
        }
    }

    // out.setCanvas(image);
    BMP out(image);
    out.writeToFile("images/test.bmp");
    return 0;
}