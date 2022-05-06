#include "stage.h"
#include "input.h"
#include "game.h"
//Joan Bosco Olives Florit

//VARIABLES GLOBALS
const Color Stage::bgcolor = { 130, 80, 100 };
Vector2 mov_jump;
//CONSTRUCTOR
Stage::Stage() : speed(30.0f) {
	current_stage = this->STAGE_ID::TITLE;
	sprit_position = TRUE;
	
	fx_vol = 7;
	music_vol = 2;
	total_countdown = 30;
	map = 0;
	limits_bk = {0, 63};
	current_level = 0;
	effects = FALSE;
	music = FALSE;
	title = TRUE;
	timeout = FALSE;
	current_menu = MENU_ID::CONTROLS;
	menu_selected = MENU_SELECTED::SGENERAL;
	
}

// RENDERS
void Stage::RenderTitleStage(Image* framebuffer) {
	std::string text_intro = "Press SPACE";
	std::string text_intro2 = "to Play";
	framebuffer->drawImage(Game::instance->intro, 0, 0);
	framebuffer->drawText(text_intro, 10, 10, Game::instance->font);
	framebuffer->drawText(text_intro2, 20, 20, Game::instance->font);
}
void Stage::RenderIntroStage(Image * framebuffer) {
	std::string text_intro = "Press SPACE to PLAY";
	framebuffer->fill(bgcolor);
	framebuffer->drawImage(Game::instance->controls, 0, 0);
	framebuffer->drawText(text_intro, int(framebuffer->width / 2) - int((7 * text_intro.size()) / 2), 100, Game::instance->font);
	framebuffer->drawText("Move", 10, 30, Game::instance->font);
	framebuffer->drawText("Interact", 3, 54, Game::instance->font);
	framebuffer->drawText("Restart", 3, 79, Game::instance->font);
	framebuffer->drawText("Jump", 85, 54, Game::instance->font);
	framebuffer->drawText("Menu", 85, 79, Game::instance->font);
}

void Stage::RenderMenuStage(Image * framebuffer) {
	std::string text_menu = "Press TAB to RETURN";
	
	if (menu_selected == MENU_SELECTED::SGENERAL) {
		std::string text_menu2 = "use UP, DOWN and ENTER keys";
		framebuffer->fill(bgcolor);
		Image select = Game::instance->sheet.getArea(83, 54, 10, 10);
		framebuffer->drawText(text_menu, 40, 110, Game::instance->minifont, 4, 6);
		framebuffer->drawText(text_menu2, 30, 5, Game::instance->minifont, 4, 6);
		framebuffer->drawImage(Game::instance->menu, 0, 0);
		framebuffer->drawImage(select, 20, int(this->current_menu));
	}
	else if (menu_selected == MENU_SELECTED::SCONTROLS) {
		
		framebuffer->fill(bgcolor);
		framebuffer->drawImage(Game::instance->controls, 0, 0);
		framebuffer->drawText(text_menu, int(framebuffer->width / 2) - int((7 * text_menu.size()) / 2), 100, Game::instance->font);
		framebuffer->drawText("Move", 10, 30, Game::instance->font);
		framebuffer->drawText("Interact", 3, 54, Game::instance->font);
		framebuffer->drawText("Restart", 3, 79, Game::instance->font);
		framebuffer->drawText("Jump", 85, 54, Game::instance->font);
		framebuffer->drawText("Menu", 85, 79, Game::instance->font);
	}
	else if (menu_selected == MENU_SELECTED::SAUDIO) {
		framebuffer->fill(bgcolor);
		framebuffer->drawText(toString(fx_vol), 120, 70, Game::instance->font);
		framebuffer->drawText(toString(music_vol), 120, 100, Game::instance->font);

		if (effects) {
			framebuffer->drawImage(*Game::instance->menus[1], 0, 0);
		}
		if (music) {
			framebuffer->drawImage(*Game::instance->menus[2], 0, 0);
		}
		if (music == FALSE && effects == FALSE)
			framebuffer->drawImage(*Game::instance->menus[0], 0, 0);
	}
	else if (menu_selected == MENU_SELECTED::SSPRITE) {
		framebuffer->fill(bgcolor);
		framebuffer->drawImage(Game::instance->sheet, 0, 0);
		framebuffer->drawText(text_menu, int(framebuffer->width / 2) - int((7 * text_menu.size()) / 2), 100, Game::instance->font);
	}
	
}

