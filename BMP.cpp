#include <fstream>
#include "BMP.hpp"

BMP::BMP(const std::string & fileName){
    std::ifstream fin(fileName, std::ios_base::binary);
    if (!fin.is_open()) return;

    // Чтение заголовка файла
    fin.read(reinterpret_cast<char*>(&fileH.type),       sizeof(fileH.type));
    fin.read(reinterpret_cast<char*>(&fileH.size),       sizeof(fileH.size));
    fin.read(reinterpret_cast<char*>(&fileH.reserved1),  sizeof(fileH.reserved1));
    fin.read(reinterpret_cast<char*>(&fileH.reserved2),  sizeof(fileH.reserved2));
    fin.read(reinterpret_cast<char*>(&fileH.offsetBits), sizeof(fileH.offsetBits));

    // Чтение информационного заголовка
    fin.read(reinterpret_cast<char*>(&infoH.size), sizeof(infoH.size));

    // Определение версии вайла
    // 40 - 3 версия, 108 - 4 версия, 124 - 5 версия
    if(infoH.size == 40 || infoH.size == 108 || infoH.size == 124){
        fin.read(reinterpret_cast<char*>(&infoH.width),           sizeof(infoH.width));
        fin.read(reinterpret_cast<char*>(&infoH.height),          sizeof(infoH.height));
        fin.read(reinterpret_cast<char*>(&infoH.planes),          sizeof(infoH.planes));
        fin.read(reinterpret_cast<char*>(&infoH.bitCount),        sizeof(infoH.bitCount));
        fin.read(reinterpret_cast<char*>(&infoH.compression),     sizeof(infoH.compression));
        fin.read(reinterpret_cast<char*>(&infoH.sizeImage),       sizeof(infoH.sizeImage));
        fin.read(reinterpret_cast<char*>(&infoH.xPelsPerMeter),   sizeof(infoH.xPelsPerMeter));
        fin.read(reinterpret_cast<char*>(&infoH.yPelsPerMeter),   sizeof(infoH.yPelsPerMeter));
        fin.read(reinterpret_cast<char*>(&infoH.colorsUsed),      sizeof(infoH.colorsUsed));
        fin.read(reinterpret_cast<char*>(&infoH.colorsImportant), sizeof(infoH.colorsImportant));
    }else{
        return;
    }

    // Чтение полей 4 версии
    if(infoH.size == 108 || infoH.size == 124){
        fin.read(reinterpret_cast<char*>(&infoH.redMask),    sizeof(infoH.redMask));
        fin.read(reinterpret_cast<char*>(&infoH.greenMask),  sizeof(infoH.greenMask));
        fin.read(reinterpret_cast<char*>(&infoH.blueMask),   sizeof(infoH.blueMask));
        fin.read(reinterpret_cast<char*>(&infoH.alphaMask),  sizeof(infoH.alphaMask));
        fin.read(reinterpret_cast<char*>(&infoH.csType),     sizeof(infoH.csType));
        fin.read(reinterpret_cast<char*>(&infoH.endpoints),  sizeof(infoH.endpoints));
        fin.read(reinterpret_cast<char*>(&infoH.gammaRed),   sizeof(infoH.gammaRed));
        fin.read(reinterpret_cast<char*>(&infoH.gammaGreen), sizeof(infoH.gammaGreen));
        fin.read(reinterpret_cast<char*>(&infoH.gammaBlue),  sizeof(infoH.gammaBlue));
    }

    // Чтение полей 5 версии
    if(infoH.size == 124){
        fin.read(reinterpret_cast<char*>(&infoH.intent),      sizeof(infoH.intent));
        fin.read(reinterpret_cast<char*>(&infoH.profileData), sizeof(infoH.profileData));
        fin.read(reinterpret_cast<char*>(&infoH.profileSize), sizeof(infoH.profileSize));
        fin.read(reinterpret_cast<char*>(&infoH.reserved),    sizeof(infoH.reserved));
    }
    
    canvas = new Canvas(infoH.width, infoH.height, Color(0, 0, 0, 0));

    // Чтение изображения

    if(infoH.bitCount == 32){
        for(unsigned int j = 0; j < infoH.height; ++j)
            for(unsigned int i = 0; i < infoH.width; ++i){
                byte1 red, green, blue, al;
                fin.read(reinterpret_cast<char*>(&blue),  sizeof(blue));
                fin.read(reinterpret_cast<char*>(&green), sizeof(green));
                fin.read(reinterpret_cast<char*>(&red),   sizeof(red)); 
                fin.read(reinterpret_cast<char*>(&al),    sizeof(al));                
                canvas->setPixel(i, j, Color(red, green, blue, al));
            }
    }

    if(infoH.bitCount == 24){
        for(unsigned int j = 0; j < infoH.height; ++j)
            for(unsigned int i = 0; i < infoH.width; ++i){
                byte1 red, green, blue;
                fin.read(reinterpret_cast<char*>(&blue),  sizeof(blue));
                fin.read(reinterpret_cast<char*>(&green), sizeof(green));
                fin.read(reinterpret_cast<char*>(&red),   sizeof(red));                
                canvas->setPixel(i, j, Color(red, green, blue, 255));
            }
        infoH.bitCount = 32;
    }

    fin.close();
}

