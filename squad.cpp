#include "squad.h"

// Squad Class

Squad::Squad(int posx, int posy, int size, int team, Pathfinder* pathfinder)
{
	m_pPathfinder = pathfinder;
	m_iPosX = posx;
	m_iPosY = posy;
	m_iTeam = team;
	m_iInterval = 2;

	int i = 0;
//	while(i < size)
//	{
//		m_lActorList.push_back(new Actor(m_iPosX + i,m_iPosY,'@',m_iTeam));
//		i++;
//	}
}
Squad::~Squad()
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
		delete (*it);
	}
	m_lActorList.clear();
}

void Squad::Update(ActorAI* ai)
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
		if((*it)->isDead())
		{
			it = m_lActorList.erase(it);
		}
		ai->UpdateActor((*it));
	}
}

int Squad::getTeam()
{
	return m_iTeam;
}

void Squad::setMoveTarget(int x, int y)
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
		std::list<Point> *f_pPath = m_pPathfinder->FindPath((*it)->getPosX(),(*it)->getPosY(),x,y);
		if(f_pPath)
			(*it)->setPath(f_pPath);
	}
}

void Squad::setTargetPos(int x, int y)
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
		(*it)->setTargetPos(x, y);
	}
}
int Squad::getTargetPosX()
{
	return (*m_lActorList.begin())->getTargetPosX();
}
int Squad::getTargetPosY()
{
	return (*m_lActorList.begin())->getTargetPosY();
}
void Squad::clearTargetPos()
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
		(*it)->clearTargetPos();
	}
}

void Squad::addToSquad(Actor *actor)
{
	if(m_iTeam == actor->getTeam())
	{
		actor->setSquad(this);
		m_lActorList.push_back(actor);
	}
	else
		std::cout << "Wrong team!" << std::endl;
}

int Squad::getPosX()
{
	return m_iPosX;
}
int Squad::getPosY()
{
	return m_iPosY;
}

void Squad::setFacing(int x, int y)
{
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end();++it)
	{
		(*it)->setFacing(x,y);
	}
}
