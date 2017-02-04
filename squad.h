#ifndef SQUAD_H
#define SQUAD_H

#include "actor.h"

#include <list>

// squad class

class Squad
{
private:
	std::list<Actor*> m_lActorList;
	
	int m_iTeam;

	int m_iPosX;
	int m_iPosY;
public:
	Squad(int posx, int posy, int size, int team);
	~Squad();

	int getTeam();
	void setMoveTarget(int x, int y);
	void setFireTarget(int x, int y);
//	std::list<Actor*>
};

#endif
