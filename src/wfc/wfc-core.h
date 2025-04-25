#ifndef __WFC_CORE_H_
#define __WFC_CORE_H_

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
	int *options;
	int maxOptions;
	int numCells;
	pos p;
} cell;

typedef struct thread_state {
	int finished;
	int abort;
}threadState;

extern void collapseGrid(cell *c, int sizeX, int sizeY, tile **t, int *maxTiles, unsigned char *canvasData, int canvasWidth, int canvasHeight, threadState *state);
extern void initTiles(tile **t, int *maxTiles, unsigned char *picData, int imgWidth, int imgHeight);
extern void deleteTiles(tile **t);

extern void initCells(cell **c, int width, int height, int maxTiles);
extern void deleteCells(cell **c);

#endif /* __WFC_CORE_H_ */