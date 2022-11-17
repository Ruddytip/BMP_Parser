#pragma once

typedef unsigned char      byte1;
typedef unsigned short     byte2;
typedef unsigned int       byte4;
typedef unsigned long long byte8;

struct Color{
    byte1 R;
    byte1 G;
    byte1 B;
    byte1 A;
    Color() : R(0), G(0), B(0), A(0){}
    Color(byte1 r, byte1 g, byte1 b, byte1 a):
    R(r), G(g), B(b), A(a){}
};

class Canvas{
private:
    byte4 width, height;
    Color** canvas;
public:
    Canvas();
    Canvas(const unsigned int _width, const unsigned int _height, const Color& color);
    ~Canvas();
    void setPixel(const unsigned int x, const unsigned int y, const Color& color);
    Color getPixel(const unsigned int x, const unsigned int y) const;
    void clear(const Color& color) const;
    Color** getPTR();
    byte4 getWidth() const;
    byte4 getHeight() const;
};