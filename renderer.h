#ifndef RENDERER_H
#define RENDERER_H

#include "map.h"
#include "actor.h"

#include <irrlicht/irrlicht.h>
#include <list>

using namespace irr;

// Renderer Class
class Renderer
{
private:
	Map* m_pMap;
	std::list<Actor*> *m_lActorList;
	Actor *m_pSelected;
	std::list<Impact> *m_lBulletImpacts;

	int m_iViewportX;
	int m_iViewportY;

	int m_iSpriteSize;

	int m_iTime;

	IrrlichtDevice *m_pDevice;
	video::IVideoDriver *m_pDriver;

	video::ITexture *m_pTileset;
public:
	Renderer(Map* map, std::list<Actor*> *actorlist, Actor *selected, std::list<Impact> *impactlist,IEventReceiver *inputhandler);
	~Renderer();

	void Update(Actor *selected, std::list<Impact> *impactlist);

	void moveViewport(int dx, int dy);
	void setZoomDelta(int zoomdelta);

	Tile* TileAt(int x, int y);
	int TileXAt(int x);
	int TileYAt(int y);
	
	int ViewportX();
	int ViewportY();
	int ZoomLevel();

	int Draw();
};

#endif
