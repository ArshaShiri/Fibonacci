#pragma once


#ifndef SWARM_H_
#define SWARM_H_

#include "Particle.h"

namespace swarmNameSpace 
{
    class Swarm
    {
    public:
        const static int numberOfParricles=10;
    private:
        particleNameSpace::Particle  *m_pParticles;
        int m_lastTime;
    public:
        Swarm();
        ~Swarm();
        void update(int elapsed);

        const particleNameSpace::Particle *const getParticles() { return m_pParticles; };
    };
}


#endif

