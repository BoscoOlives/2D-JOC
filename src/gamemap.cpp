//Joan Bosco Olives Florit
#include "gamemap.h"

GameMap::GameMap()
{
	width = height = 0;
	data = NULL;
}

GameMap::GameMap(int w, int h)
{
	width = w;
	height = h;
	data = new sCell[w * h];

	//utils per canvis de coordenades
	mapOrigin = Vector2i(0, 0);
	cellSize = 8;
}

sCell& GameMap::getCell(int x, int y)
{
	return data[x + y * width];
}



//main functions
//example of parser of .map from rogued editor
GameMap* GameMap::loadGameMap(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL) //file not found
		return NULL;

	sMapHeader header; //read header and store it in the struct
	fread(&header, sizeof(sMapHeader), 1, file);
	assert(header.bytes == 1); //always control bad cases!!


	//allocate memory for the cells data and read it
	unsigned char* cells = new unsigned char[header.w * header.h];
	fread(cells, header.bytes, header.w * header.h, file);
	fclose(file); //always close open files
	//create the map where we will store it
	GameMap* map = new GameMap(header.w, header.h);
	map->numDoors = 0;
	for (int x = 0; x < map->width; x++)			//32
		for (int y = 0; y < map->height; y++)		//16
		{
			
			map->getCell(x, y).type = (eCellType)cells[x + y * map->width];

			if (map->getCell(x, y).type == eCellType::START) { //guardem les coordenades de SPAWN / START
				map->spawnPoint = Vector2i(x, y);
			}
			if (map->getCell(x, y).type == eCellType::DOOR) { //guardem el total de portes! 
				map->numDoors +=1;
			}
		}

	delete[] cells; //always free any memory allocated!

	return map;
}

void GameMap::RenderGameMap(Image& framebuffer, Image tileset) {
	int cs = tileset.width / 16;

	//for every cell
	for (int x = 0; x < this->width; ++x)
		for (int y = 0; y < this->height; ++y)
		{
			//get cell info
			sCell& cell = this->getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			//compute tile pos in tileset image
			int tilex = (type % 16) * cs;     //x pos in tileset
			int tiley = floor(type / 16) * cs;    //y pos in tileset
			Area area(tilex, tiley, cs, cs); //tile area
			int screenx = x * cs; //place offset here if you want
			int screeny = y * cs;
			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx > framebuffer.width ||
				screeny < -cs || screeny > framebuffer.height)
				continue;

			//draw region of tileset inside framebuffer
			framebuffer.drawImage(tileset,         //image
				screenx, screeny,     //pos in screen
				area);         //area
		}

}

Vector2 GameMap::CelltoWorld(Vector2i cellPos){
	return ((cellPos * cellSize) + mapOrigin);
}
Vector2i GameMap::WorltoCell(Vector2 worldPos, int cellSize) {
	return worldPos / cellSize;
}
bool GameMap::isValid(Vector2 worldPos) {
	Vector2i cellCoord = WorltoCell(worldPos, 8);

	//poliza
	if (cellCoord.x < 0 || cellCoord.y < 0 || cellCoord.x >= this->width || cellCoord.y >= this->height)
		return false;
	if (this->getCell(cellCoord.x, cellCoord.y).type == eCellType::BLOCK || 
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::CORNER || 
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL || 
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL2 ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL3 ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL4 ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL5 ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL6 ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL7 ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL8 ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::WALL9 ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::INVISIBLE_WALL ||
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::DOOR || 
		this->getCell(cellCoord.x, cellCoord.y).type == eCellType::DOOR2)
		return false;
	else
		return true;
}
bool GameMap::isStairs(Vector2 worldPos) {
	Vector2i cellCoord = WorltoCell(worldPos, 8);

	//poliza
	if (this->getCell(cellCoord.x, cellCoord.y).type == eCellType::STAIR) { //SÍ LA CEL·LA DEL TARGET COINCIDEIX AMB EL TIPOS ESCALES, UP
		return true;
	}
	else
		return false;
}
bool GameMap::isChest(Vector2 worldPos) {
	Vector2i cellCoord = WorltoCell(worldPos, 8);

	//poliza
	if (this->getCell(cellCoord.x, cellCoord.y).type == eCellType::CHEST) { //SÍ LA CEL·LA DEL TARGET COINCIDEIX AMB EL TIPOS COFRE, retorna true
		return true;
	}
	else
		return false;
}
bool GameMap::isDoor(Vector2 worldPos) {
	Vector2i cellCoord = WorltoCell(worldPos, 8);

	//poliza
	if (this->getCell(cellCoord.x, cellCoord.y).type == eCellType::DOOR_O || this->getCell(cellCoord.x, cellCoord.y).type == eCellType::DOOR2_O) { //SÍ LA CEL·LA DEL TARGET COINCIDEIX AMB EL TIPOS PORTA
		return true;
	}
	else
		return false;
}