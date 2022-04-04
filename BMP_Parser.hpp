#pragma once

typedef unsigned char      byte1;
typedef unsigned short     byte2;
typedef unsigned int       byte4;
typedef unsigned long long byte8;

class _BMP_image{
private:
    struct _BitMapFileHeader{ // Заголовок файла
    byte2 Type; // Сигнатура "BM";
    byte4 Size; // Размер файла
    byte2 Reserved1; // Зарезервировано
    byte2 Reserved2; // Зарезервировано
    byte4 OffsetBits; // Смещение изображения от начала файла
} BitMapFileHeader;
struct _BitMapInfoHeader{ // Информационный заголовок
    byte4 Size; // Длина заголовка
    byte4 Width; // Ширина изображения, точки
    byte4 Height; // Высота изображения, точки
    byte2 Planes; // Число плоскостей
    byte2 BitCount; // Глубина цвета, бит на точку
    byte4 Compression; // Тип компрессии (0 - несжатое изображение)
    byte4 SizeImage; // Размер изображения, байт
    byte4 XpelsPerMeter; // Горизонтальное разрешение, точки на метр
    byte4 YpelsPerMeter; // Вертикальное разрешение, точки на метр
    byte4 ColorsUsed; // Число используемых цветов (0 - максимально возможное для данной глубины цвета)
    byte4 ColorsImportant; // Число основных цветов
} BitMapInfoHeader;

struct _Pixel{
    byte1 R;
    byte1 G;
    byte1 B;
    byte1 A;
};

byte4 ColorTable[256]; // Таблица цветов (палитра), 256 элементов по 4 байта
_Pixel** Image; // Изображение, записанное по строкам слева направо и снизу вверх
public:
_BMP_image(const char *name);
~_BMP_image();
void printAllInfo();
byte4 getWidth();
byte4 getHeight();
void showConsole();
};