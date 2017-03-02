#include "map.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

// Tile Class

Tile::Tile()
{
	m_bCanWalk = true;
	m_bCanSee = true;
	m_cGraphic = '1';
	m_iCover = 0;
	m_iConcealment = 1;
}

void Tile::setCanWalk(bool canwalk)
{
	m_bCanWalk = canwalk;
}

void Tile::setCanSee(bool cansee)
{
	m_bCanSee= cansee;
}

void Tile::setGraphic(char graphic)
{
	m_cGraphic = graphic;
}

int Tile::Cover()
{
	return m_iCover;
}
void Tile::setCover(int cover)
{
	m_iCover = cover;
}

int Tile::Concealment()
{
	return m_iConcealment;
}
void Tile::setConcealment(int concealment)
{
	m_iConcealment = concealment;
}

bool Tile::CanWalk()
{
	return m_bCanWalk;
}

bool Tile::CanSee()
{
	return m_bCanSee;
}

char Tile::Graphic()
{
	return m_cGraphic;
}

// Map Class
Map::Map(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_pTiles = new Tile[m_iHeight * m_iWidth];
}

Map::~Map()
{
	delete [] m_pTiles;
}

int Map::Width()
{
	return m_iWidth;
}

int Map::Height()
{
	return m_iHeight;
}

Tile* Map::TileAt(int x, int y)
{
	return &m_pTiles[x + y * Width()];
}

bool Map::inLos(int ox, int oy, int tx, int ty)
{
    int delta_x(tx - ox);
    // if ox == tx, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;
 
    int delta_y(ty - oy);
    // if oy == ty, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;
 
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
 
        while (ox != tx)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                oy += iy;
            }
            // else do nothing
 
            error += delta_y;
            ox += ix;
 
			if(!TileAt(ox,oy)->CanSee())
				return false;
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
 
        while (oy != ty)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                ox += ix;
            }
            // else do nothing
 
            error += delta_x;
            oy += iy;
 
			if(!TileAt(ox,oy)->CanSee())
				return false;
        }
    }
	return true;
}
Point Map::CalculateImpact(int ox, int oy, int tx, int ty)
{
	float f_fRangeBonus = 0;
    int delta_x(tx - ox);
    // if ox == tx, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;
 
    int delta_y(ty - oy);
    // if oy == ty, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;
 
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
 
        while (ox != tx)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                oy += iy;
            }
            // else do nothing
 
            error += delta_y;
            ox += ix;
			if(f_fRangeBonus < 1)
				f_fRangeBonus += 0.1;
 
			if(TileAt(ox,oy)->Cover() * f_fRangeBonus > (rand()%100))
				return Point{ox,oy};
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
 
        while (oy != ty)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                ox += ix;
            }
            // else do nothing
 
            error += delta_x;
            oy += iy;
			if(f_fRangeBonus < 1)
				f_fRangeBonus += 0.1;
 
			if(TileAt(ox,oy)->Cover() * f_fRangeBonus > (rand()%100))
				return Point{ox,oy};
        }
    }
	return Point{ox,oy};
}
int Map::getConcealment(int ox, int oy, int tx, int ty)
{
	int f_iConcealment = 0;
    int delta_x(tx - ox);
    // if ox == tx, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;
 
    int delta_y(ty - oy);
    // if oy == ty, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;
 
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
 
        while (ox != tx)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                oy += iy;
            }
            // else do nothing
 
            error += delta_y;
            ox += ix;
 
			f_iConcealment += TileAt(ox,oy)->Concealment();
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
 
        while (oy != ty)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                ox += ix;
            }
            // else do nothing
 
            error += delta_x;
            oy += iy;
 
			f_iConcealment += TileAt(ox,oy)->Concealment();
        }
    }
	return f_iConcealment;
}

bool Map::canSee(Actor* actor1, Actor* actor2)
{
	if(getConcealment(actor1->getPosX(), actor1->getPosY(),actor2->getPosX(), actor2->getPosY()) > (rand()%100))
		return false;
	else
		return true;
}
void Map::ReadMapFromFile()
{
	std::ifstream infile("./map.txt");

	char c;

	int x = 0;
	int y = 0;

	while(infile.get(c))
	{
		switch(c)
		{
			case '1':
				TileAt(x,y)->setGraphic(c);
				TileAt(x,y)->setCanWalk(true);
				TileAt(x,y)->setCanSee(true);
				TileAt(x,y)->setCover(0);
				TileAt(x,y)->setConcealment(0);
				x++;
				break;
			case '2':
				TileAt(x,y)->setGraphic(c);
				TileAt(x,y)->setCanWalk(false);
				TileAt(x,y)->setCanSee(false);
				TileAt(x,y)->setCover(100);
				TileAt(x,y)->setConcealment(100);
				x++;
				break;
			case '3':
				TileAt(x,y)->setGraphic(c);
				TileAt(x,y)->setCanWalk(true);
				TileAt(x,y)->setCanSee(true);
				TileAt(x,y)->setCover(50);
				TileAt(x,y)->setConcealment(50);
				x++;
				break;
			case '\n':
				y++;
				x = 0;
				break;
		}
	}
	infile.close();
}
