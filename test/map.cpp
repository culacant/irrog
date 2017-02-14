#include "renderer3d.h"

int main()
{
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL,core::dimension2d<u32>(640, 480), 16, false);
        
    if (device == 0)
        return 1; // could not create selected driver.

    // create engine and camera

    device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

    smgr->addCameraSceneNodeFPS();
	Mapd *mapd = new Mapd(40,10,smgr->getRootSceneNode(), smgr, 666);

	scene::ISceneNode *node;
	node = smgr->addSphereSceneNode();
	if(node)
	{
		node->setPosition(core::vector3df(10,10,0));
		node->setScale(core::vector3df(0.1,0.1,0.1));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}

	mapd->drop();
	u32 frames=0;
    while(device->run())
    {
        driver->beginScene(true, true, video::SColor(0,100,100,100));

        smgr->drawAll();

        driver->endScene();
        if (++frames==100)
        {
            core::stringw str = L"Irrlicht Engine [";
            str += driver->getName();
            str += L"] FPS: ";
            str += (s32)driver->getFPS();

            device->setWindowCaption(str.c_str());
            frames=0;
        }
    }

    device->drop();
	return 0;
}
