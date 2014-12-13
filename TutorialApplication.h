/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include <PxPhysicsAPI.h> //Single header file to include all features of PhysX API 
#include "Entity.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif


//-------Loading PhysX libraries (32bit only)----------//

// #ifdef _DEBUG //If in 'Debug' load libraries for debug mode 
// #pragma comment(lib, "PhysX3DEBUG_x86.lib")             //Always be needed  
// #pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")       //Always be needed
// #pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")       //PhysX extended library 
// #pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib") //For PVD only 

// #else //Else load libraries for 'Release' mode
// #pragma comment(lib, "PhysX3_x86.lib")  
// #pragma comment(lib, "PhysX3Common_x86.lib") 
// #pragma comment(lib, "PhysX3Extensions.lib")
// #pragma comment(lib, "PhysXVisualDebuggerSDK.lib")
// #endif

using namespace std;
using namespace physx; 
// using namespace System;
// using namespace System::Threading;

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

	
protected:
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

void initPhysX(bool pvd);
void shutdownPhysX();
void fatalError(string error);
bool advancePhysX(Ogre::Real dt);

static PxDefaultErrorCallback 			gDefaultErrorCallback;
static PxDefaultAllocator 				  gDefaultAllocatorCallback;
static PxMaterial*						      mMaterial;
static PxPhysics* 						      mPhysics;                                                  
static PxFoundation* 					      mFoundation;                                      
static PxCooking*						        mCooking;
static PxProfileZoneManager*		  	mProfileZoneManager;
static PxScene*						          mScene;
static PxCudaContextManager*		  	mCudaContextManager;
static PxCudaContextManagerDesc 		cudaContextManagerDesc;
static PxCpuDispatcher*				     	mCpuDispatcher;
Ogre::Real 						              mAccumulator = 0;
Ogre::Real 					              	mStepSize = 1.0f/60.0f;
PxScene*                            gScene = NULL;
Ogre::Entity*                       mOgreHead= NULL;
Ogre::SceneNode*                    mHeadNode = NULL;
vector<Entity>                      mEntities;
// Entity                              mEntity;


#endif // #ifndef __TutorialApplication_h_
