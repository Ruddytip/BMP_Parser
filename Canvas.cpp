#include "Canvas.hpp"

Canvas::Canvas():
width(0), height(0), canvas(nullptr){}

Canvas::Canvas(const unsigned int _width, const unsigned int _height, const Color& color):
width(_width), height(_height){
    canvas = new Color*[width];
    for(unsigned int i = 0; i < width; ++i)
        canvas[i] = new Color[height];
    clear(color);
}

Canvas::~Canvas(){
    if(canvas != nullptr){
        for(unsigned int i = 0; i < width; ++i)
            delete[] canvas[i];
        delete[] canvas;
    }
}

void Canvas::setPixel(const unsigned int x, const unsigned int y, const Color& color){
    if(x >= width || y >= height) return;
    canvas[x][y] = color;
}

Color Canvas::getPixel(const unsigned int x, const unsigned int y) const{
    if(x >= width || y >= height) return Color(0, 0, 0, 0);
    return canvas[x][y];
}

void Canvas::clear(const Color& color) const{
    for(unsigned int j = 0; j < height; ++j)
        for(unsigned int i = 0; i < width; ++i)
            canvas[i][j] = color;
}

Color** Canvas::getPTR(){
    return canvas;
}

byte4 Canvas::getWidth() const{
    return width;
}

byte4 Canvas::getHeight() const{
    return height;
}