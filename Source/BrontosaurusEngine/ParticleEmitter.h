#pragma once
#include "EmitterData.h"
#include "vector4.h"
#include "Texture.h"
#include <climits>


namespace CU
{
	class Time;
	class Camera;

	template<typename TYPE>
	class Matrix44;
	using Matrix44f = Matrix44<float>;
}




class CEffect;
class CDXFramework;
struct ID3D11Buffer;

class CParticleEmitter
{
public:

	struct SParticle
	{
		CU::Vector4f position; // stores rotation in W
		CU::Vector4f color;
		float size;
		float garbage[3];
	};

	//doesn't need to be alligned since it's not going to the GPU ?
	struct SParticleLogic
	{
		CU::Vector4f movementDir;
		float lifeTime;
		float lifetimeLeft;
		float rotation;
	};
public:
	CParticleEmitter(const SEmitterData& EmitterData);
	CParticleEmitter(const CParticleEmitter& aParticleEmitter) = delete;
	~CParticleEmitter();
	void Init();
	void Update(const CU::Time & aDeltaTimem, const CU::Matrix44f& aToWorldSpace);
	void Render(const CU::Matrix44f & aToWorldSpace);
	inline void Activate();
	inline void Deactivate();

private:

	void EmitParticle(const CU::Matrix44f& aToWorldSpace);
	void UpdateCBuffers(const CU::Matrix44f& aToWorldSpace);
	bool InitBuffers();

private:
	static constexpr unsigned short ourMaxNrOfParticles = 2048u;
	CU::GrowingArray<SParticle> myParticles;
	CU::GrowingArray<SParticleLogic> myParticleLogic;

	SEmitterData	myEmitterData;

	CTexture*		myTexture;
	CEffect*		myEffect;

	CDXFramework*	myFramework;
	ID3D11Buffer*	myVertexBuffer;
	ID3D11Buffer*	myModelBuffer;


	float myEmitDelta;
	float myEmitTimer;
	bool  myIsActive;
};

void CParticleEmitter::Activate()
{
	myIsActive = true;
}

void CParticleEmitter::Deactivate()
{
	myIsActive = false;
}