BMP::BMP(const byte4 _width, const byte4 _height, const Color& color){
    // Заголовок файла
    fileH.type       = 0x4D42; // Сигнатура BM
    fileH.reserved1  = 0x0; // Всегда 0
    fileH.reserved2  = 0x0; // Всегда 0
    fileH.offsetBits = 0x8A; // Всегда 138

    // Информационный заголовок
    // 3 версия
    infoH.size            = 0x7C; // Всегда 124
    infoH.width           = _width;
    infoH.height          = _height;
    infoH.planes          = 0x1; // Всегда 1
    infoH.bitCount        = 0x20; // Всегда 32
    infoH.compression     = 0x3; // Всегда 3
    infoH.sizeImage       = _width * _height * 0x4; // 0x4 - кол-во цветов
    infoH.xPelsPerMeter   = 0xBB8; // Всегда 3000
    infoH.yPelsPerMeter   = 0xBB8; // Всегда 3000
    infoH.colorsUsed      = 0x0; // Всегда 0
    infoH.colorsImportant = 0x0; // Всегда 0
    // Все переменные ниже являются константами
    // 4 версия
    infoH.redMask   = 0x00FF0000;
    infoH.greenMask = 0x0000FF00;
    infoH.blueMask  = 0x000000FF;
    infoH.alphaMask = 0xFF000000;
    infoH.csType    = 0x73524742;
    infoH.endpoints.xyzRed   = {0x28F5C28F, 0x151EB852, 0x01EB851F};
    infoH.endpoints.xyzGreen = {0x13333333, 0x26666666, 0x06666666};
    infoH.endpoints.xyzBlue  = {0x0999999A, 0x03D70A3D, 0x328F5C29};
    infoH.gammaRed   = 0x0002199A;
    infoH.gammaGreen = 0x0002199A;
    infoH.gammaBlue  = 0x0002199A;
    // 5 версия
    infoH.intent      = 0x4;
    infoH.profileData = 0x0;
    infoH.profileSize = 0x0;
    infoH.reserved    = 0x0;
    
    fileH.size = infoH.sizeImage + fileH.offsetBits;

    // Изображение
    canvas = new Canvas(infoH.width, infoH.height, color);

}

BMP::BMP(const Canvas& _canvas):
BMP(_canvas.getWidth(), _canvas.getHeight(), Color(0, 0, 0, 0)){
    setCanvas(_canvas);
}

BMP::~BMP(){
    delete canvas;
}

