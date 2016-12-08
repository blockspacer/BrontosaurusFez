#pragma once
#include <Vector3.h>


struct SEmitterData
{
	const char* TexturePath;
	float EmissionRate;

	CU::Vector3f MinEmissionVelocity;
	CU::Vector3f MaxEmissionVelocity;

	float MinParticleLifeTime;
	float MaxParticleLifeTime;

	float StartSize;
	float EndSize;

	float StartAlpha;
	float EndAlpha;

	float EmissonLifeTime;


	int NumOfParticles;
};

struct  SStreakEmitterData
{
	const char* TexturePath;
	float EmissionRate;

	float ParticleLifetime;

	float StartSize;
	float EndSize;

	float StartAlpha;
	float EndAlpha;

	float EmissonLifeTime;

	int NumOfParticles;
};
