#pragma once

class CStatComponent;
class CTexture;

class CHat 
{
public:
	CHat() = default; //wew
	CHat(char* aTexturePath, CStatComponent* aStats);
	~CHat();
private:
	CStatComponent* myStatComponent; //behöver dessaa vara pekare sen?
	CTexture* myTexture;

};

