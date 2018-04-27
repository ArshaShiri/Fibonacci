#include "Screen.h"
#include <vector>



namespace screenNameSpace {

    void getRandomColor(
        unsigned char *&pColor, const double smoothChangeRed, const double smoothChangeGreen, const double smoothChangeBlue, int elapsed)
    {
        unsigned char red = unsigned char(abs(sin(elapsed*smoothChangeRed)) * 256);
        unsigned char green = unsigned char(abs(cos(elapsed*smoothChangeGreen)) * 256);
        unsigned char blue = unsigned char(abs(sin(elapsed*smoothChangeBlue)) * 256);
        unsigned char color[] = {red, green, blue};
        pColor = color;
    }

    double blurKernel(std::string blurType, int numberOfRows, int numberOfColumns, int row, int column)
    {
        int totalNumberOfPixels = numberOfRows * numberOfColumns;

        double weight;
        if (blurType == "Box")
        {
            weight = 1.0;
        }
        else if (blurType == "Gauss")
        {
            // intialising standard deviation to 1.0
            double sigma = 0.01;
            double r, s = 2.0 * sigma * sigma;
            r = sqrt(row*row + column * column);
            weight = (exp(-(r*r) / s)) / (M_PI * s);
        }
        else
        {
            std::cout << "Blur Type has not been defined!!" << std::endl;
        }
        return weight;
    }

    Screen::Screen() : m_window(NULL), m_rendere(NULL), m_texture(NULL), m_buffer(NULL), m_bufferBlur(NULL) {}
    

