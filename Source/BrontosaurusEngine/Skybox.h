#pragma once
#include <vector4.h>
#include <matrix44.h>
#include "Texture.h"

class CEffect;
class CTexture;

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;


namespace CU
{
	class Camera;
}


class CSkybox
{
public: 
	

public:
	CSkybox();
	~CSkybox();
	void Init(const char* aPath);
	void Render(const CU::Camera & aCamera);
	inline CTexture* GetTexture();
private:
	void CreateVertexIndexBuffer();
	void UpdateCbuffer(const CU::Camera& aCamera);

private:
	CEffect* myEffect;
	ID3D11Buffer* myVSBuffer;
	ID3D11Buffer* myVertexBuffer;
	ID3D11Buffer* myIndexBuffer;

	CTexture* mySkyboxTexture;
};

inline CTexture * CSkybox::GetTexture()
{
	return mySkyboxTexture;
}
