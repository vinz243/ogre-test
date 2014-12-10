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


//-------Loading PhysX libraries (32bit only)----------//

#ifdef _DEBUG //If in 'Debug' load libraries for debug mode 
#pragma comment(lib, "PhysX3DEBUG_x86.lib")             //Always be needed  
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")       //Always be needed
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")       //PhysX extended library 
#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib") //For PVD only 

#else //Else load libraries for 'Release' mode
#pragma comment(lib, "PhysX3_x86.lib")  
#pragma comment(lib, "PhysX3Common_x86.lib") 
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDK.lib")
#endif

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

void InitPhysX();		//Initialize the PhysX SDK and create two actors. 
void StepPhysX();		//Step PhysX simulation 300 times.
void ShutdownPhysX();	//Shutdown PhysX SDK
void PhysXStep(Ogre::Real stepSize);
bool PhysXUpdate(const Ogre::Real timeSinceLastUpdate);
void ConnectPVD();		//Function for the visualization of PhysX simulation (Optional and 'Debug' mode only) 

//--------------Global variables--------------//
static PxPhysics*               gPhysicsSDK = NULL;         //Instance of PhysX SDK
static PxFoundation*            gFoundation = NULL;         //Instance of singleton foundation SDK class
static PxDefaultErrorCallback   gDefaultErrorCallback;      //Instance of default implementation of the error callback
static PxDefaultAllocator       gDefaultAllocatorCallback;  //Instance of default implementation of the allocator interface required by the SD

Ogre::Real 						mAccumulator = 0;
Ogre::Real 						mStepSize = 1.0f/60.0f;
PxScene*                        gScene = NULL;              //Instance of PhysX Scene               
PxRigidDynamic                  *gBox = NULL;               //Instance of box actor 
Ogre::Entity*                   mOgreHead= NULL;
Ogre::SceneNode*                mHeadNode = NULL;
//-------------------------------------------------------------------------------------
#endif // #ifndef __TutorialApplication_h_
