//Joan Bosco Olives Florit
#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "gamemap.h"

#include <cmath>

Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	
	loadTGAs(); //carreguem tots els TGA's necessaris
	loadMaps(); //carreguem tots els mapes
	
	player.pos = map_11->CelltoWorld(map_11->spawnPoint);

	enableAudio(); //enable this line if you plan to add audio to your application
	music = synth.loadSample("data/audio/Apxlo.wav");
	synth.playSample(music,1,true);
	synth.volume = 0.2;
	jump = synth.loadSample("data/audio/jump.wav");
	chest = synth.loadSample("data/audio/chest.wav");
	door = synth.loadSample("data/audio/door.wav");

}

//what to do when the image has to be draw
void Game::render(void)
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	Image framebuffer(160, 120); //do not change framebuffer size

	switch (stage.current_stage)
	{
	case Stage::STAGE_ID::TITLE:
		stage.RenderTitleStage(&framebuffer);
		break;
	case Stage::STAGE_ID::INTRO:
		stage.RenderIntroStage(&framebuffer);
		break;
	case Stage::STAGE_ID::MENU:
		stage.RenderMenuStage(&framebuffer);
		break;
	case Stage::STAGE_ID::GAMEPLAY:
		stage.RenderGameplayStage(&framebuffer, &tileset, &player, time, &sheet);
		break;
	case Stage::STAGE_ID::END:
		stage.RenderEndStage(&framebuffer, &tileset, &player, time, &sheet);
		break;
	default:
		break;
	}

	showFramebuffer(&framebuffer);
}

void Game::update(double seconds_elapsed)
{
	////Add here your update method
	switch (stage.current_stage)
	{
	case Stage::STAGE_ID::TITLE:
		stage.UpdateTitleStage();
		break;
	case Stage::STAGE_ID::INTRO:
		stage.UpdateIntroStage();
		break;
	case Stage::STAGE_ID::MENU:
		stage.UpdateMenuStage();
		break;
	case Stage::STAGE_ID::GAMEPLAY:
		stage.UpdateGameplayStage(&player, seconds_elapsed);
		break;
	case Stage::STAGE_ID::END:
		stage.UpdateGameplayStage(&player, seconds_elapsed); //repetim el update de gameplay, la pantalla de end te funcionalitats jugables
		break;
	default:
		break;
	}


	

	//example of 'was pressed'
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)||Input::wasKeyPressed(SDL_SCANCODE_A)) //if key A was pressed
	{
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)||Input::wasKeyPressed(SDL_SCANCODE_D)) //if key D was pressed
	{
	}

	
	
	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		//bgcolor.set(0, 255, 0);
	}
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{

	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{

}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();

}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
void Game::loadMaps() {
	
	map_11 = GameMap::loadGameMap("data/maps/mymap_1.1.map");
	map_12 = GameMap::loadGameMap("data/maps/mymap_1.2.map");
	map_21 = GameMap::loadGameMap("data/maps/mymap_2.1.map");
	map_22 = GameMap::loadGameMap("data/maps/mymap_2.2.map");
	map_23 = GameMap::loadGameMap("data/maps/mymap_2.3.map");
	map_24 = GameMap::loadGameMap("data/maps/mymap_2.4.map");
	map_31 = GameMap::loadGameMap("data/maps/mymap_3.1.map");
	map_32 = GameMap::loadGameMap("data/maps/mymap_3.2.map");
	map_33 = GameMap::loadGameMap("data/maps/mymap_3.3.map");
	map_34 = GameMap::loadGameMap("data/maps/mymap_3.4.map");
	map_35 = GameMap::loadGameMap("data/maps/mymap_3.5.map");
	map_36 = GameMap::loadGameMap("data/maps/mymap_3.6.map");
	map_end = GameMap::loadGameMap("data/maps/mymap_end.map");
	maps.push_back(map_11);
	maps.push_back(map_12);
	maps.push_back(map_21);
	maps.push_back(map_22);
	maps.push_back(map_23);
	maps.push_back(map_24);
	maps.push_back(map_31);
	maps.push_back(map_32);
	maps.push_back(map_33);
	maps.push_back(map_34);
	maps.push_back(map_35);
	maps.push_back(map_36);
	maps.push_back(map_end);
}
void Game::loadTGAs() {
	font.loadTGA("data/fonts/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/fonts/mini-font-white-4x6.tga"); //load bitmap-font image
	sheet.loadTGA("data/sprite/Astronaut-Sheet.tga"); //  load an sprite --> https://dizabanik.itch.io/pixelastronaut
	background.loadTGA("data/maps/backgrounds.tga");// load background --> https://vectorpixelstar.itch.io/space?download
	tileset.loadTGA("data/maps/tileset.tga");
	intro.loadTGA("data/menus/intro.tga");
	menu.loadTGA("data/menus/menu.tga");
	audio.loadTGA("data/menus/audio.tga");
	audio1.loadTGA("data/menus/audio1.tga");
	audio2.loadTGA("data/menus/audio2.tga");
	controls.loadTGA("data/menus/controls.tga");
	timeout.loadTGA("data/menus/timeout.tga");
	menus.reserve(3);
	menus.push_back(&audio);
	menus.push_back(&audio1);
	menus.push_back(&audio2);
}