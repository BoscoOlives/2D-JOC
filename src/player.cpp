//Joan Bosco Olives Florit
#include "Player.h"
#include "image.h"
#include "gamemap.h"


int puja;
int baixa;
int animSpeed;
const int Player::num_animations[] = { 4, 14 ,9 ,7 };

Player::Player(){

	width = 16;
	height = 16;

	dir = this->PLAYER_DIR::IDLE;
	sprit_position = TRUE;
	JumpTimer = 0.0f;
	jump = FALSE;

}

void Player::Render(Image* framebuffer, float time, Image* sheet) {
	int start_y = int(this->dir * 16); //depén del tipos de moviment
	int start_x = (int(time * animSpeed) % this->num_animations[this->dir]) * 16; //el cas (la fila) s'escolleix en funció de quina acció es realitzi

	if (this->dir == this->PLAYER_DIR::IDLE) {
		animSpeed = 10.0f;
	}
	else {
		animSpeed = 15.0f;
	}
	if (sprit_position == TRUE)
		framebuffer->drawImage(*sheet, this->pos.x, this->pos.y, Area(start_x, start_y, width, height));
		//framebuffer->drawRectangle(this->pos.x, this->pos.y,width, height, Color::BLACK); //RECTANGLE QUE SIMULA LA ZONA QUE OCUPA EL SPRITE DEL MEU PERSONATGE
	else {
		Image sprite_flip = sheet->getArea(start_x, start_y, width, height);
		sprite_flip.flipX();
		framebuffer->drawImage(sprite_flip, this->pos.x, this->pos.y);
	}
}

Vector2 Player::Jump(double seconds_elapsed) {
	Vector2 mov_jump;
	if (JumpTimer < 55) {
		mov_jump.y -= seconds_elapsed * 150; //90
	}

	JumpTimer += 0.2;
	
	return mov_jump;
}