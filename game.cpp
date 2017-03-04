#include "game.h"

#include <math.h>
#include <iostream>

// Game Class

Game::Game()
{
	m_pMap = new Map(40,40);
	m_pInputHandler = new InputHandler();

	m_pMap->ReadMapFromFile();

	m_pPathfinder = new Pathfinder(m_pMap);
	
	m_pSelected = NULL;
	m_lActorList.push_back(new Actor(5,5,'@',1));
	m_lActorList.push_back(new Actor(6,5,'@',1));
	m_lActorList.push_back(new Actor(20,5,'@',2));

	m_pSelectedSquad = NULL;
	m_lSquadList.push_back(new Squad(1,1,1,1,m_pPathfinder));

	m_pActorAI = new ActorAI(m_pMap,m_pPathfinder);

	m_lBulletImpacts = new std::list<Impact>();
	m_pRenderer = new Renderer(m_pMap, &m_lActorList,NULL,m_lBulletImpacts, m_pInputHandler);


	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end(); ++it)
	{
		(*m_lSquadList.begin())->addToSquad((*it));
		std::cout << (*it)->getSquad() << std::endl;
	}
}

Game::~Game()
{
	delete m_pRenderer;
	delete m_pMap;
	delete m_pInputHandler;
	delete m_pPathfinder;
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end(); ++it)
	{
		delete (*it);
	}
	m_lActorList.clear();

	for(std::list<Squad*>::iterator it = m_lSquadList.begin(); it != m_lSquadList.end(); ++it)
	{
		delete (*it);
	}
	m_lSquadList.clear();

	delete m_pActorAI;
	delete m_lBulletImpacts;
}

void Game::Loop()
{
	static int f_iFrameTime = 0;
// inputs
	// renderer zoomlevel
	m_pRenderer->setZoomDelta(m_pInputHandler->MouseWheelDelta());
	// renderer viewport
	if(m_pInputHandler->IsKeyDown(KEY_VIEWPORT_UP))
		m_pRenderer->moveViewport(0,-5);
	if(m_pInputHandler->IsKeyDown(KEY_VIEWPORT_DOWN))
		m_pRenderer->moveViewport(0,5);
	if(m_pInputHandler->IsKeyDown(KEY_VIEWPORT_LEFT))
		m_pRenderer->moveViewport(-5,0);
	if(m_pInputHandler->IsKeyDown(KEY_VIEWPORT_RIGHT))
		m_pRenderer->moveViewport(5,0);
	// selector
	if(m_pInputHandler->MouseLeftButtonDown())
	{
		m_pRenderer->TileAt(m_pInputHandler->MousePositionX(),m_pInputHandler->MousePositionY());
		int x,y;
		x = m_pRenderer->TileXAt(m_pInputHandler->MousePositionX());
		y = m_pRenderer->TileYAt(m_pInputHandler->MousePositionY());
		m_pSelected = NULL;
		m_pSelectedSquad = NULL;
		for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end(); ++it)
		{
			if((*it)->getPosX() == x && (*it)->getPosY() == y )
			{
				m_pSelected = (*it);
				m_pSelectedSquad = (*it)->getSquad();
			}
		}
	}
	// facing set
	if(m_pInputHandler->IsKeyDown(KEY_SET_FACING))
	{
		if(m_pSelectedSquad)
		{
			int x,y;
			x = m_pRenderer->TileXAt(m_pInputHandler->MousePositionX());
			y = m_pRenderer->TileYAt(m_pInputHandler->MousePositionY());
			m_pSelectedSquad->setFacing(x,y);
		}
	}
	// target set
	if(m_pInputHandler->IsKeyDown(KEY_SET_TARGETPOS))
	{
		if(m_pSelectedSquad)
		{
			int x,y;
			x = m_pRenderer->TileXAt(m_pInputHandler->MousePositionX());
			y = m_pRenderer->TileYAt(m_pInputHandler->MousePositionY());
			if(m_pMap->inLos(m_pSelectedSquad->getPosX(), m_pSelectedSquad->getPosY(),x,y))
				m_pSelectedSquad->setTargetPos(x,y);
		}
	}
	// target clear
	if(m_pInputHandler->IsKeyDown(KEY_CLEAR_TARGETPOS))
	{
		if(m_pSelectedSquad)
			m_pSelectedSquad->clearTargetPos();
	}
	if(m_pInputHandler->IsKeyDown(KEY_MOVE))
	{
		if(m_pSelectedSquad)
		{
			int x,y;
			x = m_pRenderer->TileXAt(m_pInputHandler->MousePositionX());
			y = m_pRenderer->TileYAt(m_pInputHandler->MousePositionY());
			m_pSelectedSquad->setMoveTarget(x,y);
		}
	}
	if(m_pInputHandler->IsKeyDown(KEY_TEST))
	{
		if(m_pSelectedSquad)
			m_pSelectedSquad->Update(m_pActorAI);
	}
