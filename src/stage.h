//Joan Bosco Olives Florit
#pragma once
#include "image.h"
#include "gamemap.h"
#include "player.h"
#include "synth.h"

#ifndef STAGE_H
#define STAGE_H

class Stage
{
public:
	Stage();
	static const Color bgcolor;
	const float speed;
	bool sprit_position;
	int fx_vol;
	int music_vol;
	int total_countdown;
	int map;
	int current_level;
	Vector2i limits_bk;
	bool effects;
	bool music;
	bool title;
	float countdown;
	bool timeout;
	float time;


	enum STAGE_ID {
		INTRO = 0,
		MENU = 1,
		GAMEPLAY = 2,
		TITLE = 3,
		END = 4
	};

	STAGE_ID current_stage;

	enum MENU_ID { //serveix per la selecció visual del menu, quan sa utilitza UP i DOWN al MENU principal
		CONTROLS = 20,
		AUDIO = 55,
		SPRITE = 90
	};
	MENU_ID current_menu;

	enum MENU_SELECTED { //es el indentificador del menu ja seleccionat, en quin esteim
		SGENERAL = 0,
		SCONTROLS = 1,
		SAUDIO = 2,
		SSPRITE = 3
	};
	MENU_SELECTED menu_selected;

	void RenderTitleStage(Image* framebuffer);
	void RenderIntroStage(Image* framebuffer);
	void RenderMenuStage(Image* framebuffer);
	void RenderGameplayStage(Image* framebuffer, Image* tileset, Player* player, float time, Image* sheet);
	void RenderEndStage(Image* framebuffer, Image* tileset, Player* player, float time, Image* sheet);

	void UpdateTitleStage();
	void UpdateIntroStage();
	void UpdateMenuStage();
	void UpdateGameplayStage(Player* player, double seconds_elapsed);
	
	void resetStage(Player* player);
	void newGameplayStage(Player* player);
	void RenderBackground(Image* framebuffer);
};

#endif