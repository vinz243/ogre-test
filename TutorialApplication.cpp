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
    mRoot->addFrameListener(this);
    // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
 
    // Create an Entity
    mOgreHead = mSceneMgr->createEntity("Head", "cube.mesh");
    mOgreHead->setCastShadows(true);
 
    // Create a SceneNode and attach the Entity to it
    mHeadNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
    mHeadNode->setPosition(Ogre::Vector3(0.0f, 100.0f, 0.0f));
    mHeadNode->attachObject(mOgreHead); 
    mHeadNode->scale(.5, .5, .5);
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


void InitPhysX() 
{
    //Creating foundation for PhysX
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    
    //Creating instance of PhysX SDK
    gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale() );

    if(gPhysicsSDK == NULL) 
    {
        cerr<<"Error creating PhysX3 device, Exiting..."<<endl;
        exit(1);
    }


    //Creating scene
    PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());   //Descriptor class for scenes 

    sceneDesc.gravity       = PxVec3(0.0f, -9.8f, 0.0f);        //Setting gravity
    sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);  //Creates default CPU dispatcher for the scene
    sceneDesc.filterShader  = PxDefaultSimulationFilterShader;  //Creates default collision filter shader for the scene
    
    gScene = gPhysicsSDK->createScene(sceneDesc);               //Creates a scene 
    
    
    //Creating PhysX material
    PxMaterial* material = gPhysicsSDK->createMaterial(0.5,0.5,0.5); //Creating a PhysX materia
    
    
    //---------Creating actors-----------]
    
    //1-Creating static plane    
    PxTransform planePos =  PxTransform(PxVec3(0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));   //Position and orientation(transform) for plane actor  
    PxRigidStatic* plane =  gPhysicsSDK->createRigidStatic(planePos);                               //Creating rigid static actor   
    plane->createShape(PxPlaneGeometry(), *material);                                               //Defining geometry for plane actor
    gScene->addActor(*plane);                                                                       //Adding plane actor to PhysX scene


    //2-Creating dynamic cube                                                                        
    PxTransform     boxPos(PxVec3(0.0f, 100.0f, 0.0f));                                              //Position and orientation(transform) for box actor 
    PxBoxGeometry   boxGeometry(PxVec3(2,2,2));                                         //Defining geometry for box actor
                    gBox = PxCreateDynamic(*gPhysicsSDK, boxPos, boxGeometry, *material, 1.0f);     //Creating rigid static actor
                    gScene->addActor(*gBox);                                                        //Adding box actor to PhysX scene



}

void StepPhysX()                    //Stepping PhysX
{ 
    gScene->simulate(mStepSize);    //Advances the simulation by 'gTimeStep' time
    gScene->fetchResults(true);     //Block until the simulation run is completed
} 


void ShutdownPhysX()                //Shutdown PhysX
{
    gPhysicsSDK->release();         //Removes any actors,  particle systems, and constraint shaders from this scene
    gFoundation->release();         //Destroys the instance of foundation SDK
}

void ConnectPVD()                   //Function for the visualization of PhysX simulation (Optional and 'Debug' mode only) 
{
    // check if PvdConnection manager is available on this platform
    if(gPhysicsSDK->getPvdConnectionManager() == NULL)
        return;

    // setup connection parameters
    const char*     pvd_host_ip = "127.0.0.1";  // IP of the PC which is running PVD
    int             port        = 5425;         // TCP port to connect to, where PVD is listening
    unsigned int    timeout     = 100;          // timeout in milliseconds to wait for PVD to respond,
                                                // consoles and remote PCs need a higher timeout.
    PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

    // and now try to connect
    debugger::comm::PvdConnection* theConnection = PxVisualDebuggerExt::createConnection(gPhysicsSDK->getPvdConnectionManager(),
    pvd_host_ip, port, timeout, connectionFlags);

}
void PhysXStep(Ogre::Real stepSize)
{
  
    gScene->simulate(stepSize);
    gScene->fetchResults(true);
   
}
bool PhysXUpdate(const Ogre::Real timeSinceLastUpdate)
{   
   mAccumulator += timeSinceLastUpdate; 

   while (mAccumulator >= mStepSize) 
   { 
      mAccumulator -= mStepSize; 
      PhysXStep(mStepSize);
   }

   return(true);
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {

    // System.Threading.Thread.Sleep( 1 );
    //Step PhysX simulation
    if(gScene)
        PhysXUpdate(evt.timeSinceLastFrame); 
     
    //Get current position of actor(box) and print it
    PxVec3 boxPos = gBox->getGlobalPose().p;
    mHeadNode->setPosition(Ogre::Vector3(boxPos.x, boxPos.y, boxPos.z));
    return(true);
}







//----------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

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
        InitPhysX();  //Initialize PhysX then create scene and actors

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
        //ConnectPVD(); //Uncomment this function to visualize  the simulation in PVD



        //Shut down PhysX
        ShutdownPhysX(); 
        // _getch();
        return 0;
    }

#ifdef __cplusplus
}

#endif
