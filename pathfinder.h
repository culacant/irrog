#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"
#include "actor.h"

#include <list>

// Pathmap Class
class Pathmap
{
private:
	int m_iWidth;
	int m_iHeight;

	int *m_pTiles;
	Map *m_pMap;
public:
	Pathmap(Map* map);
	~Pathmap();

	int *TileAt(int x, int y);

	int *NeighboursOf(int x, int y);
	Point getLowestNeighbour(int x, int y);

	int setValuesForPoint(int ox, int oy, int tx, int ty);

	int DistanceBetween(int ox, int oy, int tx, int ty);

};

// Pathfinder Class
class Pathfinder
{
private:
	Pathmap* m_pPathmap;
public:
	Pathfinder(Map * map);
	~Pathfinder();

	std::list<Point> *FindPath(int ox, int oy, int tx, int ty);
};

#endif
