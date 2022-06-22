#include "BMP.hpp"

int main(){
    BMP test("images/cubes.bmp");
    // BMP test(800, 600, TColor{0, 0, 0, 0});
    for(size_t i = 0; i < 100; ++i)
        for(size_t j = 0; j < 100; ++j)
            test.setPixel(i + 200, j + 200, TColor{255, 255, 255, 0});
    test.writeToFile("test.bmp");
    return 0;
}