#include "wfc-core.h"
#include "..\queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void findTileOptions(tile *lstTile, int nmbTile);
void fillTileOptions(tile *lstTile, int ithis, int iother);
void collapseCell(cell *c, tile *t, int tileNmb, int x, int y);
void drawCell(cell *c, unsigned char* canvas, int width, int height);
void reduceNeighbours(cell *c, int_q_lnk *queue, int sizeX, int sizeY, tile **lstTile);
int selectCellToCollapse(cell *c, int *x, int *y, int sizeX, int sizeY);
void selectRandomTile(cell *c, int *tileNmb);

size_t random;

/* for debugging, remove later */
void printAllCells(cell *c, int sizeX, int sizeY);

void initCell(cell *c)
{
	c->collapsed = 0;
}

void collapseCell(cell *c, tile *t, int tileNmb, int x, int y)
{
	c->m_tile = t;
	c->p.x = x;
	c->p.y = y;
	for (int i = 0; i < c->maxOptions; i++) {
		c->options[i] = (i == tileNmb);
	}
	c->collapsed = 1;
}

void drawCell(cell *c, unsigned char* canvas, int width, int height)
{	
	if(c->collapsed == 0) {
		printf("Cell is not collapsed\n");
		return;
	}
	if((c->p.x) >= width || (c->p.y) >= height) {
		printf("Cell (%d, %d) outside canvas (%d, %d)!\n", c->p.x, c->p.y, width, height);
		return;
	}
	tile *t = c->m_tile;
	int curpos;
	curpos = (c->p.x + c->p.y * width) * 3;
	canvas[curpos + 0] = t->data[12]; /* 4. Pixel ist die Mitte der Kachel * 3 (RGB) = 12 */
	canvas[curpos + 1] = t->data[13];
	canvas[curpos + 2] = t->data[14];
	
	return;
}

void initTiles(tile **t, int *numTiles, unsigned char *picData, int imgWidth, int imgHeight)
{
	if (imgWidth < 3 || imgHeight < 3) {
		*numTiles = 0;
		*t = (tile*) malloc(sizeof(tile) * 1);
		return;
	}
	int maxTiles = *numTiles = (imgWidth - 2) * (imgHeight - 2);

	tile *tempT = (tile*) malloc(sizeof(tile) * maxTiles);
	for (int i = 0; i < maxTiles; i++) {
		tempT[i].maxTiles = maxTiles;
		tempT[i].optionNorth = (int*) malloc(sizeof(int) * maxTiles);
		tempT[i].optionSouth = (int*) malloc(sizeof(int) * maxTiles);
		tempT[i].optionEast = (int*) malloc(sizeof(int) * maxTiles);
		tempT[i].optionWest = (int*) malloc(sizeof(int) * maxTiles);
	}
	*t = tempT;
	
	int wImg, hImg;
	for (int i = 0; i < maxTiles; i++) {
		wImg = i % (imgWidth - 2);
		hImg = i / (imgWidth - 2);
		for (int h = 0; h < 3; h++) {
			for (int w = 0; w < 3; w++) {
				(*t)[i].data[(w + h * 3) * 3 + 0] = picData[((wImg + w) + (hImg + h) * imgWidth) * 3 + 0];
				(*t)[i].data[(w + h * 3) * 3 + 1] = picData[((wImg + w) + (hImg + h) * imgWidth) * 3 + 1];
				(*t)[i].data[(w + h * 3) * 3 + 2] = picData[((wImg + w) + (hImg + h) * imgWidth) * 3 + 2];
			}
		}
	}
	
	findTileOptions(*t, maxTiles);
	return;
}

void deleteTiles(tile **t)
{
	tile *tempT = *t;
	for (int i = 0; i < tempT[0].maxTiles; i++) {
		free(tempT[i].optionNorth);
		free(tempT[i].optionSouth);
		free(tempT[i].optionEast);
		free(tempT[i].optionWest);
	}
	free(*t);
	printf("Tiles deleted\n");
}