void Stage::RenderGameplayStage(Image * framebuffer, Image * tileset, Player * player, float time, Image * sheet) {
	
	RenderBackground(framebuffer);

	framebuffer->drawText(toString(countdown), 140, 2, Game::instance->minifont, 4, 6);

	Game::instance->maps[map]->RenderGameMap(*framebuffer, *tileset);
	
	player->Render(framebuffer, time, sheet);
	//framebuffer->setPixel(player->target.x, player->target.y, Color::RED);
	if(timeout)
		framebuffer->drawImage(Game::instance->timeout, 0, 0);
}
void Stage::RenderEndStage(Image* framebuffer, Image* tileset, Player* player, float time, Image* sheet) {
	RenderBackground(framebuffer);
	Game::instance->maps[map]->RenderGameMap(*framebuffer, *tileset);
	player->Render(framebuffer, time, sheet);
	std::string text_end = "END";
	framebuffer->drawText(text_end, int(framebuffer->width / 2) - int((7 * text_end.size()) / 2), 10, Game::instance->font);
	std::string text_end1 = "Press ESC to EXIT the Game";
	framebuffer->drawText(text_end1, 30, 20, Game::instance->minifont, 4, 6);
	std::string text_end2 = "Bosco Olives";
	framebuffer->drawText(text_end2, int(framebuffer->width / 2) - int((7 * text_end2.size()) / 2), 100, Game::instance->font);
	
}


// UPDATES
void Stage::UpdateTitleStage() {
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		current_stage = STAGE_ID::INTRO;
	}
}
void Stage::UpdateIntroStage() {
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		current_stage = STAGE_ID::GAMEPLAY;
		time = Game::instance->time;
	}
	
}
void Stage::UpdateMenuStage() {
	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
		if (menu_selected == MENU_SELECTED::SGENERAL)
			current_stage = STAGE_ID::GAMEPLAY;
		else if (menu_selected == MENU_SELECTED::SCONTROLS ||
				menu_selected == MENU_SELECTED::SAUDIO ||
				menu_selected == MENU_SELECTED::SSPRITE) {
			menu_selected = MENU_SELECTED::SGENERAL;
		}
	}

	//------------ UP I DOWN DEL MENU PRINCIPAL--------------
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) {
		if (current_menu == MENU_ID::CONTROLS) {
			current_menu = MENU_ID::AUDIO;
		}
		else if (current_menu == MENU_ID::AUDIO) {
			current_menu = MENU_ID::SPRITE;
		}
		else if (current_menu == MENU_ID::SPRITE)
			current_menu = MENU_ID::CONTROLS;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
		if (current_menu == MENU_ID::CONTROLS)
			current_menu = MENU_ID::SPRITE;
		else if (current_menu == MENU_ID::SPRITE)
			current_menu = MENU_ID::AUDIO;
		else if (current_menu == MENU_ID::AUDIO)
			current_menu = MENU_ID::CONTROLS;
	}

	//-----------------SELECCIÓ DEL MENU AL QUE VOLEM ENTRAR, AMB LA TECLA ENTER-------------
	if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)) {
		if(current_menu == MENU_ID::CONTROLS) {
			menu_selected = MENU_SELECTED::SCONTROLS;
		}
		else if(current_menu == MENU_ID::AUDIO) {
			menu_selected = MENU_SELECTED::SAUDIO;
		}
		else if(current_menu == MENU_ID::SPRITE) {
			menu_selected = MENU_SELECTED::SSPRITE;
		}
	}
	//--------------PER EL CAS DEL MENU D'AUDIO, ACTIVEM AQUESTES FUNCIONALITATS DE TECLAT!------
	if (menu_selected == MENU_SELECTED::SAUDIO) {
		if (Input::wasKeyPressed(SDL_SCANCODE_1)) {
			effects = TRUE;
			music = FALSE;
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_2)) {
			effects = FALSE;
			music = TRUE;
		}
		if (effects) {
			if (fx_vol < 10) {
				if (Input::wasKeyPressed(SDL_SCANCODE_UP))
					fx_vol += 1;
			}
			if (fx_vol > 0) {
				if (Input::wasKeyPressed(SDL_SCANCODE_DOWN))
					fx_vol -= 1;
			}
		}
		if (music) {
			if (music_vol < 10) {
				if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
					music_vol += 1;
				}
			}
			if (music_vol > 0) {
				if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) {
					music_vol -= 1;
				}
				Game::instance->synth.volume = float(music_vol) / 10;
			}
		}
	}
	
}

