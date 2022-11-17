#include "BMP.hpp"

int main(){
    // BMP test("images/cubes.bmp");
    // BMP test("images/rick.bmp");
    // BMP test("images/sky.bmp");
    // BMP test("images/dog.bmp");
    BMP test(1000, 1000, Color(255, 0, 0, 100));

    Canvas* map = test.getImage();

    for(unsigned int i = 0; i < 500; ++i){
        for(unsigned int j = 0; j < 500; ++j){
            map->setPixel(i, j, Color(i * 0.5, j * 0.5, 0, i * 0.5));
        }
    }

    test.writeToFile("images/test.bmp");
    return 0;
}