#pragma once
#include <irrKlang.h>
#include <stdio.h>
using namespace irrklang;
class Sound
{
public:
	Sound();
	Sound(float posx,float posy,float posz);
	void playMusic(const char* path);
	void playFX(const char* file);
	~Sound();
private: 
	ISoundEngine* engine = createIrrKlangDevice();
	ISound* music;
    float pos_x;
    float pos_y;
    float pos_z;
};
