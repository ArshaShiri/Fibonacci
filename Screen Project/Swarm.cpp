#include "Swarm.h"

namespace swarmNameSpace
{
    Swarm::Swarm() : m_lastTime(0)
    {
        m_pParticles = new particleNameSpace::Particle[numberOfParricles];
    }

    void Swarm::update(int elapsed)
    {
        int interval = elapsed - m_lastTime;
        for (int i = 0; i < Swarm::numberOfParricles; i++)
        {
            m_pParticles[i].update(interval);
        }
        m_lastTime = elapsed;
    }

    Swarm::~Swarm()
    {
        delete[] m_pParticles;
    }

}