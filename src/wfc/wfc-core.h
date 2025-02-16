#ifndef __WFC_CORE_H_
#define __WFC_CORE_H_

#include "..\Liste\Verkettete_Liste.h"

typedef struct tiles tile;
typedef struct position pos;
typedef struct cells cell;

typedef struct tiles {
	unsigned char data[3*3*3];
	int maxTiles;
	int *optionNorth;
	int *optionEast;
	int *optionSouth;
	int *optionWest;
} tile;

typedef struct position {
	int x, y;
} pos;

typedef struct cells {
	int collapsed;
	int entropy;
	tile *m_tile;
	Liste<tile> options;
	pos p;
} cell;

void collapseCell(cell *c, tile *t, pos p);
void drawCell(cell *c, unsigned char* canvas, int width, int height);

void findTileOptions(tile *lstTile, int nmbTile);
void fillTileOptions(tile *lstTile, int ithis, int iother);
void initTiles(tile **t, int maxTiles);
void deleteTiles(tile **t);


#endif /* __WFC_CORE_H_ */