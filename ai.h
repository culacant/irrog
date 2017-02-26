#ifndef AI_H
#define AI_H

#include "map.h"
#include "actor.h"
#include "pathfinder.h"

// class ActorAI
class ActorAI
{
private:
	Pathfinder *m_pPathfinder;
	Map *m_pMap;

	void SeekCover(Actor *actor, int radius);
public:
	ActorAI(Map *map, Pathfinder *pathfinder);
	void UpdateActor(Actor *actor);
};

#endif
