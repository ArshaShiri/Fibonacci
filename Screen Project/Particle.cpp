#include "Particle.h"

namespace particleNameSpace
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
        radiusOfFib = 0.1 * rand() / RAND_MAX;
        m_updateSpeed = (M_PI / 1e05) * rand() / RAND_MAX;
    }

    void Particle::update(int interval)
    {
        m_direction = (1 / 0.3063489) * log(m_speed / radiusOfFib);
        double xSpeed = m_speed * cos(m_direction);
        double ySpeed = m_speed * sin(m_direction);
        m_x += xSpeed * interval;
        m_y += ySpeed * interval;
        m_speed += m_updateSpeed;

        if (m_x < -20.0 || m_x >= 20.0 || m_y < -20.0 || m_y >= 20.0)
        {
            init();
        }
    }

    Particle::~Particle()
    {
    }

}