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