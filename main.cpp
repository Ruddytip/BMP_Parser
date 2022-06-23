#include "BMP.hpp"

int main(){
    TColor white;
    white.R = 0;
    white.G = 255;
    white.B = 0;
    white.A = 255;
    // BMP test("images/cubes.bmp");
    BMP test("test.bmp");
    // BMP test(800, 600, white);
    // for(size_t i = 0; i < 256; ++i)
    //     for(size_t j = 0; j < 256; ++j)
    //         test.setPixel(i + 400, j + 300, TColor{0, 255, 0, byte1(i)});
    test.writeToFile("test.bmp");
    return 0;
}