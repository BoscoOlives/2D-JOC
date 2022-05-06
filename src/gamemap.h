//Joan Bosco Olives Florit
#pragma once

#ifndef GAMEMAP_H
#define GAMEMAP_H

//#include "includes.h"
//#include "game.h"
//#include "utils.h"
//#include "input.h"
#include "image.h"


enum eCellType : uint8 {
	EMPTY = 0,
	CORNER = 1,
	WALL = 2,
	DOOR = 3,
	DOOR_O = 4,
	INVISIBLE_WALL = 5,
	ENEMY = 12,
	WALL2 = 17,
	DOOR2 = 18,
	DOOR2_O = 20,
	WALL3 = 33,
	WALL4 = 36,
	WALL5 = 50,
	WALL6 = 51,
	WALL7 = 52,
	CHEST = 58,
	WALL8 = 66,
	WALL9 = 66,
	STAIR = 92,
	BLOCK = 97,
	START = 255
};

enum eItemType : uint8 {
	NOTHING,
	SWORD,
	POTION
};

struct sCell {
	eCellType type;
	eItemType item;
};

struct sMapHeader {
	int w; //width of map
	int h; //height of map
	unsigned char bytes; //num bytes per cell
	unsigned char extra[7]; //filling bytes, not used
};

class GameMap
{

public:
	int width;
	int height;
	sCell* data;

	Vector2i mapOrigin; //Vector2int indicant el origen del mapa
	int cellSize; //mida de una cel·la

	Vector2i spawnPoint; //Vector amb coordenades de mapa per guardar el punt de Spawn
	//std::vector<Vector2i> enemySpawnpoints; //vector de vectors2i per ubicar els enemics
	int numDoors;
	// CONSTRUCTORS
	GameMap();
	GameMap(int w, int h);

	sCell& getCell(int x, int y);

	static GameMap* loadGameMap(const char* filename);

	void RenderGameMap(Image& framebuffer, Image tileset);

	Vector2 CelltoWorld(Vector2i cellPos);
	Vector2i WorltoCell(Vector2 worldPos, int cellSize);
	bool isValid(Vector2 worldPos);
	bool isStairs(Vector2 worldPos);
	bool isChest(Vector2 worldPos);
	bool isDoor(Vector2 worldPos);
};

#endif