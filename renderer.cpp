#include "renderer.h"

#include <unistd.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Renderer Class
Renderer::Renderer(Map* map, std::list<Actor*> *actorlist, Actor* selected,std::list<Impact> *impactlist, IEventReceiver *inputhandler)
{
	m_pMap = map;

	m_lActorList = actorlist;
	m_pSelected = selected;
	m_lBulletImpacts = impactlist;

	m_pDevice = createDevice(video::EDT_OPENGL,core::dimension2d<u32>(WINDOW_WIDTH, WINDOW_HEIGHT),16,false,false,false, inputhandler);
	m_pDriver = m_pDevice->getVideoDriver();

	m_pTileset = m_pDriver ->getTexture("./tiles.png");
	m_pDriver->makeColorKeyTexture(m_pTileset,video::SColor(0,255,0,255));
	m_pDevice->run();

	m_iViewportX = 0;
	m_iViewportY = 0;
	m_iSpriteSize = 32;

	m_iTime = 0;
}

Renderer::~Renderer()
{
	m_pDevice->drop();
}

void Renderer::moveViewport(int dx, int dy)
{
	m_iViewportX += dx;
	m_iViewportY += dy;
	if(m_iViewportX < 0)
		m_iViewportX = 0;
	if(m_iViewportY < 0)
		m_iViewportY = 0;
	if(m_iViewportX > m_pMap->Width() * m_iSpriteSize - WINDOW_WIDTH)
			m_iViewportX = m_pMap->Width() * m_iSpriteSize - WINDOW_WIDTH;
	if(m_iViewportY > m_pMap->Height() * m_iSpriteSize - WINDOW_HEIGHT)
			m_iViewportY = m_pMap->Height() * m_iSpriteSize - WINDOW_HEIGHT;
}

void Renderer::setZoomDelta(int zoomdelta)
{
	m_iSpriteSize += zoomdelta;
}

Tile* Renderer::TileAt(int x, int y)
{
	int mapx = (x + m_iViewportX) / m_iSpriteSize;
	int mapy = (y + m_iViewportY) / m_iSpriteSize;
	return m_pMap->TileAt(mapx,mapy);
}
int Renderer::TileXAt(int x)
{
	return (x + m_iViewportX) / m_iSpriteSize;
}
int Renderer::TileYAt(int y)
{
	return (y + m_iViewportY) / m_iSpriteSize;
}

int Renderer::ViewportX()
{
	return m_iViewportX;
}

int Renderer::ViewportY()
{
	return m_iViewportY;
}

int Renderer::ZoomLevel()
{
	return m_iSpriteSize;
}

void Renderer::Update(Actor *selected, std::list<Impact> *impactlist)
{
	m_pSelected = selected;
	m_lBulletImpacts = impactlist;
}

int Renderer::Draw()
{
	int f_iNewTime = m_pDevice->getTimer()->getTime();
	int f_iRealTime = f_iNewTime - m_iTime;
	m_iTime = f_iNewTime;
	core::rect<s32> f_rSpriterect;
	if(m_pDevice->run() && m_pDevice->isWindowActive())
	{
		m_pDriver->beginScene(true,true,video::SColor(255,120,120,136));
// map
		for(int y = 0; y < m_pMap->Height(); y++)
		{
			for(int x = 0; x < m_pMap->Width(); x++)
			{
				switch(m_pMap->TileAt(x,y)->Graphic())
				{
					case '1':
						f_rSpriterect = core::rect<s32>(0,0,32,32);
						break;
					case '2':
						f_rSpriterect = core::rect<s32>(32 + 1,0,64,32);
						break;
					case '3':
						f_rSpriterect = core::rect<s32>(225 + 1,0,255,32);
						break;
				}

				m_pDriver->draw2DImage(m_pTileset, core::rect<s32>((x * m_iSpriteSize) - m_iViewportX,(y * m_iSpriteSize) - m_iViewportY,(x * m_iSpriteSize) - m_iViewportX + m_iSpriteSize,(y * m_iSpriteSize) - m_iViewportY + m_iSpriteSize),f_rSpriterect);
			}
		}
// actors
		for(std::list<Actor*>::iterator it = m_lActorList->begin(); it != m_lActorList->end(); ++it)
		{
			if((*it)->Visible())
			{
			switch(((*it))->Graphic())
			{
				case '@':
					f_rSpriterect = core::rect<s32>(64 + 1,0,96,32);
					break;
				case 'x':
					f_rSpriterect = core::rect<s32>(192 + 1 + 1,0,224,32);
					break;
			}
			m_pDriver->draw2DImage(m_pTileset, core::rect<s32>((((*it))->getPosX() *m_iSpriteSize) - m_iViewportX,(((*it))->getPosY() *m_iSpriteSize) - m_iViewportY,(((*it))->getPosX() *m_iSpriteSize) - m_iViewportX +m_iSpriteSize,(((*it))->getPosY() *m_iSpriteSize) - m_iViewportY  +m_iSpriteSize),f_rSpriterect);
			}
		}
// selected
		if(m_pSelected)
		{
			f_rSpriterect = core::rect<s32>(128 + 1,0,160,32);
			m_pDriver->draw2DImage(m_pTileset,core::rect<s32>((m_pSelected->getTargetPosX() * m_iSpriteSize) - m_iViewportX,(m_pSelected->getTargetPosY() * m_iSpriteSize) - m_iViewportY,(m_pSelected->getTargetPosX() * m_iSpriteSize) - m_iViewportX + m_iSpriteSize,(m_pSelected->getTargetPosY() * m_iSpriteSize) - m_iViewportY + m_iSpriteSize), f_rSpriterect,0,0,true);
// draw path
			if(m_pSelected->getPath())
			{
				f_rSpriterect = core::rect<s32>(96 + 1,0,128,32);
				for(std::list<Point>::iterator it = m_pSelected->getPath()->begin(); it != m_pSelected->getPath()->end();it++)
				{
					m_pDriver->draw2DImage(m_pTileset,core::rect<s32>(((*it).X * m_iSpriteSize) - m_iViewportX,((*it).Y * m_iSpriteSize) - m_iViewportY,((*it).X * m_iSpriteSize) - m_iViewportX + m_iSpriteSize,((*it).Y * m_iSpriteSize) - m_iViewportY + m_iSpriteSize),f_rSpriterect);
				}
			}
		}
		if(!m_lBulletImpacts->empty())
		{
			f_rSpriterect = core::rect<s32>(160 + 1,0,192,32);
			for(std::list<Impact>::iterator it = m_lBulletImpacts->begin(); it != m_lBulletImpacts->end();it++)
			{
				m_pDriver->draw2DImage(m_pTileset, core::rect<s32>((((*it)).getPosX() *m_iSpriteSize) - m_iViewportX,(((*it)).getPosY() *m_iSpriteSize) - m_iViewportY,(((*it)).getPosX() *m_iSpriteSize) - m_iViewportX +m_iSpriteSize,(((*it)).getPosY() *m_iSpriteSize) - m_iViewportY  +m_iSpriteSize),f_rSpriterect);
			}
		}

		m_pDriver->endScene();
	}
	return f_iRealTime;
}
