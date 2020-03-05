//--------------------------------------------------------------------------
//
//  File:       ParticleSystem.h
//
//  Contents:   Declaration of ParticleSystem
//
//	To do:  N/A at this time
//
//	Note:  This isn't a good example of a Class
//    I chose not to use access routines - feel free to use them if you really want to.
//    MyD3DApplication had full access to ParticleSystem
//      
//      Created By:   Orkl
//---------------------------------------------------------------------------

#ifndef __PARTICLESYSTEM_H_
#define __PARTICLESYSTEM_H_

#pragma once

//we need these, so dont fuck them up
#include <windows.h>
#include "D3DApp.h"
#include "D3DMath.h"
#include "D3DTextr.h"
#include "D3DUtil.h"

#include "Particle.h" //Include our particle header ^^'

// return a float in the range 0.0f - 1.0f
#define RANDOM_NUM      (((FLOAT)rand()-(FLOAT)rand())/RAND_MAX)

// return a vector. x,y,z are between 0.0f and 1.0f (not a normalised vector)
#define RANDOM_VECTOR   D3DVECTOR(RANDOM_NUM,RANDOM_NUM,RANDOM_NUM)

// max number of particles (dont fuck with this too much)
#define MAX_PARTICLES	10000 

// some definitions!
#define MIN_SPEED  0.0f	// world units / sec
#define MIN_LIFETIME	0.1f	// seconds
#define MIN_SPREAD  0.01f	// degrees
#define MIN_EMISSION	1.0f	// particles / sec
#define MIN_SIZE  0.5f	// world units
#define MIN_GRAVITY  -5.0f	// multiple of normal gravity 
#define MIN_ALPHA  0.0f	// ratio 

#define MAX_SPEED  250.0f	// world units / sec
#define MAX_LIFETIME	10.0f	// seconds
#define MAX_SPREAD  180.0f	// degrees
#define MAX_EMISSION    1000.0f	// particles / sec
#define MAX_SIZE  10.0f	// world units
#define MAX_GRAVITY  5.0f	// multiple of normal gravity 
#define MAX_ALPHA  1.0f	// ratio 

#define MAX_TEXTURES  	5  //Dont fuck with it unless you add more textures

// add new presets = change this number
#define MAX_PRESETS    9
//---

// guarantee that values are within a given range
#define Clamp(x, min, max)  x = (x<min  ? min : x<max ? x : max);

// class
class ParticleSystem
{
// give access to private members of particlesystem
friend class MyD3DApplication;

public:
   ParticleSystem(); //constructor
  ~ParticleSystem(); //destructor

// Update particles and system with timedelta
bool  Update(float fTimeDelta);
// render
void  Draw(LPDIRECT3DDEVICE7 lpd3dDevice);
// Not used, but can be used to move the particle system (defined it because it could be useful later)
void  Move(D3DVECTOR vLocation);

// this is bad.. Normally you would access routines to change the data
void  ChangeParams(const BYTE Keys[]);

// is system moving, colliding, or attracting?
bool  IsMoving();
bool  IsColliding();
bool  IsAttractive();

// find where system is in world space
void   GetLocation(D3DVECTOR& vLocation);
// number of active particles
unsigned int NumParticles();
// Load current texture into D3D
HRESULT   LoadCurrentTexture(LPDIRECT3DDEVICE7 lpd3dDevice);

private:
// pointer main app, incase reference is needed
void*	m_pApplication;

// texture ID so we know which one is being used
unsigned int	m_uTextureID;

// particles per second to emit
unsigned int	m_uParticlesPerSec;

// stores what particles are currently active
unsigned int	m_uParticlesAlive;

// last known location of system, used for interpolation
D3DVECTOR  m_d3dvPrevLocation;
// current location of system
D3DVECTOR  m_d3dvLocation;
// current velocity as a vector
D3DVECTOR  m_d3dvVelocity;
// current system direction.  particles emit using vector and a variant
D3DVECTOR  m_d3dvDirection;

// as particles age, we keep interpolating between start and end parameters
// var parameter is used to keep things a little random
// we change particle parameters over time to create various different effects, from explosions, to "wind" effects.

// percentage of normal gravity. gravity could be made as a vector to show gravity pushing in a different direction (wind effect)
float m_fGravityStart;
float m_fGravityVar;
float m_fGravityEnd;

// particle size (world units)
float m_fSizeStart;
float m_fSizeVar;
float m_fSizeEnd;

// transparency of particles. Alpha 0 = invisible, Alpha 1 = full visibility
float m_fAlphaStart;
float m_fAlphaVar;
float m_fAlphaEnd;

// colour, start and finish
// colour is interpolated between the two values over time
D3DCOLORVALUE	m_d3dcColorStart;
D3DCOLORVALUE	m_d3dcColorVar;
D3DCOLORVALUE	m_d3dcColorEnd;

// scalar speed is combined with normalised diection vector of system to get a velocity vector
float m_fSpeed;
float m_fSpeedVar;

// particle life (seconds)
float m_fLife;
float m_fLifeVar;

// randomness of particles in directional vector
float m_fTheta;

// is system moving autonomously?
bool m_bIsMoving;
// are particles attracted to system?
bool m_bIsAttractive;

// can particles be emitted? or will they build up?
bool m_bIsSuppressed;

// are particles colliding with ground?
bool m_bIsColliding;

// age of system (seconds)
float m_fAge;

// last time system was updated
float m_fTimeLastUpdate;

// value that allows precise amounts of particles to be emitted.
// example: if we wanted 0.5 particles/sec to be emitted, it cant be done because 0.5 particle != full particle
// therefore we remember what wanted to be emitted and add that to the next emission value
// This can be used to create some cool effects, for example, water that is very slowly leaking.. The particle mass will become bigger and bigger until there is enough for it to "Drip"
float m_fEmissionResidue;

// fixed particle array
CParticle	m_rParticles[MAX_PARTICLES];

   // Geometry
// 4 quad vertices. D3DLVertices means we handle lighting (custom lighting = better effects :D)
D3DLVERTEX   m_Mesh[4];

};

#endif