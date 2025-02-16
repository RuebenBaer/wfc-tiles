#include "wfc-core.h"

void initCell(cell *c)
{
	c->collapsed = 0;
}

void collapseCell(cell *c, tile *t, pos p)
{
	c->m_tile = t;
	c->p.x = p.x;
	c->p.y = p.y;
	c->collapsed = 1;
	c->options.ListeLeeren("collapseCell");
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
}

void initTiles(tile *t, int maxTiles)
{
	t = new tile[maxTiles];
	for (int i = 0; i < maxTiles; i++) {
		t[i].maxTiles = maxTiles;
		t[i].optionNorth = new int[maxTiles];
		t[i].optionSouth = new int[maxTiles];
		t[i].optionEast = new int[maxTiles];
		t[i].optionWest = new int[maxTiles];
	}
}

void deleteTiles(tile *t)
{
	for (int i = 0; i < t[i].maxTiles; i++) {
		delete []t[i].optionNorth;
		delete []t[i].optionSouth;
		delete []t[i].optionEast;
		delete []t[i].optionWest;
	}
	delete []t;
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
		if (t_tiles[thisIndex].data[(2 + h * 3) * 3 + 0] != t_tiles[otherIndex].data[(0 + h * 3) * 3 + 0]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(2 + h * 3) * 3 + 1] != t_tiles[otherIndex].data[(0 + h * 3) * 3 + 1]) {
			isOption = 0;
			break;
		}
		if (t_tiles[thisIndex].data[(2+ h * 3) * 3 + 2] != t_tiles[otherIndex].data[(0 + h * 3) * 3 + 2]) {
			isOption = 0;
			break;
		}
	}
	t_tiles[thisIndex].optionWest[otherIndex] = isOption;
	/* END optionWest */
	return;
}