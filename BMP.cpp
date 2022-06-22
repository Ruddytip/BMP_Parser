#include <fstream>
#include "BMP.hpp"

BMP::BMP(const std::string & fileName){
    std::ifstream fin(fileName, std::ios_base::binary);
    if (fin.is_open()){
        // Чтение заголовка файла
        fin.read((char*)&bitMapFileHeader.type,       sizeof(bitMapFileHeader.type));
        fin.read((char*)&bitMapFileHeader.size,       sizeof(bitMapFileHeader.size));
        fin.read((char*)&bitMapFileHeader.reserved1,  sizeof(bitMapFileHeader.reserved1));
        fin.read((char*)&bitMapFileHeader.reserved2,  sizeof(bitMapFileHeader.reserved2));
        fin.read((char*)&bitMapFileHeader.offsetBits, sizeof(bitMapFileHeader.offsetBits));

        // Чтение информационного заголовка
        fin.read((char*)&bitMapInfoHeader.size,            sizeof(bitMapInfoHeader.size));
        fin.read((char*)&bitMapInfoHeader.width,           sizeof(bitMapInfoHeader.width));
        fin.read((char*)&bitMapInfoHeader.height,          sizeof(bitMapInfoHeader.height));
        fin.read((char*)&bitMapInfoHeader.planes,          sizeof(bitMapInfoHeader.planes));
        fin.read((char*)&bitMapInfoHeader.bitCount,        sizeof(bitMapInfoHeader.bitCount));
        fin.read((char*)&bitMapInfoHeader.compression,     sizeof(bitMapInfoHeader.compression));
        fin.read((char*)&bitMapInfoHeader.sizeImage,       sizeof(bitMapInfoHeader.sizeImage));
        fin.read((char*)&bitMapInfoHeader.xPelsPerMeter,   sizeof(bitMapInfoHeader.xPelsPerMeter));
        fin.read((char*)&bitMapInfoHeader.yPelsPerMeter,   sizeof(bitMapInfoHeader.yPelsPerMeter));
        fin.read((char*)&bitMapInfoHeader.colorsUsed,      sizeof(bitMapInfoHeader.colorsUsed));
        fin.read((char*)&bitMapInfoHeader.colorsImportant, sizeof(bitMapInfoHeader.colorsImportant));
        
        image = new TColor*[bitMapInfoHeader.width];
        for(size_t i = 0; i < bitMapInfoHeader.width; ++i)
            image[i] = new TColor[bitMapInfoHeader.height];

        // Чтение изображения
        for(size_t j = 0; j < bitMapInfoHeader.height; ++j)
            for(size_t i = 0; i < bitMapInfoHeader.width; ++i){
                fin.read((char*)&image[i][j].A, sizeof(image[i][j].A));
                fin.read((char*)&image[i][j].B, sizeof(image[i][j].B));
                fin.read((char*)&image[i][j].G, sizeof(image[i][j].G));
                fin.read((char*)&image[i][j].R, sizeof(image[i][j].R));
            }

        fin.close();
    }
}

BMP::BMP(const size_t _width, const size_t _height, const TColor& color){

    // Заголовок файла
    bitMapFileHeader.type       = 0x4D42; // Сигнатура BM
    bitMapFileHeader.reserved1  = 0x0; // Всегда 0
    bitMapFileHeader.reserved2  = 0x0; // Всегда 0
    bitMapFileHeader.offsetBits = 0x36; // Всегда 54
    bitMapFileHeader.size       = _width * _height * 0x3 + bitMapFileHeader.offsetBits; // 0x3 - кол-во цветов

    // Информационный заголовок
    bitMapInfoHeader.size            = 0x28; // Всегда 40
    bitMapInfoHeader.width           = _width;
    bitMapInfoHeader.height          = _height;
    bitMapInfoHeader.planes          = 0x1; // Всегда 1
    bitMapInfoHeader.bitCount        = 0x20; // Всегда 24
    bitMapInfoHeader.compression     = 0x0; // Всегда 0
    bitMapInfoHeader.sizeImage       = _width * _height * 0x4; // 0x3 - кол-во цветов
    bitMapInfoHeader.xPelsPerMeter   = 0xBB8; // Всегда 3000
    bitMapInfoHeader.yPelsPerMeter   = 0xBB8; // Всегда 3000
    bitMapInfoHeader.colorsUsed      = 0x0; // Всегда 0
    bitMapInfoHeader.colorsImportant = 0x0; // Всегда 0
    
    image = new TColor*[bitMapInfoHeader.width];
    for(size_t i = 0; i < bitMapInfoHeader.width; ++i)
        image[i] = new TColor[bitMapInfoHeader.height];

    // Изображение
    for(size_t j = 0; j < bitMapInfoHeader.height; ++j)
        for(size_t i = 0; i < bitMapInfoHeader.width; ++i){
            image[i][j].R = color.R;
            image[i][j].G = color.G;
            image[i][j].B = color.B;
            image[i][j].A = color.A;
        }

}

