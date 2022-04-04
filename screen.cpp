#include <locale> // wstring_convert
#include <codecvt> // codecvt_utf8
#include "screen.hpp"

/*В программе используются Escape-код
Перемещение курсора вверх на N строк: \033[NA
Перемещение курсора вниз на N строк: \033[NB
Перемещение курсора вперед на N столбцов: \033[NC
Перемещение курсора назад на N столбцов: \033[ND
Стереть до конца строки: \033[K
Цвет текста: \033[38;2;⟨r⟩;⟨g⟩;⟨b⟩m
Цвет фона: \033[48;2;⟨r⟩;⟨g⟩;⟨b⟩m*/

std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;

void CScreen::cursorVision(bool _flag){
    std::cout << (_flag ? "\x1b[?25h" : "\x1b[?25l");
}

CScreen::CScreen(uint32_t _width, uint32_t _height):
width(_width), height(_height), background(1){
    cursorVision(false);
    for(int i = 0; i < width * height; ++i) pixels.push_back(VirtualPixel{COLORS::WHITE, COLORS::BLACK, SYMBOLS::EMPTY});
    for(int i = 0; i < height; ++i) {
        screenBuffer.push_back("");
        std::cout << std::endl;
    }
}

CScreen::~CScreen(){
    cursorVision(true);
    while(!pixels.empty()) pixels.pop_back();
    while(!screenBuffer.empty()) screenBuffer.pop_back();
}

void CScreen::clear(){
    for(auto it = pixels.begin(); it <= pixels.end(); ++it){
        it->penColor   = COLORS::WHITE;
        it->brushColor = COLORS::BLACK;
        it->symbol = SYMBOLS::EMPTY;
    }
}

void CScreen::show(){
    convertBuffer();
    std::cout << "\033[" + std::to_string(height) + "A";
    for(auto it = screenBuffer.begin(); it != screenBuffer.end(); ++it) std::cout << *it + "\x1b[0m" << std::endl;
}

std::string CScreen::convertColor(COLOR _cp, COLOR _cb){
    std::string penCol  = "\033[38;2;" + std::to_string(_cp.R) + ';' + std::to_string(_cp.G) + ';' + std::to_string(_cp.B) + 'm';
    std::string brshCol = "\033[48;2;" + std::to_string(_cb.R) + ';' + std::to_string(_cb.G) + ';' + std::to_string(_cb.B) + 'm';
    brshCol = background ? brshCol : "";
    return (penCol + brshCol);
}

void CScreen::convertBuffer(){
    clearScreenBuffer();
    int row(0);
    for(int i = 1; i <= pixels.size(); ++i){
        screenBuffer[row]+= convertColor(pixels[i - 1].penColor, pixels[i - 1].brushColor);
        screenBuffer[row]+= convert.to_bytes(pixels[i - 1].symbol);
        if(i%width == 0) ++row;
    }
}

void CScreen::clearScreenBuffer(){
    for(auto it = screenBuffer.begin(); it != screenBuffer.end(); ++it) it->clear();
}

void CScreen::setPixel(uint32_t _x, uint32_t _y, COLOR _penColor, COLOR _brushColor, char32_t _symbol){
    if(_x < 0 || _y < 0) return;
    if(_x >= width || _y >= height) return;
    int index = _y * width + _x;
    pixels[index].penColor = _penColor;
    pixels[index].brushColor = _brushColor;
    pixels[index].symbol = _symbol;
}

COLOR CScreen::getPenColor(uint32_t _x, uint32_t _y){
    if(_x < 0 || _y < 0) return COLORS::BLACK;
    if(_x >= width || _y >= height) return COLORS::BLACK;
    int index = _y * width + _x;
    return pixels[index].penColor;
}

COLOR CScreen::getBrushColor(uint32_t _x, uint32_t _y){
    if(_x < 0 || _y < 0) return COLORS::BLACK;
    if(_x >= width || _y >= height) return COLORS::BLACK;
    int index = _y * width + _x;
    return pixels[index].brushColor;
}

char32_t CScreen::getSymbol(uint32_t _x, uint32_t _y){
    if(_x < 0 || _y < 0) return SYMBOLS::ERROR;
    if(_x >= width || _y >= height) return SYMBOLS::ERROR;
    int index = _y * width + _x;
    return pixels[index].symbol;
}

void CScreen::setText(int _x, int _y, COLOR _penColor, COLOR _brushColor, std::u32string _text){
    if(_y < 0 || _y >= height) return;
    int x(_x);
    for(int i = 0; i < _text.length(); ++i){
        if(x + i >= width) break;
        if(x + i < 0) continue;
        int index = _y * width + x + i;
        pixels[index].penColor = _penColor;
        pixels[index].brushColor = _brushColor;
        pixels[index].symbol = _text[i];
    }
}

void CScreen::setBG(bool _flag){
    background = _flag;
}

uint32_t CScreen::getWidth(){
    return width;
}

uint32_t CScreen::getHeight(){
    return height;
}