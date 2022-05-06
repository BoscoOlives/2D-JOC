//Joan Bosco Olives Florit
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "includes.h"
#include "image.h"

class Player
{
public:
	Vector2 pos;
	static const int num_animations[4];
	bool sprit_position;
	float JumpTimer;
	bool jump;
	Vector2 start_pos;
	Vector2 target;
	Vector2 target2;
	Vector2 oldPlayerPos;
	int width;
	int height;
	Player();
	void Render(Image* framebuffer, float time, Image* sheet);
	Vector2 Jump(double seconds_elapsed);


	enum PLAYER_DIR {
		IDLE = 0,
		RIGHT = 1,
		JUMP = 2,
		ACTION = 3
	};

	PLAYER_DIR dir;

};



#endif 