void findTileOptions(tile *lstTile, int nmbTile)
{
	for (int ithis = 0; ithis < nmbTile; ithis++) {
		for (int iother = 0; iother < nmbTile; iother++) {
			fillTileOptions(lstTile, ithis, iother);
		}
	}
	return;
}

void fillTileOptions(tile *t_tiles, int thisIndex, int otherIndex)
{
	int isOption;
	
	/* optionNorth */
	isOption = 1;
	for (int w = 0; w < 3; w++) {
		if (t_tiles[thisIndex].data[(w + 0 * 3) * 3 + 0] != t_tiles[otherIndex].data[(w + 2 * 3) * 3 + 0]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(w + 0 * 3) * 3 + 1] != t_tiles[otherIndex].data[(w + 2 * 3) * 3 + 1]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(w + 0 * 3) * 3 + 2] != t_tiles[otherIndex].data[(w + 2 * 3) * 3 + 2]) {
			isOption = 0;
			break;
		}
	}
	t_tiles[thisIndex].optionNorth[otherIndex] = isOption;
	/* END OptionNorth */
	
	/* optionEast */
	isOption = 1;
	for (int h = 0; h < 3; h++) {
		if (t_tiles[thisIndex].data[(2 + h * 3) * 3 + 0] != t_tiles[otherIndex].data[(0 + h * 3) * 3 + 0]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(2 + h * 3) * 3 + 1] != t_tiles[otherIndex].data[(0 + h * 3) * 3 + 1]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(2 + h * 3) * 3 + 2] != t_tiles[otherIndex].data[(0 + h * 3) * 3 + 2]) {
			isOption = 0;
			break;
		}
	}
	t_tiles[thisIndex].optionEast[otherIndex] = isOption;
	/* END optionEast */
	
	/* optionSouth */
	isOption = 1;
	for (int w = 0; w < 3; w++) {
		if (t_tiles[thisIndex].data[(w + 2 * 3) * 3 + 0] != t_tiles[otherIndex].data[(w + 0 * 3) * 3 + 0]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(w + 2 * 3) * 3 + 1] != t_tiles[otherIndex].data[(w + 0 * 3) * 3 + 1]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(w + 2 * 3) * 3 + 2] != t_tiles[otherIndex].data[(w + 0 * 3) * 3 + 2]) {
			isOption = 0;
			break;
		}
	}
	t_tiles[thisIndex].optionSouth[otherIndex] = isOption;
	/* END OptionSouth */
	
	/* optionWest */
	isOption = 1;
	for (int h = 0; h < 3; h++) {
		if (t_tiles[thisIndex].data[(0 + h * 3) * 3 + 0] != t_tiles[otherIndex].data[(2 + h * 3) * 3 + 0]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(0 + h * 3) * 3 + 1] != t_tiles[otherIndex].data[(2 + h * 3) * 3 + 1]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(0 + h * 3) * 3 + 2] != t_tiles[otherIndex].data[(2 + h * 3) * 3 + 2]) {
			isOption = 0;
			break;
		}
	}
	t_tiles[thisIndex].optionWest[otherIndex] = isOption;
	/* END optionWest */
	return;
}

void collapseGrid(cell *c, int sizeX, int sizeY, tile **t, int *maxTiles, unsigned char *canvasData, int canvasWidth, int canvasHeight, threadState *state)
{
	int x, y, tileNmb;
	tile curTile;
	
	srand(time(0));
	random = rand();
	
	do {
		if (state->abort == 1) {/* request for abort */
			printf("abort requested\n");
			state->finished = 1;
			return;
		}
		if (!selectCellToCollapse(c, &x, &y, sizeX, sizeY)) {
			break;
		}
		selectRandomTile(&c[x + y * sizeX], &tileNmb);
		if (tileNmb < 0) {
			printf("tile selection failed (cell %3d | %3d: entropy = %d)\nexiting\n", x, y, c[x + y * sizeX].entropy);
			state->finished = 1;
			return;
		}
		curTile = (*t)[tileNmb];
		collapseCell(&c[x + y * sizeX], &curTile, tileNmb, x, y);
		drawCell(&c[x + y * sizeX], canvasData, canvasWidth, canvasHeight);
		
		int_q_lnk *queue = initQueue();
		if (!queue) {
			state->finished = 1;
			return;
		}
		if (!push_to_int_q (x, y, queue)) {
			state->finished = 1;
			return;
		}
		reduceNeighbours(c, queue, sizeX, sizeY, t);
		deleteQueue(queue);
	}while (1);

	state->finished = 1;
	return;
}

