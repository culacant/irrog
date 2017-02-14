#include "renderer3d.h"

#include <unistd.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// 3dMap Class

Mapd::Mapd(int width, int height, scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) : scene::ISceneNode(parent,mgr,id)
{
	m_iWidth = width;
	m_iHeight = height;
	m_vVertices = new video::S3DVertex[m_iWidth*m_iHeight];
	m_mMaterial.Wireframe = false;
	m_mMaterial.Lighting = false;
	for(int y = 0;y < m_iHeight;y++)
	{
		for(int x = 0;x < m_iWidth;x++)
		{
			m_vVertices[x + y * m_iWidth] = video::S3DVertex(x*10,0,y*10,0,0,0,video::SColor(255,0,255,255), 0, 0);
		}
	}
	m_aBoundingBox.reset(m_vVertices[0].Pos);
	m_iWidth = width;
	m_iHeight = height;
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
	std::cout << "1: " << std::endl;
	for(int y = 0;y < m_iHeight;y++)
	{
		for(int x = 0;x < m_iWidth - 1;x++)
		{
				indices[i+2] = x+y*m_iWidth;
				indices[i+1] = x+1+y*m_iWidth;
				indices[i+0] = x+(y+1)*m_iWidth;


				std::cout << indices[i+0] << "," << indices[i+1] << "," << indices[i+2] << std::endl;
				i+=3;
		}
	}
	i = 0;
	u16 indices2[m_iWidth*m_iHeight*3];
	std::cout << "2: " << std::endl;
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
	driver->drawVertexPrimitiveList(&m_vVertices[0], m_iWidth*m_iHeight, &indices2[0], (m_iWidth-1)*(m_iHeight-1), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
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
