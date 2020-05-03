#pragma once
#include<glew.h>

class Texture
{
public:
	Texture();
	Texture(const char* FileLoc);
	bool LoadTexture();
	void UseTexture();
	void ClearTexture();
	~Texture();
private: 
	unsigned int depthMapFBO;
	GLuint textureID;
	int width, height, bitDepth;
	const char *fileLocation;

};

