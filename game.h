#ifndef GAME_H
#define GAME_H

#include "ai.h"
#include "actor.h"
#include "map.h"
#include "renderer.h"
#include "input.h"
#include "pathfinder.h"

#include <list>

// Game Class
class Game
{
private:
	Renderer *m_pRenderer;
	Map *m_pMap;
	InputHandler *m_pInputHandler;
	Pathfinder *m_pPathfinder;

	Actor* m_pSelected;
	std::list<Actor*> m_lActorList;

	ActorAI *m_pActorAI;

	std::list<Impact> *m_lBulletImpacts;

	void Loop();
public:
	Game();
	~Game();

	int getDistance(int x1, int y1, int x2, int y2);
	int getFastDistance(int x1, int y1, int x2, int y2);

	void Run();
};

#endif
