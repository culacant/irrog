#ifndef SQUAD_H
#define SQUAD_H

#include "actor.h"
#include "ai.h"
#include "pathfinder.h"

#include <list>
#include <iostream>

// squad class

class Squad
{
private:
	std::list<Actor*> m_lActorList;

	Pathfinder* m_pPathfinder;
	
	int m_iTeam;

	int m_iInterval;

	int m_iPosX;
	int m_iPosY;
public:
	Squad(int posx, int posy, int size, int team, Pathfinder* pathfinder);
	~Squad();

	void Update(ActorAI* ai);

	int getTeam();
	void setMoveTarget(int x, int y);

	void setTargetPos(int x, int y);
	int getTargetPosX();
	int getTargetPosY();
	void clearTargetPos();

	void addToSquad(Actor *actor);

	int getPosX();
	int getPosY();

	void setFacing(int x, int y);
//	std::list<Actor*>
};

#endif
