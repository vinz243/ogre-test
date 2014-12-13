#ifndef __Entity_h_
#define __Entity_h_

#include <PxPhysicsAPI.h> //Single header file to include all features of PhysX API 

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

using namespace physx; 

class Entity
{

public:
	Entity(void);
	Entity(Ogre::Vector3 dims, Ogre::Vector3 pos, std::string mesh, std::string id);
	virtual ~Entity(void);
	virtual void update(Ogre::Real dt);
	virtual void init(Ogre::SceneManager* sceneMgr, PxPhysics* physics, PxScene* scene, PxVec3 velocity=PxVec3(0, 0, 0));
protected:
	Ogre::Entity* 		mOgreEntity = NULL;
	Ogre::SceneNode* 	mOgreNode = NULL;
	
	Ogre::Vector3 		mPosition;
	Ogre::Vector3 		mDimensions;
	std::string			mMesh;
	std::string			mId;

	
	PxRigidDynamic*		mActor;
    PxMaterial* 		mMaterial;
};
#endif // #ifndef __Entity_h_