int selectCellToCollapse(cell *c, int *x, int *y, int sizeX, int sizeY)
{
	int minEntropy = c[0].maxOptions;
	int_q_lnk *queue = initQueue();
	int cellsQueued = 0;
	/* loop every cell */
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			if (c[x + y * sizeX].collapsed) continue;
			c[x + y * sizeX].entropy = 0;
			for (int opt = 0; opt < c[0].maxOptions; opt++) {
				if (c[x + y * sizeX].options[opt] != 0) {
					c[x + y * sizeX].entropy += 1;
				}
			}
			if (c[x + y * sizeX].entropy < minEntropy) {
				emptyQueue(queue);
				cellsQueued = 1;
				push_to_int_q(x, y, queue);
				minEntropy = c[x + y * sizeX].entropy;
			}
			else if (c[x + y * sizeX].entropy == minEntropy) {
				push_to_int_q(x, y, queue);
				cellsQueued++;
			}
		}
	}
	if (cellsQueued == 0) {
		return 0;
	}
	srand(random);
	random = rand();
	int pickedCell = random % cellsQueued + 1;

	while (pickedCell > 0) {
		pop_head_int_q(x, y, queue);
		pickedCell--;
	}
	deleteQueue(queue);
	return 1;
}

void selectRandomTile(cell *c, int *tileNmb)
{
	c->entropy = 0;
	for (int opt = 0; opt < c->maxOptions; opt++) {
		if (c->options[opt] != 0) {
			c->entropy++;
		}
	}
	*tileNmb = -1;
	if (c->entropy == 0) {
		return;
	}
	int pick = random = rand();
	pick = pick % c->entropy;
	
	for (int opt = 0; opt < c->maxOptions; opt++) {
		if (c->options[opt] != 0) {
			if (pick == 0) {
				*tileNmb = opt;
				return;
			}
			pick--;
		}
	}
	return;
}

void initCells(cell **c, int width, int height, int maxTiles)
{
	if (*c != NULL) {
		deleteCells(c);
	}
	*c = (cell*)malloc(sizeof(cell) * width * height);
	if (*c == NULL) return;

	for (int h = 0; h < height; h++) {
		for(int w = 0; w < width; w++) {
			(*c)[w + h * width].collapsed = 0;
			(*c)[w + h * width].entropy = maxTiles;
			(*c)[w + h * width].m_tile = NULL;
			(*c)[w + h * width].maxOptions = maxTiles;
			(*c)[w + h * width].options = (int*)malloc(sizeof(int) * maxTiles);
			if ((*c)[w + h * width].options != NULL) {
				for (int i = 0; i < maxTiles; i++) {
					(*c)[w + h * width].options[i] = 1;
				}
			}
			(*c)[w + h * width].p.x = w;
			(*c)[w + h * width].p.y = h;
			(*c)[w + h * width].numCells = width * height;
		}
	}
	return;
}

void deleteCells(cell **c)
{
	if (c == NULL) return;
	for (int i = 0; i < (*c)[0].numCells; i++) {
		free((*c)[i].options);
		(*c)[i].options = NULL;
	}
	free(*c);
	*c = NULL;
	printf("Cells deleted\n");
	return;
}

