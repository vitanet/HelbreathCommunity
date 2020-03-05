//--------------------------------------------------------------------------
//
//  File:       ParticleSystem.cpp
//
//  Contents:   Implementation of ParticleSystem.h
//
// To do:  N/A at this time
//
// Note: Smiley-Face :)
//      
//      Created By:   Orkl
//---------------------------------------------------------------------------
/*
#include "ParticleSystem.h" //No point of this file without the header for it

#include "D3DApp.h" //D3D shit we need
#include "D3DTextr.h" //...
#include "D3DUtil.h" //...
#include "D3DMath.h" //...

//------------------------------------------
ParticleSystem::ParticleSystem()
{
//the new C++ init methods are faster, but meh..
m_fAge = 0.0f;
m_fTimeLastUpdate = 0.0f;
m_fEmissionResidue =  0.0f;
m_d3dvDirection = D3DVECTOR(0,1,0);

//we have prev location, set to current location, or can cause fucked data in first update
m_d3dvLocation = m_d3dvPrevLocation = D3DVECTOR( 0.0f, 0.0f, 0.0f);

//init the vertices of mesh to acceptable values
// D3DVECTOR(-1.0f,-1.0f, 0.0f ) is location of vertex in local space
// 0x40ffffff is the D3DCOLOR value. doesn't matter, we recalc it every frame anyway
// 0xaarrggbb..
       // this is white with 0.25 opacity (0x40/0xff) = 64/255 ~= 0.25
// 0 is specular value (not using it)
// 0.0f, 1.0f are UV coordinates for the vertex. basically it tells renderer how to map the texture onto triangles that are rendered
m_Mesh[0] = D3DLVERTEX( D3DVECTOR(-1.0f,-1.0f, 0.0f ), 0x40ffffff, 0, 0.0f, 1.0f );
m_Mesh[1] = D3DLVERTEX( D3DVECTOR(-1.0f, 1.0f, 0.0f ), 0x40ffffff, 0, 0.0f, 0.0f );
m_Mesh[2] = D3DLVERTEX( D3DVECTOR( 1.0f,-1.0f, 0.0f ), 0x40ffffff, 0, 1.0f, 1.0f );
m_Mesh[3] = D3DLVERTEX( D3DVECTOR( 1.0f, 1.0f, 0.0f ), 0x40ffffff, 0, 1.0f, 0.0f );
}

//------------------------------------------
ParticleSystem::~ParticleSystem()
{
// dont need anything here
}


//------------------------------------------
//	ParticleSystem::Update
//
//	Will always return true for this, but are reasons why you may want //      it to return false, I wont go into them...
//------------------------------------------

bool ParticleSystem::Update(float fTime)
{
//fTime is absolute time.  we need to calculate relative time
       //displacement or delta that has elapsed
float fTimeDelta = fTime - m_fTimeLastUpdate;

m_fTimeLastUpdate = fTime;

// test out a moving particle system
if (m_bIsMoving)
{
 static float fThetaHoriz = 0.0f;
 static float fThetaVert = 180.0f;

 fThetaHoriz += 50.0f * fTimeDelta;
 fThetaVert += 30.0f * fTimeDelta;

 if ( fThetaHoriz > 359.0f ) 
 	fThetaHoriz = 0.0f;

 if ( fThetaVert > 359.0f ) 
 	fThetaVert = 0.0f;

 m_d3dvLocation.x = 30.0f * (float)cos( fThetaHoriz * 0.01745f );
 m_d3dvLocation.y = 50.0f + 30.0f * (float)sin( fThetaVert * 0.01745f );
 m_d3dvLocation.z = 30.0f * (float)sin( fThetaHoriz * 0.01745f );
 
}
else
{
 // this will reset every frae, and will reset to the origin if its 
               // not moving
 m_d3dvLocation.x = 0.0f;
 m_d3dvLocation.y = 10.0f;
 m_d3dvLocation.z = 0.0f;
 
}

// clear counter before counting (else it'd be useless)
m_uParticlesAlive = 0;

// update all particles that system owns
for ( int i=0; i<MAX_PARTICLES; i++ )
{
 if (m_rParticles[i].m_fAge >= 0.0f )
 	if ( m_rParticles[i].Update( fTimeDelta )) // update function will return FALSE if particle died
   m_uParticlesAlive++;
}

// calc how many particles to create taking into consideration time elapsed and emissionresidue from last frame
float fParticlesNeeded = m_uParticlesPerSec * fTimeDelta + m_fEmissionResidue;

// see how many particles really need to be created
unsigned int uParticlesCreated = (unsigned int)fParticlesNeeded;

if ( !m_bIsSuppressed )
{
 // difference between what we wanted to create and how many we created.
 m_fEmissionResidue = fParticlesNeeded - uParticlesCreated;
}
else
{
 m_fEmissionResidue = fParticlesNeeded;
 uParticlesCreated = 0;
}

// see if there are actually any to create
if ( uParticlesCreated > 0 )
{
 // loop through all particles to see if any available
 for ( i=0; i<MAX_PARTICLES; i++ )
 {
 	// if enough particles are created, this will be 0 and can 
                       // skip the rest
 	// (could also keep list of free and used particles)  	if ( !uParticlesCreated )
   break;

 	// if age is -1.0f then particle not in use
 	if ( m_rParticles[i].m_fAge < 0.0f ) 
 	{
   // New particle so it's age is 0.0f 
   m_rParticles[i].m_fAge = 0.0f;
   m_rParticles[i].m_fLifetime = m_fLife + RANDOM_NUM * m_fLifeVar;
   Clamp( m_rParticles[i].m_fLifetime, MIN_LIFETIME, MAX_LIFETIME ); 

   // start colour is going to be the systems startcolour + the systems colour variation
   m_rParticles[i].m_d3dcColor.r = m_d3dcColorStart.r + RANDOM_NUM * m_d3dcColorVar.r;
   m_rParticles[i].m_d3dcColor.g = m_d3dcColorStart.g + RANDOM_NUM * m_d3dcColorVar.g;
   m_rParticles[i].m_d3dcColor.b = m_d3dcColorStart.b + RANDOM_NUM * m_d3dcColorVar.b;
   // dont use the alpha value of the color here
   m_rParticles[i].m_d3dcColor.a = 1.0f;

   // colour may overflow due to variation, stop that
   Clamp( m_rParticles[i].m_d3dcColor.r, 0.0f, 1.0f );
   Clamp( m_rParticles[i].m_d3dcColor.g, 0.0f, 1.0f );
   Clamp( m_rParticles[i].m_d3dcColor.b, 0.0f, 1.0f );

   // calc a delta so that when the particle dies, it will have reached its colorend
   m_rParticles[i].m_d3dcColorDelta.r = (m_d3dcColorEnd.r - m_rParticles[i].m_d3dcColor.r) / m_rParticles[i].m_fLifetime;
   m_rParticles[i].m_d3dcColorDelta.g = (m_d3dcColorEnd.g - m_rParticles[i].m_d3dcColor.g) / m_rParticles[i].m_fLifetime;
   m_rParticles[i].m_d3dcColorDelta.b = (m_d3dcColorEnd.b - m_rParticles[i].m_d3dcColor.b) / m_rParticles[i].m_fLifetime;

   // calc alphra from system alphastart and alphavar
   m_rParticles[i].m_fAlpha = m_fAlphaStart + RANDOM_NUM * m_fAlphaVar;
   // clamp any overflow
   Clamp( m_rParticles[i].m_fAlpha, MIN_ALPHA, MAX_ALPHA );
   // calc a delta so that when the particle dies, it will have reached its alphaend
   m_rParticles[i].m_fAlphaDelta = (m_fAlphaEnd - m_rParticles[i].m_fAlpha) / m_rParticles[i].m_fLifetime;
   
   
   m_rParticles[i].m_fSize = m_fSizeStart + RANDOM_NUM * m_fSizeVar;
   Clamp( m_rParticles[i].m_fSize, MIN_SIZE, MAX_SIZE );
   m_rParticles[i].m_fSizeDelta = (m_fSizeEnd - m_rParticles[i].m_fSize) / m_rParticles[i].m_fLifetime;
   

   m_rParticles[i].m_fGravity = m_fGravityStart*9.8f + RANDOM_NUM * m_fGravityVar*9.8f;
   // lots of repetitive calculation ( * 9.8f ), but it keeps the meaning of m_fgravity clear
   // its a percentage of normal gravity
   Clamp( m_rParticles[i].m_fGravity, MIN_GRAVITY*9.8f, MAX_GRAVITY*9.8f );
   m_rParticles[i].m_fGravityDelta = ( m_fGravityEnd*9.8f - m_rParticles[i].m_fGravity ) / m_rParticles[i].m_fLifetime;
   
   // calc velocity particle would have to have travelled to move from prevlocation to location in delta secs
   // then calc points in path by passing different time deltas (note random_num * ftimedelta)
   D3DVECTOR tempVelocity = (m_d3dvLocation - m_d3dvPrevLocation)/fTimeDelta;
   
   // emit particles from a location between the last known location and the current location
   m_rParticles[i].m_d3dvLocation = m_d3dvPrevLocation + tempVelocity * RANDOM_NUM * fTimeDelta;

   // simple way of randomisation
   // can also emit from a source primitive e.g.
   
 
   //m_rParticles[i].m_d3dvLocation.x += 2.0f - RANDOM_NUM * 4.0f;
   //m_rParticles[i].m_d3dvLocation.y += 2.0f - RANDOM_NUM * 4.0f;
   //m_rParticles[i].m_d3dvLocation.z += 2.0f - RANDOM_NUM * 4.0f;
   

   // update previous location so the next update we can remember where we were
   m_rParticles[i].m_d3dvPrevLocation = m_d3dvLocation;

   //emitter has a diection, code below adds some randomness to direction so that just a line of particles isn't emitted. 
                               //at the moment it assumes a randomness of 360deg in yaw direction. Will amend code later to allow this to be 
                              //modified separately from the pitch. normally define PI 3.14159 and use a DEG_2_RAD macro.. but I haven't yet.
   float RandomYaw = RANDOM_NUM * 3.14159f * 2.0f;
   float RandomPitch = RANDOM_NUM * m_fTheta * 3.14159f / 180.0f;

   //use spherical coordinates to randomise direction of particle (velocity vector)
   m_rParticles[i].m_d3dvVelocity.y = cosf( RandomPitch );
   m_rParticles[i].m_d3dvVelocity.x = sinf(RandomPitch) * cosf(RandomYaw);
   m_rParticles[i].m_d3dvVelocity.z = sinf(RandomPitch) * sinf(RandomYaw);

   //at the moment, velocity is just direction vector, and needs to be multiplied by speed to be true velocity
   float fNewSpeed = m_fSpeed + RANDOM_NUM * m_fSpeedVar;
   Clamp( fNewSpeed, MIN_SPEED, MAX_SPEED );
   // D3D knows how to multiply vectors by scalars (fucking smartass microsoft :D)
   m_rParticles[i].m_d3dvVelocity *= fNewSpeed;
   
   // who's yo daddy particle?
   m_rParticles[i].SetParent(this);

   // decrement number of needed particles
   uParticlesCreated--;
 	}
 }
}

///set prevlocation to current location so next frame we know where we were
m_d3dvPrevLocation = m_d3dvLocation;	

   return TRUE;
}


//------------------------------------------
//	ParticleSystem::Draw
//
//	render all active particles as billboards! *yay*
//------------------------------------------

void ParticleSystem::Draw(LPDIRECT3DDEVICE7 lpd3dDevice)
{

// as the function says.. load the fkn texture.
LoadCurrentTexture(lpd3dDevice);

   // render!
   for( DWORD i=0; i<MAX_PARTICLES; i++ )
   {

 // if particle is alive, its age wont be zero
 if ( m_rParticles[i].m_fAge >= 0.0f )
 {

 	// set mesh colour to current particles RGBA value
 	m_Mesh[0].color = 
 	m_Mesh[1].color = 
 	m_Mesh[2].color = 
 	m_Mesh[3].color = D3DRGBA(	m_rParticles[i].m_d3dcColor.r, 
         m_rParticles[i].m_d3dcColor.g, 
         m_rParticles[i].m_d3dcColor.b, 
         m_rParticles[i].m_fAlpha);   
   
 	D3DMATRIX ViewMatrix;
 	// makes sure we have a clean matrix before doing stuff with it
 	D3DUtil_SetIdentityMatrix( ViewMatrix );
 	// get viewmatrix from d3d
 	lpd3dDevice->GetTransform( D3DTRANSFORMSTATE_VIEW, &ViewMatrix );

 	D3DMATRIX InvViewMatrix;
 
 	D3DUtil_SetIdentityMatrix( InvViewMatrix );
 	// get inverse of viewmatrix 
 	D3DMath_MatrixInvert( InvViewMatrix, ViewMatrix );

 	// set translation portion of the InvViewMatrix to location of the particles in world space
 	InvViewMatrix._41 = m_rParticles[i].m_d3dvLocation.x;
 	InvViewMatrix._42 = m_rParticles[i].m_d3dvLocation.y;
 	InvViewMatrix._43 = m_rParticles[i].m_d3dvLocation.z;

 	// set scale of particles in local space so when transformed they are correct size
 	m_Mesh[0].dvX = -m_rParticles[i].m_fSize;
 	m_Mesh[0].dvY = -m_rParticles[i].m_fSize;
 	
 	m_Mesh[1].dvX = -m_rParticles[i].m_fSize;
 	m_Mesh[1].dvY =  m_rParticles[i].m_fSize;
 	
 	m_Mesh[2].dvX =  m_rParticles[i].m_fSize;
 	m_Mesh[2].dvY = -m_rParticles[i].m_fSize;
 	
 	m_Mesh[3].dvX =  m_rParticles[i].m_fSize;
 	m_Mesh[3].dvY =  m_rParticles[i].m_fSize;

 	// set new world transform and render particle
 	lpd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &InvViewMatrix );

 	// Note: should get all polygons in array and then pass as trianglelist, but cbf
 	lpd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_Mesh, 4, 0 );

 }
   }

}

void ParticleSystem::Move(D3DVECTOR vLocation)
{
m_d3dvPrevLocation = m_d3dvLocation;
m_d3dvLocation = vLocation;
}

void ParticleSystem::ChangeParams()
{ 
//Will fill this out later
}

bool ParticleSystem::IsMoving()
{ 
return m_bIsMoving; 
}

bool ParticleSystem::IsColliding()
{
return m_bIsColliding;
}

bool ParticleSystem::IsAttractive()
{
return m_bIsAttractive;
}

void ParticleSystem::GetLocation(D3DVECTOR& vLocation)
{
vLocation = m_d3dvLocation;
}

unsigned int ParticleSystem::NumParticles()
{
return m_uParticlesAlive;
}

HRESULT ParticleSystem::LoadCurrentTexture(LPDIRECT3DDEVICE7 lpd3dDevice)
{
switch(m_uTextureID)
{
 case 0:
 	lpd3dDevice->SetTexture( 0, D3DTextr_GetSurface("tex1.tga") );
 	break;
 
 case 1:
 	lpd3dDevice->SetTexture( 0, D3DTextr_GetSurface("tex2.tga") );
 	break;

 case 2:
 	lpd3dDevice->SetTexture( 0, D3DTextr_GetSurface("tex3.tga") );
 	break;

 case 3:
 	lpd3dDevice->SetTexture( 0, D3DTextr_GetSurface("tex4.tga") );
 	break;

 case 4:
 	lpd3dDevice->SetTexture( 0, D3DTextr_GetSurface("tex5.tga") );
 	break;

 default:
 	return FALSE;
 	break;
}

return TRUE;

}
*/