void Stage::UpdateGameplayStage(Player* player, double seconds_elapsed) {
	//Add here your update method

	GameMap* current_map = Game::instance->maps[map];
	countdown = total_countdown - (Game::instance->time - time);
	if (countdown <= 0) {
		timeout = TRUE;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) {
		resetStage(player);
	}
	if (timeout == FALSE) {
		Vector2 movement;
		//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode

		player->dir = player->PLAYER_DIR::IDLE; //SI NO ESTEM PRESSIONANT TECLES, QUE ES MATENGUI EN EL SPRITE IDLE

		movement.y += speed; //GRAVETAT!

		//MENU
		if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) 
			current_stage = STAGE_ID::MENU;

		
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN) || Input::isKeyPressed(SDL_SCANCODE_S)) //if key down or s
		{
			//movement.y += speed;

		}
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT) || Input::isKeyPressed(SDL_SCANCODE_D)) //if key down or d
		{
			movement.x += speed;
			player->sprit_position = TRUE;
			player->dir = player->PLAYER_DIR::RIGHT;

		}
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT) || Input::isKeyPressed(SDL_SCANCODE_A)) //if key down or a
		{
			movement.x -= speed;
			player->sprit_position = FALSE;
			player->dir = player->PLAYER_DIR::RIGHT;
		}

		if (player->jump == FALSE) {
			if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) //if SPACE, JUMP
			{
				player->jump= TRUE; //variable que determina que el jugador esta saltant, no es posara en False fins passats certs frames
				Game::instance->synth.playSample(Game::instance->jump, float(fx_vol)/10, false); //AUDIO del salt
			}
		}
		//------------ SALT ------------
		if (player->jump){
			mov_jump = player->Jump(seconds_elapsed);	
			player->dir = player->PLAYER_DIR::JUMP;
		}
		Vector2 pos_foot = { player->pos.x + int(player->width / 2), player->pos.y + int(player->height - 1) };
		//------------ PUJAR ESCALES ------------
		if (Input::isKeyPressed(SDL_SCANCODE_UP) || Input::isKeyPressed(SDL_SCANCODE_W))  //if key up or w
		{	
			if (current_map->isStairs(pos_foot)) {
				movement.y -= speed * 2;
				player->dir = player->PLAYER_DIR::ACTION;
			}
		}
		//------------ ACCIÓ, LLETRA C ------------
		if (Input::isKeyPressed(SDL_SCANCODE_E)) {
			player->dir = player->PLAYER_DIR::ACTION;
			if (current_map->isChest(pos_foot)) { //OBRIR COFRE
				Game::instance->synth.playSample(Game::instance->chest, float(fx_vol) / 10, false); //AUDIO COFRE
				map += 1;
			}
			if (current_map->isDoor(pos_foot)) { //ENTRAR PORTA
				Game::instance->synth.playSample(Game::instance->door, float(fx_vol) / 10, false); //AUDIO PORTA
				map += 1;
				if (current_map->numDoors == 0) {
					if (map+1 == Game::instance->maps.size()) {//si el nº de mapa actual es igual al total, el +1 és pq map comença a 0
						newGameplayStage(player);
						current_stage = STAGE_ID::END;
						total_countdown = 1000000; //nombre molt gran perque no acabi el joc per temps, a la darrera pantalla END! 
					}
					else
						newGameplayStage(player);
				}
			}
		}
	
		player->target = player->pos + mov_jump + movement * seconds_elapsed;
		player->target2 = player->target; //una copia perque volem seguir pintant el jugador per el pixel de dalt a la dreta, però la comparació volem que es faixi amb el target dels 'peus'
		player->target.x += int(player->width / 2);
		player->target.y += int(player->height)-1; //la zona TARGET es el centre de la última línia del nostre personatge
		player->oldPlayerPos = player->pos;

		if (current_map->isValid(player->target)) {
			player->pos = player->target2;
		}
		else if (current_map->isValid(Vector2(player->target.x, (player->pos.y + int(player->height)-1)))) {
			player->jump = FALSE;
			player->JumpTimer = 0.0;
			player->pos = Vector2(player->target2.x, player->pos.y);
		}
		else if (current_map->isValid(Vector2((player->pos.x + int(player->width / 2)), player->target.y))) {
			player->pos = Vector2(player->pos.x, player->target2.y);
		}

		mov_jump.x = 0;
		mov_jump.y = 0;
	}
}

void Stage::resetStage(Player* player) {
	player->pos = Game::instance->maps[0]->CelltoWorld(Game::instance->maps[0]->spawnPoint); //RE-ASSINGNEM LA POSICIÓ INICIAL!
	player->dir = player->PLAYER_DIR::RIGHT;
	player->sprit_position = TRUE;
	timeout = FALSE;
	time = Game::instance->time; //ARA EL COUNTDOWN DEPEN DEL TOTAL I JA ESTÀ
	map = 0;
	total_countdown = 30;
	limits_bk.x = 0;
	limits_bk.y = 63;
	current_level =  0;
}

void Stage::newGameplayStage(Player* player) {
	Vector2i current_spawnPoint = Game::instance->maps[map]->spawnPoint;//guardem la posicio del spawnPoint del mapa carregat
	player->pos = Game::instance->maps[map]->CelltoWorld(current_spawnPoint); //RE-ASSINGNEM LA POSICIÓ INICIAL!
	player->dir = player->PLAYER_DIR::RIGHT; //posició inicial per partir de un mapa nou
	player->sprit_position = TRUE;
	timeout = FALSE;
	total_countdown += 15;
	current_level += 1;
	time = Game::instance->time; 
	limits_bk.x += 64;
	limits_bk.y += 64;
}

void Stage::RenderBackground(Image* framebuffer) {
	Image bk = Game::instance->background.getArea(limits_bk.x, 0, 63, limits_bk.y);
	framebuffer->drawImage(bk, 0, 0);
	framebuffer->drawImage(bk, 63, 0);
	framebuffer->drawImage(bk, 126, 0);
	framebuffer->drawImage(bk, 0, 63);
	framebuffer->drawImage(bk, 63, 63);
	framebuffer->drawImage(bk, 126, 63);
}