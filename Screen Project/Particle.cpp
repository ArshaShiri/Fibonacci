#include "Particle.h"

namespace particleNameSpace;
{
    Particle::Particle():m_x(0), m_y(0)
    {
         init();
    }

    void Particle::init()
    {
        m_x = 0;
        m_y = 0;
        m_direction = 0;
        m_speed = 0.01;
        m_speed = pow(m_speed, 2);
        radiusOfFib = 0.001 * rand() / RAND_MAX;
    }

    void Particle::update(int interval)
    {
        m_direction = (1 / 0.3063) * log(m_speed / radiusOfFib);
        double xSpeed =m_speed * cos(m_direction);
        double ySpeed = m_speed * sin(m_direction);
        m_x += xSpeed * interval * 0.1;
        m_y += ySpeed * interval * 0.1;
        m_direction += interval * 0.00005;
        m_speed += (M_PI / 0.5e06) * rand() / RAND_MAX;


        if (m_x < -1.0 || m_x >= 1.0 || m_y < -1.0 || m_y >= 1.0)
        {
            init();
        }
        if (rand() * 100 < RAND_MAX)
        {

        }
    }

    Particle::~Particle()
    {
    }

}