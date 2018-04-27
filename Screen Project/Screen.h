#pragma once

#ifndef SCREEN_H_
#define SCREEN_H_

#include <iostream>
#include <math.h>
#include <SDL.h>

namespace screenNameSpace
{

    void getRandomColor(
        unsigned char *&pColor, const double smoothChangeRed, const double smoothChangeGreen, const double smoothChangeBlue, int elapsed);

    class Screen
    {
    public:
        static const int screenWidth = 800;
        static const int screenHeight = 600;

    private:
        SDL_Window * m_window;
        SDL_Renderer *m_rendere;
        SDL_Texture *m_texture;
        Uint32 *m_buffer;
        Uint32 *m_bufferBlur;

    public:
        Screen();
        bool init();
        void blur(std::string blurType, int numberOfRows, int numberOfColumns);
        void boxBlur(int numberOfRows, int numberOfColumns);
        void gaussianBlur(int numberOfRows, int numberOfColumns);
        void update();
        void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
        bool processEvent();
        void close();
        ~Screen();
    };

}

#endif // !SCREEN_H_