#pragma once
#include <string>
#include "Canvas.hpp"

class BMP{
private:
    // Заголовок файла
    struct BitMapFileHeader{
        byte2 type;             // Сигнатура "BM" (тип файла);
        byte4 size;             // Размер файла
        byte2 reserved1;        // Зарезервировано
        byte2 reserved2;        // Зарезервировано
        byte4 offsetBits;       // Смещение изображения от начала файла
    } fileH;
    // Информационный заголовок
    struct BitMapInfoHeaderV5{
        // 3 версия
        byte4 size;             // Размер заголовка, если размер 40 - 3 версия, 108 - 4 версия, 124 - 5 версия
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
        // 4 версия
        byte4 redMask;
        byte4 greenMask;
        byte4 blueMask;
        byte4 alphaMask;
        byte4 csType; // Вид цветового пространства
        // содержит x, y и z координаты трех цветов,
        // которые соответствуют красной, зеленой, и синей конечным точкам заданного логического цветового пространства.
        struct CIEXYZTRIPLE{
            struct CIEXYZ{
                byte4 x;
                byte4 y;
                byte4 z;
            } xyzRed, xyzGreen, xyzBlue;
        } endpoints;
        byte4 gammaRed;
        byte4 gammaGreen;
        byte4 gammaBlue;
        // Значение последних четырёх полей берётся во внимание только,
        // если поле CSType содержит 0 (LCS_CALIBRATED_RGB).
        // Тогда конечные точки и значения гаммы для трёх цветовых компонент указываются в этих полях
        // 5 версия
        byte4 intent; // Предпочтения при рендеринге растра
        byte4 profileData; // Смещение в байтах цветового профиля от начала BITMAPINFO
        byte4 profileSize; // Если в BMP непосредственно включается цветовой профиль, то здесь указывается его размер в байтах
        byte4 reserved; // Зарезервировано и должно быть обнулено
    } infoH;
    Canvas* canvas;               // Изображение, записанное по строкам слева направо и снизу вверх
public:
    BMP(const std::string & fileName);
    BMP(const byte4 _width, const byte4 _height, const Color& color);
    BMP(const Canvas& _canvas);
    ~BMP();
    void writeToFile(const std::string & fileName);
    void setCanvas(const Canvas& _canvas);
    Canvas getCanvas() const;
};