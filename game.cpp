#include "game.h"

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
	m_lActorList.push_back(new Actor(20,5,'@',2));

	m_lBulletImpacts = new std::list<Impact>();
	m_pRenderer = new Renderer(m_pMap, &m_lActorList,NULL,m_lBulletImpacts, m_pInputHandler);
}

Game::~Game()
{
	delete m_pRenderer;
	delete m_pMap;
	delete m_pInputHandler;
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end(); ++it)
	{
		delete (*it);
	}
	m_lActorList.clear();
	m_lBulletImpacts->clear();
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
		int x,y;
		x = m_pRenderer->TileXAt(m_pInputHandler->MousePositionX());
		y = m_pRenderer->TileYAt(m_pInputHandler->MousePositionY());
		m_pSelected = NULL;
		for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end(); ++it)
		{
			if((*it)->getPosX() == x && (*it)->getPosY() == y )
				m_pSelected = (*it);
		}
	}
	// target set
	if(m_pInputHandler->IsKeyDown(KEY_SET_TARGETPOS))
	{
		if(m_pSelected)
		{
			int x,y;
			x = m_pRenderer->TileXAt(m_pInputHandler->MousePositionX());
			y = m_pRenderer->TileYAt(m_pInputHandler->MousePositionY());
			if(m_pMap->inLos(m_pSelected->getPosX(), m_pSelected->getPosY(),x,y))
				m_pSelected->setTargetPos(x,y);
		}
	}
	// target clear
	if(m_pInputHandler->IsKeyDown(KEY_CLEAR_TARGETPOS))
	{
		if(m_pSelected)
			m_pSelected->clearTargetPos();
	}
	if(m_pInputHandler->IsKeyDown(KEY_MOVE))
	{
		if(m_pSelected)
		{
			int x,y;
			x = m_pRenderer->TileXAt(m_pInputHandler->MousePositionX());
			y = m_pRenderer->TileYAt(m_pInputHandler->MousePositionY());

			std::list<Point> *f_pPath = m_pPathfinder->FindPath(m_pSelected->getPosX(),m_pSelected->getPosY(),x,y);
			if(f_pPath)
				m_pSelected->setPath(f_pPath);
		}
	}
	if(m_pInputHandler->IsKeyDown(KEY_TEST))
	{
		int x,y;
		x = m_pRenderer->TileXAt(m_pInputHandler->MousePositionX());
		y = m_pRenderer->TileYAt(m_pInputHandler->MousePositionY());
		m_pPathfinder->FindPath(x,y,0,0);
	}
// ticking
	// selected actor
	if(m_pSelected)
	{
// deselect if killed
		if(m_pSelected->isDead())
			m_pSelected = NULL;
	}

// update bulletlist
	m_lBulletImpacts->clear();
	for(std::list<Actor*>::iterator it = m_lActorList.begin(); it != m_lActorList.end(); ++it)
	{
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
					if(m_pMap->canSee((*it2),(*it)))
					{
						std::cout << m_pMap->canSee((*it2),(*it)) << std::endl;

						(*it)->setVisible(true);
					}
				}
			}
		}
		
		if(!(*it)->isDead())
			for(std::list<Impact>::iterator it2 = m_lBulletImpacts->begin(); it2 != m_lBulletImpacts->end(); ++it2)
			{
				if((*it)->getPosX() == (*it2).getPosX() && (*it)->getPosY() == (*it2).getPosY())
					(*it)->Kill();
			}
	}
//update renderer;
	m_pRenderer->Update(m_pSelected, m_lBulletImpacts);

	f_iFrameTime = m_pRenderer->Draw();
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
