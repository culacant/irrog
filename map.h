#ifndef MAP_H
#define MAP_H

// for point struct
#include "actor.h"

// Tile Class
class Tile
{
private:
	bool m_bCanWalk;
	bool m_bCanSee;
	int m_iCover;
	int m_iConcealment;
	/*
	int m_iCoverHeight;
	int m_iConcealmentHeight;
	*/
	char m_cGraphic;
public:
	Tile();

	void setCanWalk(bool canwalk);
	void setCanSee(bool cansee);
	void setGraphic(char graphic);

	int Cover();
	void setCover(int cover);

	int Concealment();
	void setConcealment(int concealment);

	bool CanWalk();
	bool CanSee();
	char Graphic();
};

// Map Class
class Map
{
private:
	int m_iWidth;
	int m_iHeight;

	Tile *m_pTiles;
public:
	Map(int width, int height);
	~Map();

	int Width();
	int Height();
	Tile* TileAt(int x, int y);

	bool inLos(int ox, int oy, int tx, int ty);
	Point CalculateImpact(int ox, int oy, int tx, int ty);
	int getConcealment(int ox, int oy, int tx, int ty);
	bool canSee(Actor* actor1, Actor* actor2);



	void ReadMapFromFile();
};

#endif
