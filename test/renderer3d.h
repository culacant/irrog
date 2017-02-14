#ifndef RENDERER_H
#define RENDERER_H

#include <irrlicht/irrlicht.h>

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
	Mapd(int width, int height, scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
	~Mapd();

	virtual void OnRegisterSceneNode();
	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);
};

#endif