// ticking
	// selected actor
	if(m_pSelected)
	{
//		std::cout << m_pSelected->getWeapon()->getName() << std::endl;
// deselect if killed
		if(m_pSelected->isDead())
			m_pSelected = NULL;
	}
	// dont need to deselect dead squad

// update bulletlist
	m_lBulletImpacts->clear();
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end(); ++it)
	{
	//	m_pActorAI->UpdateActor((*it));
		(*it)->Update(f_iFrameTime);
		for(std::list<Impact>::iterator it2 = (*it)->getBulletImpacts()->begin(); it2 != (*it)->getBulletImpacts()->end(); ++it2)
		{
			Point f_pNewImpact = m_pMap->CalculateImpact((*it)->getPosX(),(*it)->getPosY(),(*it2).getPosX(),(*it2).getPosY());
			(*it2).setPosX(f_pNewImpact.X);
			(*it2).setPosY(f_pNewImpact.Y);
		}
		m_lBulletImpacts->splice(m_lBulletImpacts->end(),*(*it)->getBulletImpacts());
	}
// all actors
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end(); ++it)
	{
		(*it)->Update(f_iFrameTime);
		if((*it)->getTeam() == 1)
			(*it)->setVisible(true);
		else
			(*it)->setVisible(false);
		{
			for(std::list<Actor*>::iterator it2 = m_lActorList.begin(); it2 != m_lActorList.end(); ++it2)
			{
				if((*it2)->getTeam() == 1)
				{
					if(m_pMap->canSee((*it2),(*it)) && (*it2)->isFacing((*it)->getPosX(),(*it)->getPosY()))
					{
						(*it)->setVisible(true);
					}
				}
			}
		}
		
		if(!(*it)->isDead())
			for(std::list<Impact>::iterator it2 = m_lBulletImpacts->begin(); it2 != m_lBulletImpacts->end(); ++it2)
			{
				if((*it)->getPosX() == (*it2).getPosX() && (*it)->getPosY() == (*it2).getPosY())
					int a = 1;
//					(*it)->Kill();
				else if(getDistance((*it)->getPosX(),(*it)->getPosY(),(*it2).getPosX(),(*it2).getPosY()) < (*it2).getSupressionArea())
				{
					(*it)->Supress((*it2).getSupression());
				}
			}
	}
//update renderer;
	m_pRenderer->Update(m_pSelected, m_lBulletImpacts);

	f_iFrameTime = m_pRenderer->Draw();
}

int Game::getFastDistance(int x1, int y1, int x2, int y2)
{
	return pow(x2 - x1,2) + pow(y2 - y1,2);
}
int Game::getDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1,2) + pow(y2 - y1,2));
}

void Game::Run()
{
	while(1)
	{
		Loop();
		if(m_pInputHandler->IsKeyDown(KEY_QUIT))
		{
			return;
		}
	}
}
