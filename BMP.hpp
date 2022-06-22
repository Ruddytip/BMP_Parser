#pragma once
#include <string>

typedef unsigned char      byte1;
typedef unsigned short     byte2;
typedef unsigned int       byte4;
typedef unsigned long long byte8;

struct TColor{
    byte1 R;
    byte1 G;
    byte1 B;
    byte1 A;
};

class BMP{
private:
    // Заголовок файла
    struct BitMapFileHeader{
    byte2 type;             // Сигнатура "BM" (тип файла);
    byte4 size;             // Размер файла
    byte2 reserved1;        // Зарезервировано
    byte2 reserved2;        // Зарезервировано
    byte4 offsetBits;       // Смещение изображения от начала файла
} bitMapFileHeader;
// Информационный заголовок
struct BitMapInfoHeader{
    byte4 size;             // Длина заголовка
    byte4 width;            // Ширина изображения, точки
    byte4 height;           // Высота изображения, точки
    byte2 planes;           // Число плоскостей
    byte2 bitCount;         // Глубина цвета, бит на точку
    byte4 compression;      // Тип компрессии (0 - несжатое изображение)
    byte4 sizeImage;        // Размер изображения, байт
    byte4 xPelsPerMeter;    // Горизонтальное разрешение, точки на метр
    byte4 yPelsPerMeter;    // Вертикальное разрешение, точки на метр
    byte4 colorsUsed;       // Число используемых цветов (0 - максимально возможное для данной глубины цвета)
    byte4 colorsImportant;  // Число основных цветов
} bitMapInfoHeader;
TColor** image;             // Изображение, записанное по строкам слева направо и снизу вверх
public:
BMP(const std::string & fileName);
BMP(const unsigned long _width, const unsigned long _height, const TColor& color);
~BMP();
byte4 getWidth();
byte4 getHeight();
void setPixel(const long x, const long y, const TColor & color);
TColor getPixel(const long x, const long y);
void writeToFile(const std::string & fileName);
};