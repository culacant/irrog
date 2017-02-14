#ifndef RENDERER_H
#define RENDERER_H

#include "map.h"
#include "actor.h"

#include <irrlicht/irrlicht.h>
#include <list>

using namespace irr;

// 3dMap Class
class Mapd: public scene::ISceneNode
{
private:
	core::aabbox3d<f32> m_aBoundingBox;
	video::S3DVertex *m_vVertices;
	video::SMaterial m_mMaterial;

	int m_iWidth;
	int m_iHeight;
public:
	Mapd(Map *map, scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
	~Mapd();

	virtual void OnRegisterSceneNode();
	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);
};

// renderunit
class RenderUnit
{
public:
	Actor* Unit;
	scene::ISceneNode* Model;
};

// Renderer Class
class Renderer
{
private:
	Map* m_pMap;
	std::list<Actor*> *m_lActorList;
	std::list<scene::ISceneNode*> *m_lModelList;

	Actor *m_pSelected;
	std::list<Impact> *m_lBulletImpacts;

	int m_iViewportX;
	int m_iViewportY;

	int m_iSpriteSize;

	int m_iTime;

	IrrlichtDevice *m_pDevice;
	video::IVideoDriver *m_pDriver;
	scene::ISceneManager *m_pSceneManager;

	Mapd *m_pMapd;

public:
	Renderer(Map* map, std::list<Actor*> *actorlist, Actor *selected, std::list<Impact> *impactlist,IEventReceiver *inputhandler);
	~Renderer();

	void Update(Actor *selected, std::list<Impact> *impactlist);

// not used for 3d
	void moveViewport(int dx, int dy);
	void setZoomDelta(int zoomdelta);

	int ViewportX();
	int ViewportY();
	int ZoomLevel();

	Tile* TileAt(int x, int y);
	int TileXAt(int x);
	int TileYAt(int y);
	

	int Draw();
};

#endif