    bool Screen::init()
    {
        if (SDL_INIT_EVERYTHING < 0)
        {
            return false;
        }
 
        std::string test;

        m_window = SDL_CreateWindow(
            "Particle Fire Explosion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

        if (m_window == NULL)
        {
            SDL_Quit();
            return false;
        }

        m_rendere = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
        m_texture = SDL_CreateTexture(m_rendere, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, screenWidth, screenHeight);

        if (m_rendere == NULL)
        {
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        if (m_texture == NULL)
        {
            SDL_DestroyRenderer;
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        m_buffer = new Uint32[screenWidth*screenHeight];
        m_bufferBlur = new Uint32[screenWidth*screenHeight];

        memset(m_buffer, 0, screenWidth*screenHeight * sizeof(Uint32));
        memset(m_bufferBlur, 0, screenWidth*screenHeight * sizeof(Uint32));

        return true;
    }

    void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
    {
        if ( (x<0||x>=screenWidth) || (y<0 || y >= screenHeight) )
        {
            return;
        }

        Uint32 color = 0;

        color += red;
        color <<= 8;
        color += green;
        color <<= 8;
        color += blue;
        color <<= 8;
        color += 0xFF;

        m_buffer[(y*screenWidth)+x] = color; //starts from top left of the screen and goes to the right. like a matrix
    }

    void Screen::blur(std::string blurType, int numberOfRows, int numberOfColumns)
    {
        int nearbyPointsInX = (numberOfRows - 1) / 2;
        int nearbyPointsInY = (numberOfColumns - 1) / 2;
        double weight;
        int totalNumberOfPixels = numberOfRows * numberOfColumns;

        //swap the buffers so the pixel info is in m_bufferBlur and we are drawing to m_buffer
        Uint32 *tempBuffer = m_buffer;
        m_buffer = m_bufferBlur;
        m_bufferBlur = tempBuffer;
        int currentX = 0;
        int currentY = 0;
        double totalWeight = 0;

        for (int y = 0; y < Screen::screenHeight; y++)
        {
            for (int x = 0; x < Screen::screenWidth; x++)
            {
                /* Average the central pixle with the ones around it (in a 3x3 matrix)
                *
                * 0 0 0
                * 0 1 0
                * 0 0 0
                */

                int redAvg = 0;
                int greenAvg = 0;
                int blueAvg = 0;
                double totalWeight = 0;
                for (int row = -nearbyPointsInX; row <= nearbyPointsInX; row++)
                {
                    for (int col = -nearbyPointsInY; col <= nearbyPointsInY; col++)
                    {
                        currentX = x + col;
                        currentY = y + row;
                        weight = blurKernel(blurType, numberOfRows, numberOfColumns, row, col);
                        totalWeight += weight;
                        if (currentX >= 0 && currentX < screenWidth && currentY >= 0 && currentY < screenHeight)
                        {
                            Uint32 color = m_bufferBlur[currentY * screenWidth + currentX];
                            Uint8 red = color >> 24;
                            Uint8 green = color >> 16;
                            Uint8 blue = color >> 8;


                            redAvg += red * weight;
                            greenAvg += green * weight;
                            blueAvg += blue * weight;
                        }
                    }
                }
                if (blurType == "Guass") totalNumberOfPixels = totalWeight;
                Uint8 red = redAvg / totalNumberOfPixels;
                Uint8 green = greenAvg / totalNumberOfPixels;
                Uint8 blue = blueAvg / totalNumberOfPixels;

                setPixel(x, y, red, green, blue);

            }
        }
    }

    void Screen::boxBlur(int numberOfRows, int numberOfColumns)
    {
        //swap the buffers so the pixel info is in m_bufferBlur and we are drawing to m_buffer
        Uint32 *tempBuffer = m_buffer;
        m_buffer = m_bufferBlur;
        m_bufferBlur = tempBuffer;
        int currentX = 0;
        int currentY = 0;
        int totalNumberOfPixels = (2 * numberOfRows + 1) * (2 * numberOfColumns + 1);
        

        for (int y = 0; y < Screen::screenHeight; y++)
        {
            for (int x = 0; x < Screen::screenWidth; x++)
            {
                /* Average the central pixle with the ones around it (in a 3x3 matrix)
                *
                * 0 0 0
                * 0 1 0
                * 0 0 0
                */ 
                int redTotal = 0;
                int greenTotal = 0;
                int blueTotal = 0;
                for (int row = -numberOfRows; row <= numberOfRows; row++)
                {
                    for (int col = -numberOfColumns; col <= numberOfColumns; col++)
                    {
                        currentX = x + col;
                        currentY = y + row;
                        if (currentX >= 0 && currentX < screenWidth && currentY >= 0 && currentY < screenHeight)
                        {
                            Uint32 color = m_bufferBlur[currentY * screenWidth + currentX];
                            Uint8 red = color >> 24;
                            Uint8 green = color >> 16;
                            Uint8 blue = color >> 8;

                            redTotal += red;
                            greenTotal += green;
                            blueTotal += blue;
                        }
                    }
                }
                Uint8 red = redTotal / totalNumberOfPixels;
                Uint8 green = greenTotal / totalNumberOfPixels;
                Uint8 blue = blueTotal / totalNumberOfPixels;

                setPixel(x, y, red, green, blue);

            }
        }
    }

    /*void Screen::gaussianBlur(const int numberOfRows, const int numberOfColumns)
    {
        // intialising standard deviation to 1.0
        double sigma = 1.0;
        double r, s = 2.0 * sigma * sigma;
        double **kernel;

        kernel = new double *[2 * numberOfRows + 1];
        for (int i = 0; i < 2 * numberOfRows + 1; i++)
            kernel[i] = new double[2 * numberOfRows + 1];
        //double kernel[numberOfRows][numberOfColumns];

        // sum is for normalization
        double sum = 0.0;

        // generating 5x5 kernel
        for (int x = -2; x <= 2; x++)
        {
            for (int y = -2; y <= 2; y++)
            {
                r = sqrt(x*x + y * y);
                kernel[x + 2][y + 2] =
                    (exp(-(r*r) / s)) / (M_PI * s);
                sum += kernel[x + 2][y + 2];
            }
        }

        // normalising the Kernel
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                kernel[i][j] /= sum;

        //swap the buffers so the pixel info is in m_bufferBlur and we are drawing to m_buffer
        Uint32 *tempBuffer = m_buffer;
        m_buffer = m_bufferBlur;
        m_bufferBlur = tempBuffer;
        int currentX = 0;
        int currentY = 0;
        int totalNumberOfPixels = (2 * numberOfRows + 1) * (2 * numberOfColumns + 1);

        for (int y = 0; y < Screen::screenHeight; y++)
        {
            for (int x = 0; x < Screen::screenWidth; x++)
            {
                /* Average the central pixle with the ones around it (in a 3x3 matrix)
                *
                * 0 0 0
                * 0 1 0
                * 0 0 0
                */ /*
                int redTotal = 0;
                int greenTotal = 0;
                int blueTotal = 0;
                for (int row = -numberOfRows; row <= numberOfRows; row++)
                {
                    for (int col = -numberOfColumns; col <= numberOfColumns; col++)
                    {
                        currentX = x + col;
                        currentY = y + row;
                        if (currentX >= 0 && currentX < screenWidth && currentY >= 0 && currentY < screenHeight)
                        {
                            Uint32 color = m_bufferBlur[currentY * screenWidth + currentX];
                            Uint8 red = color >> 24;
                            Uint8 green = color >> 16;
                            Uint8 blue = color >> 8;

                            redTotal += red * kernel[row][col];
                            greenTotal += green * kernel[row][col];
                            blueTotal += blue * kernel[row][col];
                        }
                    }
                }
                Uint8 red = redTotal;
                Uint8 green = greenTotal;
                Uint8 blue = blueTotal;

                setPixel(x, y, red, green, blue);
            }
        }

        for (int i = 0; i < numberOfRows; i++)
            delete[] kernel[i];
        delete[] kernel;
    }
    */

    void Screen::update()
    {
        SDL_UpdateTexture(m_texture, NULL, m_buffer, screenWidth * sizeof(Uint32));
        SDL_RenderClear(m_rendere);
        SDL_RenderCopy(m_rendere, m_texture, NULL, NULL);
        SDL_RenderPresent(m_rendere);
    }

    bool  Screen::processEvent()
    {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return false;
            }
        }
        return true;
    }

    void Screen::close()
    {
        delete[] m_buffer;
        delete[] m_bufferBlur;
        SDL_DestroyRenderer;
        SDL_DestroyTexture;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
/*
    void Screen::clear()
    {
        memset(m_buffer, 0, screenWidth*screenHeight * sizeof(Uint32));
    }
*/
    Screen::~Screen() {}

}
