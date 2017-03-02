#include "ai.h"
#include <iostream>

// class ActorAI

ActorAI::ActorAI(Map *map, Pathfinder *pathfinder)
{
	m_pMap = map;
	m_pPathfinder = pathfinder;
}

void ActorAI::UpdateActor(Actor *actor)
{
//	if(actor->getState() == IDLE && !actor->inCover())
//	{
		SeekCover(actor, 10);
//		actor->setCover(true);
//	}
	std::cout << actor->getState() << std::endl;
	return;
}

void ActorAI::SeekCover(Actor *actor, int radius)
{
	int x;
	int y;

	int xmin = -(radius/2);
	int xmax = radius/2;

	int ymin = -(radius/2);
	int ymax = radius/2;

	int xfinal;
	int yfinal;

// go around actor
	Point f_pFacing = actor->getFacingDir();

	int f_iBestScore = 0;
	Point f_pBestTile = {actor->getPosX(),actor->getPosY()};

	for(int y = ymin; y < ymax;y++)
	{
		for(int x = xmin; x < xmax;x++)
		{
			xfinal = x + actor->getPosX();
			yfinal = y + actor->getPosY();
			
			if(xfinal < 0)
				xfinal = 0;
			if(xfinal > m_pMap->Width())
				xfinal = m_pMap->Width();
			if(yfinal < 0)
				yfinal = 0;
			if(yfinal > m_pMap->Height())
				yfinal = m_pMap->Height();

			if(m_pMap->TileAt(xfinal,yfinal)->CanWalk())
			{
				int score = m_pMap->TileAt(xfinal+f_pFacing.X,yfinal+f_pFacing.Y)->Cover() + m_pMap->TileAt(xfinal+f_pFacing.X,yfinal+f_pFacing.Y)->Concealment()/2;
				if(score > f_iBestScore)
				{
					f_iBestScore = score;
					f_pBestTile = {xfinal,yfinal};
				}
			}
		}
	}
	std::list<Point> *f_pPath = m_pPathfinder->FindPath(actor->getPosX(),actor->getPosY(),f_pBestTile.X,f_pBestTile.Y);
	if(f_pPath)
		actor->setPath(f_pPath);
	return;
}
