/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/
//Joan Bosco Olives Florit

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "player.h"
#include "stage.h"
class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;
	Image font;
	Image minifont;
	Image timeout;
	//maps
	//VARIABLES MAPA
	Image tileset;
	GameMap* map_11;
	GameMap* map_12;
	GameMap* map_21;
	GameMap* map_22;
	GameMap* map_23;
	GameMap* map_24;
	GameMap* map_31;
	GameMap* map_32;
	GameMap* map_33;
	GameMap* map_34;
	GameMap* map_35;
	GameMap* map_36;
	GameMap* map_end;

	std::vector<GameMap*> maps;


	//audio
	Synth synth;
	Synth::Sample* jump;
	Synth::Sample* music;
	Synth::Sample* chest;
	Synth::Sample* door;
	//stage
	Stage stage;
	std::vector<Image*> menus;
	//Sprite
	Image sheet;
	//Background and Menus Images
	Image intro;
	Image background;
	Image menu;
	Image audio;
	Image audio1;
	Image audio2;
	Image controls;

	//players
	Player player;
	//ctor
	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );
	void update( double dt );

	void showFramebuffer(Image* img);
	void loadMaps();
	void loadTGAs();

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

	//audio stuff
	void enableAudio(); //opens audio channel to play sound
	void onAudio(float* buffer, unsigned int len, double time, SDL_AudioSpec &audio_spec); //called constantly to fill the audio buffer
};


#endif 