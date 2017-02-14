#include "renderer3d.h"

#include <unistd.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// 3dMap Class
Mapd::Mapd(Map *map, scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) : scene::ISceneNode(parent,mgr,id)
{
    m_iWidth = map->Width();
    m_iHeight = map->Height();
    m_vVertices = new video::S3DVertex[m_iWidth*m_iHeight];
    m_mMaterial.Wireframe = false;
    m_mMaterial.Lighting = false;
    m_aBoundingBox.reset(m_vVertices[0].Pos);

	video::SColor f_sColor;
    for(int y = 0;y < m_iHeight;y++)
    {
        for(int x = 0;x < m_iWidth;x++)
        {
            m_vVertices[x + y * m_iWidth] = video::S3DVertex(x*10,0,y*10,0,0,0,video::SColor(255,0,255,255), 0, 0);
			m_aBoundingBox.addInternalPoint(m_vVertices[x+y*m_iWidth].Pos);

        }
    }
}

Mapd::~Mapd()
{
    delete[] m_vVertices;
}

void Mapd::OnRegisterSceneNode()
{
    if(IsVisible)
        SceneManager->registerNodeForRendering(this);
    ISceneNode::OnRegisterSceneNode();
}
void Mapd::render()
{
    int i;
    i = 0;
    u16 indices[m_iWidth*m_iHeight*3];
    for(int y = 0;y < m_iHeight;y++)
    {
        for(int x = 0;x < m_iWidth - 1;x++)
        {
                indices[i+2] = x+y*m_iWidth;
                indices[i+1] = x+1+y*m_iWidth;
                indices[i+0] = x+(y+1)*m_iWidth;


                i+=3;
        }
    }
    i = 0;
    u16 indices2[m_iWidth*m_iHeight*3];
    for(int y = 0;y < m_iHeight;y++)
    {
        for(int x = 0;x < m_iWidth - 1;x++)
        {
                indices2[i+2] = x+(y+1)*m_iWidth;
                indices2[i+1] = x+1+y*m_iWidth;
                indices2[i+0] = x+1+(y+1)*m_iWidth;
                i+=3;
        }
    }

    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setMaterial(m_mMaterial);
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&m_vVertices[0], m_iWidth*m_iHeight,&indices[0], (m_iWidth-1)*(m_iHeight-1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
    driver->drawVertexPrimitiveList(&m_vVertices[0], m_iWidth*m_iHeight, &indices2[0], (m_iWidth-1)*(m_iHeight-1), video::EVT_STANDARD,scene::EPT_TRIANGLES, video::EIT_16BIT);
}
const core::aabbox3d<f32>& Mapd::getBoundingBox() const
{
    return m_aBoundingBox;
}
u32 Mapd::getMaterialCount() const
{
    return 1;
}
video::SMaterial& Mapd::getMaterial(u32 i)
{
    return m_mMaterial;
}



// Renderer Class
Renderer::Renderer(Map* map, std::list<Actor*> *actorlist, Actor* selected,std::list<Impact> *impactlist, IEventReceiver *inputhandler)
{
	m_pMap = map;

	m_lActorList = actorlist;
	m_pSelected = selected;
	m_lBulletImpacts = impactlist;

	m_pDevice = createDevice(video::EDT_OPENGL,core::dimension2d<u32>(WINDOW_WIDTH, WINDOW_HEIGHT),16,false,false,false, inputhandler);
	m_pDriver = m_pDevice->getVideoDriver();

	m_pDevice->run();

	m_iViewportX = 0;
	m_iViewportY = 0;
	m_iSpriteSize = 32;

	m_iTime = 0;

	m_pSceneManager = m_pDevice->getSceneManager();
	m_pSceneManager->addCameraSceneNodeFPS();

	m_pMapd = new Mapd(m_pMap,m_pSceneManager->getRootSceneNode(),m_pSceneManager,666);

	m_lModelList = new std::list<scene::ISceneNode*>();
	for(std::list<Actor*>::iterator it = m_lActorList->begin(); it != m_lActorList->end();++it)
	{
		scene::ISceneNode *Model = m_pSceneManager->addSphereSceneNode();
		if(Model)
		{
			Model->setMaterialFlag(video::EMF_LIGHTING, false);
		}
		m_lModelList->push_back(Model);
	}
}

Renderer::~Renderer()
{
	m_pMapd->drop();
	m_pDevice->drop();
}

// not used for 3d
void Renderer::moveViewport(int dx, int dy)
{
	return;
}
void Renderer::setZoomDelta(int zoomdelta)
{
	return;
}
int Renderer::ViewportX()
{
	return 1;
}
int Renderer::ViewportY()
{
	return 1;
}
int Renderer::ZoomLevel()
{
	return 1;
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
// drawn with the mapd
// actors
		std::list<scene::ISceneNode*>::iterator mit = m_lModelList->begin();
		for(std::list<Actor*>::iterator it = m_lActorList->begin(); it != m_lActorList->end(); ++it)
		{
			if((*it)->Visible())
			{
				(*mit)->setVisible(true);
				(*mit)->setPosition(core::vector3df((*it)->getPosX()*10,0,(*it)->getPosY()*10));
			/*
			switch(((*it))->Graphic())
			{
				case '@':
					f_rSpriterect = core::rect<s32>(64 + 1,0,96,32);
					break;
				case 'x':
					f_rSpriterect = core::rect<s32>(192 + 1 + 1,0,224,32);
					break;
			}
//			m_pDriver->draw2DImage(m_pTileset, core::rect<s32>((((*it))->getPosX() *m_iSpriteSize) - m_iViewportX,(((*it))->getPosY() *m_iSpriteSize) - m_iViewportY,(((*it))->getPosX() *m_iSpriteSize) - m_iViewportX +m_iSpriteSize,(((*it))->getPosY() *m_iSpriteSize) - m_iViewportY  +m_iSpriteSize),f_rSpriterect);
			*/
			}
			else
				(*mit)->setVisible(false);
			++mit;
		}
// selected
		if(m_pSelected)
		{
			f_rSpriterect = core::rect<s32>(128 + 1,0,160,32);
//			m_pDriver->draw2DImage(m_pTileset,core::rect<s32>((m_pSelected->getTargetPosX() * m_iSpriteSize) - m_iViewportX,(m_pSelected->getTargetPosY() * m_iSpriteSize) - m_iViewportY,(m_pSelected->getTargetPosX() * m_iSpriteSize) - m_iViewportX + m_iSpriteSize,(m_pSelected->getTargetPosY() * m_iSpriteSize) - m_iViewportY + m_iSpriteSize), f_rSpriterect,0,0,true);
// draw path
			if(m_pSelected->getPath())
			{
				f_rSpriterect = core::rect<s32>(96 + 1,0,128,32);
				for(std::list<Point>::iterator it = m_pSelected->getPath()->begin(); it != m_pSelected->getPath()->end();it++)
				{
//					m_pDriver->draw2DImage(m_pTileset,core::rect<s32>(((*it).X * m_iSpriteSize) - m_iViewportX,((*it).Y * m_iSpriteSize) - m_iViewportY,((*it).X * m_iSpriteSize) - m_iViewportX + m_iSpriteSize,((*it).Y * m_iSpriteSize) - m_iViewportY + m_iSpriteSize),f_rSpriterect);
				}
			}
		}
		if(!m_lBulletImpacts->empty())
		{
			f_rSpriterect = core::rect<s32>(160 + 1,0,192,32);
			for(std::list<Impact>::iterator it = m_lBulletImpacts->begin(); it != m_lBulletImpacts->end();it++)
			{
//				m_pDriver->draw2DImage(m_pTileset, core::rect<s32>((((*it)).getPosX() *m_iSpriteSize) - m_iViewportX,(((*it)).getPosY() *m_iSpriteSize) - m_iViewportY,(((*it)).getPosX() *m_iSpriteSize) - m_iViewportX +m_iSpriteSize,(((*it)).getPosY() *m_iSpriteSize) - m_iViewportY  +m_iSpriteSize),f_rSpriterect);
			}
		}
// GUI here
		m_pSceneManager->drawAll();
		
		m_pDriver->endScene();
	}
	return f_iRealTime;
}