void reduceNeighbours(cell *c, int_q_lnk *q, int sizeX, int sizeY, tile **lstTile)
{
	int x, y;
	while (pop_head_int_q(&x, &y, q)) {		
		int maxOptions = c[0].maxOptions;
		int *locOptionsNorth = (int*)calloc(maxOptions, sizeof(int));
		int *locOptionsEast = (int*)calloc(maxOptions, sizeof(int));
		int *locOptionsSouth = (int*)calloc(maxOptions, sizeof(int));
		int *locOptionsWest = (int*)calloc(maxOptions, sizeof(int));
		
		int iCurCell = x + y * sizeX;
		
		/* loop every option */
		for (int opt = 0; opt < maxOptions; opt++) {
			/* check, if tile no. opt is an option for the cell */
			if (c[iCurCell].options[opt] != 0) {
				/* loop over every tileOption */
				for (int iTileOpt = 0; iTileOpt < maxOptions; iTileOpt++) {
					if((*lstTile)[opt].optionNorth[iTileOpt] == 1) locOptionsNorth[iTileOpt] = 1;
					if((*lstTile)[opt].optionEast[iTileOpt] == 1) locOptionsEast[iTileOpt] = 1;
					if((*lstTile)[opt].optionSouth[iTileOpt] == 1) locOptionsSouth[iTileOpt] = 1;
					if((*lstTile)[opt].optionWest[iTileOpt] == 1) locOptionsWest[iTileOpt] = 1;
				}
			}
		}
		
		cell nCell;
		int newOption;
		/* to implement: adding changed cells to queue */
		/* compare to options north*/
		int changed = 0;
		if (!(y - 1 < 0)) {
			nCell = c[x + (y-1) * sizeX];
			if (!nCell.collapsed) {
				for (int opt = 0; opt < maxOptions; opt++) {
					newOption = nCell.options[opt] && locOptionsNorth[opt];
					if (nCell.options[opt] != newOption) {
						changed = 1;
					}
					nCell.options[opt] = newOption;
				}
				if (changed) {
					push_to_int_q (x, y - 1, q);
				}
			}
		}
		
		/* compare to options east*/
		changed = 0;
		if (!(x + 1 >= sizeX)) {
			nCell = c[(x + 1) + y * sizeX];
			if (!nCell.collapsed) {
				for (int opt = 0; opt < maxOptions; opt++) {
					newOption = nCell.options[opt] && locOptionsEast[opt];
					if (nCell.options[opt] != newOption) {
						changed = 1;
					}
					nCell.options[opt] = newOption;
				}
				if (changed) {
					push_to_int_q (x + 1, y, q);
				}
			}
		}
		
		/* compare to options south*/
		changed = 0;
		if (!(y + 1 >= sizeY)) {
			nCell = c[x + (y + 1) * sizeX];
			if (!nCell.collapsed) {
				for (int opt = 0; opt < maxOptions; opt++) {
					newOption = nCell.options[opt] && locOptionsSouth[opt];
					if (nCell.options[opt] != newOption) {
						changed = 1;
					}
					nCell.options[opt] = newOption;
				}
				if (changed) {
					push_to_int_q (x, y + 1, q);
				}
			}
		}
		
		/* compare to options west*/
		changed = 0;
		if (!(x - 1 < 0)) {
			nCell = c[(x - 1) + y * sizeX];
			if (!nCell.collapsed) {
				for (int opt = 0; opt < maxOptions; opt++) {
					newOption = nCell.options[opt] && locOptionsWest[opt];
					if (nCell.options[opt] != newOption) {
						changed = 1;
					}
					nCell.options[opt] = newOption;
				}
				if (changed) {
					push_to_int_q (x - 1, y, q);
				}
			}
		}
		
		free(locOptionsNorth);
		free(locOptionsEast);
		free(locOptionsSouth);
		free(locOptionsWest);
	}
	
	return;
}

void printAllCells(cell *c, int sizeX, int sizeY)
{
	int maxOptions = c[0].maxOptions;
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			/* printf("c[%2d|%2d]: ", x, y); */
			for (int opt = 0; opt < maxOptions; opt++) {
				printf("%d", c[x + y * sizeX].options[opt]);
			}
			printf(" | ");
		}
		printf("\n");
	}
	return;
}
