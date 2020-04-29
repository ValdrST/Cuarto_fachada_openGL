#pragma once
#include<stdio.h>
#include <glfw3.h>
#include <stdlib.h>
#include <string.h>

typedef struct _frame
{
	float mov_x = 0.0f;
	float mov_y = 0.0f;
	float mov_z = 0.0f;
	float mov_xInc = 0.0f;
	float mov_yInc = 0.0f;
	float mov_zInc = 0.0f;
	float giro_x = 0.0f;
	float giro_y = 0.0f;
	float giro_z = 0.0f;
	float giro_xinc = 0.0f;
	float giro_yinc = 0.0f;
	float giro_zinc = 0.0f;
	float scale_x = 0.0f;
	float scale_y = 0.0f;
	float scale_z = 0.0f;
	float scale_xinc = 0.0f;
	float scale_yinc = 0.0f;
	float scale_zinc = 0.0f;
}FRAME;
class Keyframe
{
public:
	Keyframe();
	Keyframe(const char* keyFile, int _numFrames, const char* _id_key);
	void parse_key_frames();
	void animate();
	void interpolation();
	void interpolate(float *inc, float *nVal, float *val);
	void resetElements();
	void setPlay(bool status) { play = status; }
	bool getPlay() { return play; }
	void inputKeyframes(bool accion);
	float getVal(const char* val);
	~Keyframe();
private:
	FILE *kFile;
	const char* nameFile;
	const char* id_keyFrames;
	float mov_x, mov_y, mov_z, giro_x, giro_y, giro_z, scale_x, scale_y, scale_z;
	int i_max_steps = 90;
	int i_curr_steps = 5;
	int numFrames;
	FRAME KeyFrame[100];
	int FrameIndex;
	int reproduciranimacion = 0, habilitaranimacion = 0;
	bool play = false;
	int playIndex = 0;

};