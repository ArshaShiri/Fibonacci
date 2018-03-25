#include  <time.h>
#include "Screen.h"
#include "Swarm.h"

using namespace std;
using namespace firstProgram;


int main(int arg, char *argv[])
{
    srand(time(NULL)); //seeds random number generator

    Screen screen;

    if (screen.init() == false)
    {
        cout << "error" << endl;
    }

    Swarm swarm;
    unsigned char *pColor = new unsigned char[3];

    while (true)
    {
        // Update 
        // Draw the particle
        int elapsed = SDL_GetTicks(); //gets the number of mseconds that the program has started

        //screen.clear(); not clearing screen to make the blur effect
        swarm.update(elapsed);

        getRandomColor(pColor, 0.0005, 0.0001, 0.00001, elapsed);
        unsigned char red = pColor[0];
        unsigned char green = pColor[1];
        unsigned char blue = pColor[2];
        //delete[] pColor; why cannot delete here? also a better way to write the color function


        const Particle *const pParticles = swarm.getParticles();
        const int numberOfParticles = Swarm::numberOfParricles;

        for (int i = 0; i < numberOfParticles; i++)
        {
            Particle particle = pParticles[i];

            int x = (particle.m_x + 1) * screen.screenWidth / 2;
            int y = particle.m_y * screen.screenWidth / 2 + screen.screenHeight / 2;

            screen.setPixel(x, y, red, green, blue);
        }
      // screen.boxBlur(1, 1);
        screen.blur("Gauss", 3, 3);
    //   screen.gaussianBlur(2, 2);
        // Draw the screen
        screen.update();
        if (screen.processEvent() == false) {break;}
    }
    screen.close();

    return 0;
}