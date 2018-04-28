#pragma once

#ifndef PARTICLE_H_
#define PARTICLE_H_
#define _USE_MATH_DEFINES

#include<stdlib.h>
#include<math.h>
#include<time.h>

namespace particleNameSpace
{
    // In struct by default the members are public. 
    // We want the x and y to be accessible directly which is much faster than a gettr function and is suitable for rendering.
    struct Particle 
    {
        double m_x;
        double m_y;

    private:
        double m_speed;     //Constant speed for circular movement
        double m_direction; //Angle from 0 to 360
        double radiusOfFib;
        double m_updateSpeed;

    private:
        void init();

    public:
        Particle();
        ~Particle();
        void update(int interval);

    };
}

#endif // !PARTICLE_H_