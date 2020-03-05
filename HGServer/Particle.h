//--------------------------------------------------------------------------
//
//  File:       Particle.h
//
//  Contents:   Declaration of Particle
//
//	To do:  N/A at this time
//
//	Note:  This isn't a good example of a Class
//    I chose not to use access routines - feel free to use them if you really want to.
//    Particle system has full access to particle.
//      
//      Created By:   Orkl
//---------------------------------------------------------------------------

#ifndef __PARTICLE_H_
#define __PARTICLE_H_

#pragma once

#include "D3DTypes.h" //D3D required file

friend class ParticleSystem; //Its my friend ^^


class Particle
{

public:

//Constructor, Destructor
Particle();
~Particle();

//Time-Based movement
bool Update( float fTimeDelta );

// Last particle location
D3DVECTOR  m_d3dvPrevLocation;

// Current particle location
D3DVECTOR  m_d3dvLocation;

// Current particle velocity (yes, particles use physics!)
D3DVECTOR  m_d3dvVelocity;

// Colour, and colour over time
D3DCOLORVALUE	m_d3dcColor;
D3DCOLORVALUE	m_d3dcColorDelta;

// Particles age (seconds) [Particles have an age, defines when to make the particle "Dissapear" (see next)
float  m_fAge;

// Lifetime (see above)
float  m_fLifetime;

// Particles size, and size over time (we can make it grow, or shrink)
float  m_fSize;
float  m_fSizeDelta;

// Transparency and over time
// 0.0f alpha = transparent
float  m_fAlpha;
float  m_fAlphaDelta;

// gravity, and over time
// this is proportional to normal gravity and can be a negative value (ie. it floats away)
float  m_fGravity;
float  m_fGravityDelta;

// ParticleSystem = Parent of Particle
void  SetParent(ParticleSystem* Parent){ m_pParent = Parent;}

private:
// our parent <3
ParticleSystem* m_pParent;


};

#endif