void BMP::writeToFile(const std::string & fileName){
    std::ofstream fout(fileName, std::ios_base::binary | std::ios_base::trunc);
    if(!fout.is_open()) return;

    // Запись заголовка файла
    fout.write(reinterpret_cast<char*>(&fileH.type),       sizeof(fileH.type));
    fout.write(reinterpret_cast<char*>(&fileH.size),       sizeof(fileH.size));
    fout.write(reinterpret_cast<char*>(&fileH.reserved1),  sizeof(fileH.reserved1));
    fout.write(reinterpret_cast<char*>(&fileH.reserved2),  sizeof(fileH.reserved2));
    fout.write(reinterpret_cast<char*>(&fileH.offsetBits), sizeof(fileH.offsetBits));

    // Запись информационного заголовка
    fout.write(reinterpret_cast<char*>(&infoH.size), sizeof(infoH.size));

    // Запись полей 3 версии
    fout.write(reinterpret_cast<char*>(&infoH.width),           sizeof(infoH.width));
    fout.write(reinterpret_cast<char*>(&infoH.height),          sizeof(infoH.height));
    fout.write(reinterpret_cast<char*>(&infoH.planes),          sizeof(infoH.planes));
    fout.write(reinterpret_cast<char*>(&infoH.bitCount),        sizeof(infoH.bitCount));
    fout.write(reinterpret_cast<char*>(&infoH.compression),     sizeof(infoH.compression));
    fout.write(reinterpret_cast<char*>(&infoH.sizeImage),       sizeof(infoH.sizeImage));
    fout.write(reinterpret_cast<char*>(&infoH.xPelsPerMeter),   sizeof(infoH.xPelsPerMeter));
    fout.write(reinterpret_cast<char*>(&infoH.yPelsPerMeter),   sizeof(infoH.yPelsPerMeter));
    fout.write(reinterpret_cast<char*>(&infoH.colorsUsed),      sizeof(infoH.colorsUsed));
    fout.write(reinterpret_cast<char*>(&infoH.colorsImportant), sizeof(infoH.colorsImportant));
    
    // Запись полей 4 версии
    fout.write(reinterpret_cast<char*>(&infoH.redMask),    sizeof(infoH.redMask));
    fout.write(reinterpret_cast<char*>(&infoH.greenMask),  sizeof(infoH.greenMask));
    fout.write(reinterpret_cast<char*>(&infoH.blueMask),   sizeof(infoH.blueMask));
    fout.write(reinterpret_cast<char*>(&infoH.alphaMask),  sizeof(infoH.alphaMask));
    fout.write(reinterpret_cast<char*>(&infoH.csType),     sizeof(infoH.csType));
    fout.write(reinterpret_cast<char*>(&infoH.endpoints),  sizeof(infoH.endpoints));
    fout.write(reinterpret_cast<char*>(&infoH.gammaRed),   sizeof(infoH.gammaRed));
    fout.write(reinterpret_cast<char*>(&infoH.gammaGreen), sizeof(infoH.gammaGreen));
    fout.write(reinterpret_cast<char*>(&infoH.gammaBlue),  sizeof(infoH.gammaBlue));

    // Запись полей 5 версии
    fout.write(reinterpret_cast<char*>(&infoH.intent),      sizeof(infoH.intent));
    fout.write(reinterpret_cast<char*>(&infoH.profileData), sizeof(infoH.profileData));
    fout.write(reinterpret_cast<char*>(&infoH.profileSize), sizeof(infoH.profileSize));
    fout.write(reinterpret_cast<char*>(&infoH.reserved),    sizeof(infoH.reserved));

    // Запись изображения
    for (unsigned int j = 0; j < infoH.height; ++j){
        for (unsigned int i = 0; i < infoH.width; ++i){
            Color clr = canvas->getPixel(i, j);
            fout.write(reinterpret_cast<char*>(&clr.B), sizeof(clr.B));
            fout.write(reinterpret_cast<char*>(&clr.G), sizeof(clr.G));
            fout.write(reinterpret_cast<char*>(&clr.R), sizeof(clr.R));
            fout.write(reinterpret_cast<char*>(&clr.A), sizeof(clr.A));
        }
    }

    fout.close();
}

void BMP::setCanvas(const Canvas& _canvas){
    for(int j = 0; j < infoH.height; ++j)
        for(int i = 0; i < infoH.width; ++i)
            canvas->setPixel(i, j, _canvas.getPixel(i, j));
}

Canvas BMP::getCanvas() const{
    Canvas out(canvas->getWidth(), canvas->getHeight(), Color(0, 0, 0, 0));
    for(int j = 0; j < infoH.height; ++j)
        for(int i = 0; i < infoH.width; ++i)
            out.setPixel(i, j, canvas->getPixel(i, j));
    return out;
}
