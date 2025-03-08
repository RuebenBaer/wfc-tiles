#include "wfc-core.h"
#include <stdlib.h>
#include <stdio.h>

void findTileOptions(tile *lstTile, int nmbTile);
void fillTileOptions(tile *lstTile, int ithis, int iother);
void collapseCell(cell *c, tile *t, int tileNmb, pos p);
void drawCell(cell *c, unsigned char* canvas, int width, int height);
void reduceNeighbours(cell *c, int x, int y, int sizeX, int sizeY, tile **lstTile);

void initCell(cell *c)
{
	c->collapsed = 0;
}

void collapseCell(cell *c, tile *t, int tileNmb, pos p)
{
	c->m_tile = t;
	c->p.x = p.x;
	c->p.y = p.y;
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
	if((c->p.x + 3) >= width || (c->p.y + 3) >= height) {
		printf("Cell (%d, %d) outside canvas (%d, %d)!\n", c->p.x, c->p.y, width, height);
		return;
	}
	tile *t = c->m_tile;
	int curpos;
	for (int h = 0; h < 3; h++) {
		for (int w = 0; w < 3; w++) {
			curpos = ((c->p.x * 3 + w) + (c->p.y * 3 + h) * width) * 3;
			canvas[curpos + 0] = t->data[(w + h * 3) * 3 + 0];
			canvas[curpos + 1] = t->data[(w + h * 3) * 3 + 1];
			canvas[curpos + 2] = t->data[(w + h * 3) * 3 + 2];
		}
	}
	return;
}

void initTiles(tile **t, int *numTiles, unsigned char *picData, int imgWidth, int imgHeight)
{
	int maxTiles = *numTiles = imgWidth / 3;

	tile *tempT = (tile*) malloc(sizeof(tile) * maxTiles);
	for (int i = 0; i < maxTiles; i++) {
		tempT[i].maxTiles = maxTiles;
		tempT[i].optionNorth = (int*) malloc(sizeof(int) * maxTiles);
		tempT[i].optionSouth = (int*) malloc(sizeof(int) * maxTiles);
		tempT[i].optionEast = (int*) malloc(sizeof(int) * maxTiles);
		tempT[i].optionWest = (int*) malloc(sizeof(int) * maxTiles);
	}
	*t = tempT;
	
	for (int i = 0; i < maxTiles; i++) {
		for (int w = 0; w < 3; w++) {
			for (int h = 0; h < 3; h++) {
				(*t)[i].data[(w + h * 3) * 3 + 0] = picData[((i * 3 + w) + h * imgWidth) * 3 + 0];
				(*t)[i].data[(w + h * 3) * 3 + 1] = picData[((i * 3 + w) + h * imgWidth) * 3 + 1];
				(*t)[i].data[(w + h * 3) * 3 + 2] = picData[((i * 3 + w) + h * imgWidth) * 3 + 2];
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
		printf("Option (%d):", ithis);
		for (int iother = 0; iother < nmbTile; iother++) {
			fillTileOptions(lstTile, ithis, iother);
			printf(" %d|%d|%d|%d", lstTile[ithis].optionNorth[iother], lstTile[ithis].optionEast[iother], lstTile[ithis].optionSouth[iother], lstTile[ithis].optionWest[iother]);
		}
		printf("\n");
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

void collapseGrid(cell *c, int sizeX, int sizeY, tile **t, int *maxTiles, unsigned char *canvasData, int canvasWidth, int canvasHeight)
{
	int i = rand() % *maxTiles;
	printf("maxTiles: %d\n", *maxTiles);
	tile curTile = (*t)[i];
	
	printf("collapse to tile t (%p) =-%d-=> curTile (%p) offset %llu\n", (void*)&t, i, (void*)&curTile, sizeof(tile));
	
	pos p;
	p.x = rand() % sizeX;
	p.y = rand() % sizeY;
	
	collapseCell(&c[p.x + p.y * sizeX], &curTile, i, p);
	drawCell(&c[p.x + p.y * sizeX], canvasData, canvasWidth, canvasHeight);
	
	reduceNeighbours(c, p.x, p.y, sizeX, sizeY, t);
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

void reduceNeighbours(cell *c, int x, int y, int sizeX, int sizeY, tile **lstTile)
{
	int maxOptions = c[0].maxOptions;
	printf("maxOptions = %d\n", maxOptions);
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
	
	
	
	
	free(locOptionsNorth);
	free(locOptionsEast);
	free(locOptionsSouth);
	free(locOptionsWest);
	return;
}