#include <iostream>
#include <fstream>
#include "screen.hpp"
#include "BMP_Parser.hpp"

_BMP_image::_BMP_image(const char *name){
    std::ifstream fin(name, std::ios_base::binary);
    if (fin.is_open()){
        // Чтение заголовка файла
        fin.read((char*)&BitMapFileHeader.Type,       sizeof(BitMapFileHeader.Type));
        fin.read((char*)&BitMapFileHeader.Size,       sizeof(BitMapFileHeader.Size));
        fin.read((char*)&BitMapFileHeader.Reserved1,  sizeof(BitMapFileHeader.Reserved1));
        fin.read((char*)&BitMapFileHeader.Reserved2,  sizeof(BitMapFileHeader.Reserved2));
        fin.read((char*)&BitMapFileHeader.OffsetBits, sizeof(BitMapFileHeader.OffsetBits));

        //  Чтение информационного заголовка
        fin.read((char*)&BitMapInfoHeader.Size,            sizeof(BitMapInfoHeader.Size));
        fin.read((char*)&BitMapInfoHeader.Width,           sizeof(BitMapInfoHeader.Width));
        fin.read((char*)&BitMapInfoHeader.Height,          sizeof(BitMapInfoHeader.Height));
        fin.read((char*)&BitMapInfoHeader.Planes,          sizeof(BitMapInfoHeader.Planes));
        fin.read((char*)&BitMapInfoHeader.BitCount,        sizeof(BitMapInfoHeader.BitCount));
        fin.read((char*)&BitMapInfoHeader.Compression,     sizeof(BitMapInfoHeader.Compression));
        fin.read((char*)&BitMapInfoHeader.SizeImage,       sizeof(BitMapInfoHeader.SizeImage));
        fin.read((char*)&BitMapInfoHeader.XpelsPerMeter,   sizeof(BitMapInfoHeader.XpelsPerMeter));
        fin.read((char*)&BitMapInfoHeader.YpelsPerMeter,   sizeof(BitMapInfoHeader.YpelsPerMeter));
        fin.read((char*)&BitMapInfoHeader.ColorsUsed,      sizeof(BitMapInfoHeader.ColorsUsed));
        fin.read((char*)&BitMapInfoHeader.ColorsImportant, sizeof(BitMapInfoHeader.ColorsImportant));

        // Чтение таблицы цветов
        // for(size_t i = 0; i < 256; ++i)
        //     fin.read((char*)&ColorTable[i], sizeof(ColorTable[i]));
        
        Image = new _Pixel*[BitMapInfoHeader.Width];
        for(size_t i = 0; i < BitMapInfoHeader.Width; ++i)
            Image[i] = new _Pixel[BitMapInfoHeader.Height];

        // Чтение изображения
        for(size_t j = 0; j < BitMapInfoHeader.Height; ++j)
            for(size_t i = 0; i < BitMapInfoHeader.Width; ++i){
                fin.read((char*)&Image[i][j].B, sizeof(Image[i][j].B));
                fin.read((char*)&Image[i][j].G, sizeof(Image[i][j].G));
                fin.read((char*)&Image[i][j].R, sizeof(Image[i][j].R));
            }

        fin.close();

        std::cout << "===========================\n";
        std::cout << "File - \"" << name << '\"' << std::endl;

    }else{
        std::cerr << "Error read file!!!" << std::endl;
    }
}

_BMP_image::~_BMP_image(){
    for(size_t i = 0; i < BitMapInfoHeader.Width; ++i)
        delete[] Image[i];
    delete[] Image;
}

void _BMP_image::printAllInfo(){
    std::cout << "Type "       << BitMapFileHeader.Type       << std::endl;
    std::cout << "Size "       << BitMapFileHeader.Size       << std::endl;
    std::cout << "Reserved1 "  << BitMapFileHeader.Reserved1  << std::endl;
    std::cout << "Reserved2 "  << BitMapFileHeader.Reserved2  << std::endl;
    std::cout << "OffsetBits " << BitMapFileHeader.OffsetBits << std::endl;

    std::cout << "Size "            << BitMapInfoHeader.Size            << std::endl;
    std::cout << "Width "           << BitMapInfoHeader.Width           << std::endl;
    std::cout << "Height "          << BitMapInfoHeader.Height          << std::endl;
    std::cout << "Planes "          << BitMapInfoHeader.Planes          << std::endl;
    std::cout << "BitCount "        << BitMapInfoHeader.BitCount        << std::endl;
    std::cout << "Compression "     << BitMapInfoHeader.Compression     << std::endl;
    std::cout << "SizeImage "       << BitMapInfoHeader.SizeImage       << std::endl;
    std::cout << "XpelsPerMeter "   << BitMapInfoHeader.XpelsPerMeter   << std::endl;
    std::cout << "YpelsPerMeter "   << BitMapInfoHeader.YpelsPerMeter   << std::endl;
    std::cout << "ColorsUsed "      << BitMapInfoHeader.ColorsUsed      << std::endl;
    std::cout << "ColorsImportant " << BitMapInfoHeader.ColorsImportant << std::endl;
    std::cout << "===========================\n";
}

byte4 _BMP_image::getWidth(){
    return BitMapInfoHeader.Width;
}

byte4 _BMP_image::getHeight(){
    return BitMapInfoHeader.Height;
}

void _BMP_image::showConsole(){
    unsigned int width = BitMapInfoHeader.Width;
    unsigned int height = BitMapInfoHeader.Height;
    CScreen scr(width * 2, height);

    for(size_t j = 0; j < height; ++j){
        for(size_t i = 0; i < width * 2; ++i){
            COLOR color {Image[i / 2][j].R, Image[i / 2][j].G, Image[i / 2][j].B};
            scr.setPixel(i, height - j - 1, COLORS::BLACK, color, ' ');
        }
    }


    scr.show();
}