BMP::~BMP(){
    for(size_t i = 0; i < bitMapInfoHeader.width; ++i)
        delete[] image[i];
    delete[] image;
}

byte4 BMP::getWidth(){
    return bitMapInfoHeader.width;
}

byte4 BMP::getHeight(){
    return bitMapInfoHeader.height;
}

void BMP::setPixel(const size_t x, const size_t y, const TColor & color){
    if(x >= bitMapInfoHeader.width)  return;
    if(y >= bitMapInfoHeader.height) return;
    image[x][y] = color;
}

TColor BMP::getPixel(const size_t x, const size_t y){
    if(x >= bitMapInfoHeader.width)  return TColor{0, 0, 0, 0};
    if(y >= bitMapInfoHeader.height) return TColor{0, 0, 0, 0};
    return image[x][y];
}

void BMP::writeToFile(const std::string & fileName){
    std::ofstream fout(fileName, std::ios_base::binary | std::ios_base::trunc);
    if(fout.is_open()){

        // Запись заголовка файла
        fout.write((char*)&bitMapFileHeader.type,       sizeof(bitMapFileHeader.type));
        fout.write((char*)&bitMapFileHeader.size,       sizeof(bitMapFileHeader.size));
        fout.write((char*)&bitMapFileHeader.reserved1,  sizeof(bitMapFileHeader.reserved1));
        fout.write((char*)&bitMapFileHeader.reserved2,  sizeof(bitMapFileHeader.reserved2));
        fout.write((char*)&bitMapFileHeader.offsetBits, sizeof(bitMapFileHeader.offsetBits));

        // Запись информационного заголовка
        fout.write((char*)&bitMapInfoHeader.size,            sizeof(bitMapInfoHeader.size));
        fout.write((char*)&bitMapInfoHeader.width,           sizeof(bitMapInfoHeader.width));
        fout.write((char*)&bitMapInfoHeader.height,          sizeof(bitMapInfoHeader.height));
        fout.write((char*)&bitMapInfoHeader.planes,          sizeof(bitMapInfoHeader.planes));
        fout.write((char*)&bitMapInfoHeader.bitCount,        sizeof(bitMapInfoHeader.bitCount));
        fout.write((char*)&bitMapInfoHeader.compression,     sizeof(bitMapInfoHeader.compression));
        fout.write((char*)&bitMapInfoHeader.sizeImage,       sizeof(bitMapInfoHeader.sizeImage));
        fout.write((char*)&bitMapInfoHeader.xPelsPerMeter,   sizeof(bitMapInfoHeader.xPelsPerMeter));
        fout.write((char*)&bitMapInfoHeader.yPelsPerMeter,   sizeof(bitMapInfoHeader.yPelsPerMeter));
        fout.write((char*)&bitMapInfoHeader.colorsUsed,      sizeof(bitMapInfoHeader.colorsUsed));
        fout.write((char*)&bitMapInfoHeader.colorsImportant, sizeof(bitMapInfoHeader.colorsImportant));

        // Запись изображения
        for(size_t j = 0; j < bitMapInfoHeader.height; ++j)
            for(size_t i = 0; i < bitMapInfoHeader.width; ++i){
                fout.write((char*)&image[i][j].A, sizeof(image[i][j].A));
                fout.write((char*)&image[i][j].B, sizeof(image[i][j].B));
                fout.write((char*)&image[i][j].G, sizeof(image[i][j].G));
                fout.write((char*)&image[i][j].R, sizeof(image[i][j].R));
            }
        byte8 cc = 0xffffffffffffffff;
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.write((char*)&cc, sizeof(cc));
        fout.close();
    }
}