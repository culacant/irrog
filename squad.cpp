#include "squad.h"

// Squad Class

Squad::Squad(int posx, int posy, int size, int team)
{
	m_iPosX = posx;
	m_iPosY = posy;
	m_iTeam = team;

	int i = 0;
	while(i < size)
	{
		m_lActorList.push_back(new Actor(m_iPosX + i,m_iPosY,'@',m_iTeam));
		i++;
	}
}
Squad::~Squad()
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
		delete (*it);
	}
	m_lActorList.clear();
}

int Squad::getTeam()
{
	return m_iTeam;
}

void Squad::setMoveTarget(int x, int y)
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
//		std::list<Point> *f_pPath = m_pPathfinder->FindPath(m_pSelected->getPosX(),m_pSelected->getPosY(),x,y);
//		if(f_pPath)
//			(*it)->setPath(f_pPath);
	}
}

void Squad::setFireTarget(int x, int y)
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
		(*it)->setTargetPos(x, y);
	}
}
