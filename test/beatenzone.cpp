#include <termbox.h>
#include <cmath>
#include <algorithm>
#include <iostream>

int *tiles;

struct Point
{
	int X;
	int Y;
};

int main()
{
	tb_init();
	tb_event ev;

// init
	tiles = new int[40 * 40];
	for(int y = 0; y < 40;y++)
	{
		for(int x = 0; x < 40;x++)
		{
			if(x == 0 || x == 39 || y == 0 || y == 39)
				tiles[x + y * 40] = -5;
			else
				tiles[x + y * 40] = 0;
		}
	}
// beaten zone
// 10x1
	int ox = 1;
	int oy = 1;

	int px = 13;
	int py = 1;

	int lx = px + 10;
	int ly = py + 6;
	float db = 0.48;

std::cout << px << "," << py << std::endl;
std::cout << lx << "," << ly << std::endl;
	for(px = 13; px < lx;px++)
	{
		for(py = 1; py < ly ;py++)
		{
			tiles[px+py*40] = 1;

			int rx = std::cos(db) * (px-ox) - std::sin(db) * (py-oy) + ox;
			int ry = std::sin(db) * (px-ox) + std::cos(db) * (py-oy) + oy;
			tiles[rx+ry*40] = 2;
		}
	}
/*
	for(py;py < ly;py++)
	{
		for(px;px < lx;px++)
		{
			tiles[px+py*40] = 1;
			int rx = std::cos(db) * (px-ox) - std::sin(db) * (py-oy) + ox;
			int ry = std::sin(db) * (px-ox) + std::cos(db) * (py-oy) + oy;
			tiles[rx+ry*40] = 2;
		}
	}
*/
	tiles[ox+oy*40] = 1;


	while(1)
	{
// draw all
		for(int y = 0; y < 40;y++)
		{
			for(int x = 0; x < 40;x++)
			{
				if(tiles[x+y*40] == 1 || tiles[x+y*40] == 2)
					tb_change_cell(x,y,'0' + tiles[x+y*40],TB_RED,TB_GREEN);
				else
					tb_change_cell(x,y,'0' + tiles[x+y*40],TB_RED,TB_BLUE);
			}
		}

		tb_present();
		if(tb_poll_event(&ev))
		{
			if(ev.ch == 'q')
				goto end;
		}
	}
end:
	tb_shutdown();
}
