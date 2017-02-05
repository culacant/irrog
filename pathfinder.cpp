#include "pathfinder.h"

#include <iostream>
#include <cmath>
#include <algorithm>

#define MOVEPOINTS 4

// Pathmap Class
Pathmap::Pathmap(Map* map)
{
	m_iWidth = map->Width();
	m_iHeight = map->Height();
	m_pTiles = new int [m_iWidth * m_iHeight];
	m_pMap = map;
	for(int y = 0; y < m_iHeight; y++)
	{
		for(int x = 0; x < m_iWidth; x++)
		{
			if(m_pMap->TileAt(x,y)->CanWalk())
				m_pTiles[x + y * m_iWidth] = 0;
			else
				m_pTiles[x + y * m_iWidth] = 0x7FFF;
		}
	}
}

Pathmap::~Pathmap()
{
	delete []m_pTiles;
}

int *Pathmap::TileAt(int x, int y)
{
	return &m_pTiles[x + y * m_iWidth];
}

int *Pathmap::NeighboursOf(int x, int y)
{
    int *f_pNeighbours = new int[16];
    int i = 0;
    for(int dy = -1; dy <= 1; dy+=2)
    {
        f_pNeighbours[i] = m_pTiles[x + (y+dy) * 40];
        f_pNeighbours[i+4] = x;
        f_pNeighbours[i+8] = y + dy;
        i++;
    }
    for(int dx = -1; dx <= 1; dx+=2)
    {
        f_pNeighbours[i] = m_pTiles[(x+dx) + y * 40];
        f_pNeighbours[i+4] = x+dx;
        f_pNeighbours[i+8] = y;
        i++;
    }
    return f_pNeighbours;

}

Point Pathmap::getLowestNeighbour(int x, int y)
{
	Point f_pPoint;
	int *f_pNeighbours = NeighboursOf(x,y);
	int f_iLowestValue = 0x7FFF;
	for(int i = 0; i < 4;i++)
	{
		if(f_pNeighbours[i] < f_iLowestValue)
		{
			f_iLowestValue = f_pNeighbours[i];
			f_pPoint = {(int)f_pNeighbours[i+4],(int)f_pNeighbours[i+8]};
		}
	}
	delete[] f_pNeighbours;
	return f_pPoint;
}

int Pathmap::setValuesForPoint(int ox, int oy, int tx, int ty)
{

    std::list<Point> f_lTodo;
    int f_iCurrentValue = 0;
    Point f_pCurrent;

// set values to 0
	for(int y = 0; y < m_iHeight; y++)
	{
		for(int x = 0; x < m_iWidth; x++)
		{
			if(m_pMap->TileAt(x,y)->CanWalk())
				m_pTiles[x + y * m_iWidth] = 0;
			else
				m_pTiles[x + y * m_iWidth] = 0x7FFF;
		}
	}

    (*TileAt(ox,oy)) = 1;


    f_lTodo.push_front({ox,oy});
    while(!f_lTodo.empty())
    {
        f_pCurrent = {(*f_lTodo.begin()).X,(*f_lTodo.begin()).Y};
        f_lTodo.pop_front();

        int *f_pNeighbours = NeighboursOf(f_pCurrent.X,f_pCurrent.Y);
        for(int i = 0; i < 4; i++)
        {
            if(f_pNeighbours[i] == 0)
            {
                (*TileAt(f_pNeighbours[i+4],f_pNeighbours[i+8])) = (*TileAt(f_pCurrent.X,f_pCurrent.Y))+ 1;
                f_lTodo.push_back({f_pNeighbours[i+4],f_pNeighbours[i+8]});
            }
        }
	}
	return 1;
}

int Pathmap::DistanceBetween(int ox, int oy, int tx, int ty)
{
// this one will get stuck real easy
//	return std::max(std::abs(ox - tx),std::abs(oy - ty));
// this one too
	return pow(ox - tx,2)+pow(oy - ty,2);
}

// Pathfinder Class
Pathfinder::Pathfinder(Map *map)
{
	m_pPathmap = new Pathmap(map);
}
Pathfinder::~Pathfinder()
{
	delete m_pPathmap;
}
std::list<Point> *Pathfinder::FindPath(int ox, int oy, int tx, int ty)
{
	std::list<Point> *f_lPath = new std::list<Point>();

	int i = m_pPathmap->setValuesForPoint(ox,oy,tx,ty);

	Point f_pPoint = {tx,ty};

	f_lPath->push_front(f_pPoint);

	while(!(f_pPoint.X == ox && f_pPoint.Y == oy))
    {
        f_pPoint = m_pPathmap->getLowestNeighbour(f_pPoint.X,f_pPoint.Y);
        f_lPath->push_front(f_pPoint);
    }
	f_lPath->pop_front();

	return f_lPath;
}
