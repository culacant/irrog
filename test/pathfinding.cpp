#include <termbox.h>
#include <cmath>
#include <algorithm>
#include <list>
#include <iostream>

int *tiles;

struct Point
{
	int X;
	int Y;
};

int *getneighbours(int x, int y, int *map)
{
	int *f_pNeighbours = new int[16];
	int i = 0;
	for(int dy = -1; dy <= 1; dy+=2)
	{
		f_pNeighbours[i] = map[x + (y+dy) * 10];
		f_pNeighbours[i+4] = x;
		f_pNeighbours[i+8] = y + dy;
		i++;
	}
	for(int dx = -1; dx <= 1; dx+=2)
	{
		f_pNeighbours[i] = map[(x+dx) + y * 10];
		f_pNeighbours[i+4] = x+dx;
		f_pNeighbours[i+8] = y;
		i++;
	}
	return f_pNeighbours;

}

Point getlowestneighbour(int x, int y ,int *map)
{
	Point point;
	int *neighbours = getneighbours(x,y,map);
	int lowestval = 500;
	for(int i = 0;i<4;i++)
	{
		if(neighbours[i] < lowestval)
		{
			lowestval = neighbours[i];
			point = {neighbours[i+4],neighbours[i+8]};
		}
	}

	delete []neighbours;
	return point;
}

std::list<Point> *genpath(int ox, int oy, int tx, int ty, int *m_pPathmap)
{
	std::list<Point> *f_lPath = new std::list<Point>();
	Point f_pPoint = {tx,ty};

	f_lPath->push_back(f_pPoint);

	while(!(f_pPoint.X == ox && f_pPoint.Y == oy))
	{
		f_pPoint = getlowestneighbour(f_pPoint.X,f_pPoint.Y,m_pPathmap);
		f_lPath->push_back(f_pPoint);
	}

	return f_lPath;
}

void setvaluesfor(int ox, int oy, int tx, int ty, int *m_pPathmap)
{
// get neighbours
// if 0, set value + 1
// push onto list

// value +=1
// repeat for every point on list
	std::list<Point> f_lTodo;
	int f_iCurrentValue = 0;
	Point f_pCurrent;

	m_pPathmap[ox + oy * 10] = 1;

	f_lTodo.push_front({ox,oy});
	while(!f_lTodo.empty())
	{
		f_pCurrent = {(*f_lTodo.begin()).X,(*f_lTodo.begin()).Y};
		f_lTodo.pop_front();
		
		int *f_pNeighbours = getneighbours(f_pCurrent.X,f_pCurrent.Y,m_pPathmap);
		for(int i = 0; i < 4; i++)
		{
			if(f_pNeighbours[i] == 0)
			{
				m_pPathmap[f_pNeighbours[i+4] + f_pNeighbours[i+8] * 10] = m_pPathmap[f_pCurrent.X + f_pCurrent.Y*10]+ 1;
				f_lTodo.push_back({f_pNeighbours[i+4],f_pNeighbours[i+8]});
			}
		}
		f_iCurrentValue += 1;
for(int y = 0; y < 10;y++)
{
	for(int x = 0; x < 10;x++)
	{
		std::cout << tiles[x+y*10] << ',';
	}
	std::cout << std::endl;
}
std::cout << std::endl;
	}
}

int main()
{
	tb_init();
	tb_event ev;

	int ox = 2;
	int oy = 2;
	int tx = 4;
	int ty = 4;

	std::list<Point> *path;


// init
	tiles = new int[10 * 10];
	for(int y = 0; y < 10;y++)
	{
		for(int x = 0; x < 10;x++)
		{
			if(x == 0 || x == 9 || y == 0 || y == 9)
				tiles[x + y * 10] = -5;
			else
				tiles[x + y * 10] = 0;
		}
	}
// values
	setvaluesfor(ox,oy,tx,ty,tiles);

// timeout
	while(1)
	{
// draw all
		for(int y = 0; y < 10;y++)
		{
			for(int x = 0; x < 10;x++)
			{
				tb_change_cell(x,y,'0' + tiles[x+y*10],TB_RED,TB_BLUE);
				std::cout << tiles[x+y*10] << ',';
			}
			std::cout << std::endl;
		}
// get path
		path = genpath(ox,oy,tx,ty,tiles);
		for(std::list<Point>::iterator it = path->begin();it != path->end();it++)
		{
			tb_change_cell((*it).X,(*it).Y,'x',TB_YELLOW,TB_BLACK);
		}
// draw path

		tb_change_cell(ox,oy,'o',TB_YELLOW,TB_BLACK);
		tb_change_cell(tx,ty,'d',TB_YELLOW,TB_BLACK);



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
