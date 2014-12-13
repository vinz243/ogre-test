/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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
#include "TutorialApplication.h"


TutorialApplication::TutorialApplication(void)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{   
    float scale = 1.5f;

    for(int i = 0 ; i < 25 ; i++ ){
        ostringstream nameStream;
        nameStream << "Sphere_" << i;
        string name = nameStream.str();
        Entity sphere = Entity(Ogre::Vector3((i + 1) * scale, (i + 1) * scale, (i + 1) * scale), Ogre::Vector3((i + 1 + rand() % 100) * 5, (i + 1 + rand() % 100) * 4.5, (i + 1 + rand() % 100) * 6), "sphere.mesh", name);
        sphere.init(mSceneMgr, mPhysics, gScene, PxVec3(rand() % 50 - 25, rand() % 50 - 25,  rand() % 50 - 25));
        mEntities.push_back(sphere);

    }
    for(int i = 0 ; i < 25 ; i++ ){
        ostringstream nameStream;
        nameStream << "Cube_" << i;
        string name = nameStream.str();
        Entity cube = Entity(Ogre::Vector3((i + 1) * scale, (i + 1) * scale, (i + 1) * scale), Ogre::Vector3((i + 1 + rand() % 100) * 5, (i + 1 + rand() % 100) * 4.5, (i + 1 + rand() % 100) * 6), "cube.mesh", name);
        cube.init(mSceneMgr, mPhysics, gScene, PxVec3(rand() % 20 - 10, rand() % 20 - 10,  rand() % 20 - 10));
        mEntities.push_back(cube);

    }

    mRoot->addFrameListener(this);
    // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
 
    
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
 
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
 
    Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
 
    entGround->setMaterialName("Examples/Rockwall");
    entGround->setCastShadows(false);
 
    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20.0f, 80.0f, 50.0f);
}


bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }

    if(gScene)    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }

    advancePhysX(evt.timeSinceLastFrame);

    gScene->fetchResults(true);
    
    for(int i = 0 ; i < mEntities.size(); i++) {
        mEntities[i].update(evt.timeSinceLastFrame);

    }
    return(true);
}

void initPhysX(bool pvd) {

    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (!mFoundation)
        fatalError("PxCreateFoundation failed!");
    
    // if (pvd) {
    //     mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
    //     if(!mProfileZoneManager)
    //         fatalError("PxProfileZoneManager::createProfileZoneManager failed!");
    // }

    // Creates the Top-level PhysX obbject
    bool recordMemoryAllocations = true;
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager );
    if(!mPhysics)
        fatalError("PxCreatePhysics failed!");

    // mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams());
    // if (!mCooking)
    //     fatalError("PxCreateCooking failed!");

    if (!PxInitExtensions(*mPhysics))
        fatalError("PxInitExtensions failed!");


    
    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    // customizeSceneDesc(sceneDesc);
    
    if(!sceneDesc.cpuDispatcher)
    {
        mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
        if(!mCpuDispatcher)
            fatalError("PxDefaultCpuDispatcherCreate failed!");
        sceneDesc.cpuDispatcher    = mCpuDispatcher;
    }
    static PxSimulationFilterShader gDefaultFilterShader=PxDefaultSimulationFilterShader;


    if(!sceneDesc.filterShader)
        sceneDesc.filterShader  = gDefaultFilterShader;
        // fatalError("shader");
        // sceneDesc.filterShader    =  *gDefaultFilterShader;
    
    #ifdef PX_WINDOWS
    if(!sceneDesc.gpuDispatcher && mCudaContextManager)
    {
        mCudaContextManager = PxCreateCudaContextManager(*mFoundation, cudaContextManagerDesc, mProfileZoneManager);
        if( mCudaContextManager ){
            if( !mCudaContextManager->contextIsValid() ){
                mCudaContextManager->release();
                mCudaContextManager = NULL;
                fatalError("cuda");
                return;
            }
    
            if(!sceneDesc.gpuDispatcher){
                sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
            }
        } else {
            fatalError("cuda");
            return;
        }
    }
    #endif
    
    gScene = mPhysics->createScene(sceneDesc);
    if (!gScene)
        fatalError("createScene failed!");

    PxMaterial* mMaterial;

    mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution
    if(!mMaterial)
        fatalError("createMaterial failed!");
    // And add the actor to a scene:


    PxRigidStatic* plane = PxCreatePlane(*mPhysics, PxPlane(PxVec3(0,1,0), 0), *mMaterial);
    if (!plane)
        fatalError("create shape failed!");
    gScene->addActor(*plane);

}
bool advancePhysX(Ogre::Real dt)
{
    mAccumulator  += dt;
    if(mAccumulator < mStepSize)
        return false;

    mAccumulator -= mStepSize;

    gScene->simulate(mStepSize);

    return true;
}
void shutdownPhysX() {
    
    mPhysics->release();
    // Do not forget to release the foundation object as well, but only after all other PhysX modules have been released:

    mFoundation->release();
}

void fatalError(string error)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, error.c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    std::cerr << "An exception has occured: " <<
                error << std::endl;
#endif
}


//----------------------------------------------


#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;
        initPhysX(false);
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }


        shutdownPhysX();
        //Shut down PhysX
        // _getch();
        return 0;
    }

#ifdef __cplusplus
}

#endif
