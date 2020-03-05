//--------------------------------------------------------------------------
//
//  File:       Particle.cpp
//
//  Contents:   Implementation of Particle.h
//
// To do:  N/A at this time
//
// Note: READ THE FUCKING COMMENTS :D
//      
//      Created By:   Orkl
//---------------------------------------------------------------------------
/*
#include "D3DApp.h" //Include our D3D shit
#include "D3DUtil.h"

#include "Particle.h"
#include "ParticleSystem.h"

//Define it at standard gravity (9.8m/s/s) for now
#define GRAVITY D3DVECTOR(0.0f, -9.8f, 0.0f)

Particle::Particle()
{
// note to self: should use newer version of parameter initialisation
m_fAge = 0.0f;
m_fSize = 1.0f;
m_d3dcColor.r = 1.0f;
m_d3dcColor.g = 0.0f;
m_d3dcColor.b = 0.0f;
m_d3dcColor.a = 0.5f;
}


Particle::~Particle()
{
// nothing to do in here
}


//-----------------------------------------------------------------------------
//
//	Particle::Update
//  
//	What it does: Updates the particle with passed in time delta. 
//	If particle dies, false returned
//
//-----------------------------------------------------------------------------
bool Particle::Update( float fTimeDelta )
{

// Some systems can have particles with no lifetime, so they have    
       // to collide with terrain or stay constantly alive if fixed particles are 
       // defined
if ( m_fAge + fTimeDelta >= m_fLifetime ) 
{
 m_fAge = -1.0f;
 return FALSE;
}
else
{

 // all particles age, increase time
 m_fAge += fTimeDelta;

 // current location = previous location next iteration
 m_d3dvPrevLocation = m_d3dvLocation;

 // move particle with last known velocity and time passed 
               // (delta)
 m_d3dvLocation += m_d3dvVelocity * fTimeDelta;

 // allow collision - this should be per particle, but cbf
 // this will snap any particle below ground, to ground level
 if ( m_pParent->IsColliding() )
 {
 	if ( m_d3dvLocation.y < -25.0f ) 
 	{
   // if they are moving at a fast rate, deflect 
                               // vertically
   if ( m_d3dvVelocity.y > -5.0f )  // -5.0f <= velocity.y <= 0.0f
   {
   	m_d3dvLocation.y = -25.0f;
   	m_d3dvVelocity.y = 0.0f;
   }
   // else, rest on ground
   else
   {
   	m_d3dvLocation.y = m_d3dvPrevLocation.y; 
   	m_d3dvVelocity.y = -m_d3dvVelocity.y*0.5f;
   }
 	}
 }

 // gravity is hardcoded above, can be made a vector to
               // to simulate "wind"
 m_d3dvVelocity += ( GRAVITY * m_fGravity * fTimeDelta );

 // if parent supports attractive
 if ( m_pParent->IsAttractive() )
 {

 	D3DVECTOR AttractLocation;
 	
 	// Find parent to track it
 	m_pParent->GetLocation(AttractLocation);

 	// Can be points in space, makes cool effects,                
                       // commented for now
 	//AttractLocation = D3DVECTOR(50,50,50);

 	// calc vector between particle and attractor
 	D3DVECTOR AttractDir = AttractLocation - m_d3dvLocation; 
 	
 	// can turn off attraction for certain axes 
 	// AttractDir.y = 0; etc. etc.
 	
 	// note to self: not accurate gravitation - look in your
                       // physics book later
 	m_d3dvVelocity += Normalize(AttractDir) * 25.0f * fTimeDelta;
 
 }

 // adjust colour from deltas and time elapsed
 m_d3dcColor.r += m_d3dcColorDelta.r * fTimeDelta;
 m_d3dcColor.g += m_d3dcColorDelta.g * fTimeDelta;
 m_d3dcColor.b += m_d3dcColorDelta.b * fTimeDelta;

 // adjust alpha
 m_fAlpha += m_fAlphaDelta * fTimeDelta;
 // adjust size
 m_fSize += m_fSizeDelta * fTimeDelta;
 // adjust gravity 
 m_fGravity += m_fGravityDelta * fTimeDelta;

}

// if we got this far, everything is probably cool
return TRUE;

}
*/