#include "BMP.hpp"

int main(){
    // BMP test("images/cubes.bmp");
    BMP test(800, 600, TColor{0, 0, 0, 0});
    for(size_t i = 0; i < 256; ++i)
        for(size_t j = 0; j < 256; ++j)
            test.setPixel(i + 400, j + 300, TColor{byte1(i), 0, byte1(i), 0});
    test.writeToFile("test.bmp");
    return 0;
}