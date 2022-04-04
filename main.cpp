#include "BMP_Parser.hpp"

int main(){
    // _BMP_image image("images/test.bmp");
    _BMP_image image("images/test 2.bmp");
    // _BMP_image image("images/rick.bmp");
    // _BMP_image image("images/dog.bmp");
    // _BMP_image image("images/sky.bmp");
    image.printAllInfo();
    image.showConsole();
    return 0;
}