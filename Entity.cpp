#include "Entity.h"

Entity::Entity(void) {

}

Entity::Entity(Ogre::Vector3 dims, Ogre::Vector3 pos, std::string mesh, std::string id) 
{

	mDimensions = dims;
	mPosition 	= pos;
	mMesh		= mesh;
	mId 		= id;
	mActor		= NULL;
    mMaterial   = NULL;
}

Entity::~Entity(void)
{

}

void Entity::update(Ogre::Real dt)
{
	// if (mActor) {
		PxVec3 pos = mActor->getGlobalPose().p;
		PxQuat quat = mActor->getGlobalPose().q;
		Ogre::Real r = 0;
		mOgreNode->setPosition(Ogre::Vector3(pos.x + r, pos.y + r, pos.z + r));
		mOgreNode->setOrientation(quat.w, quat.x, quat.y, quat.z);
	// }
}

void Entity::init(Ogre::SceneManager* sceneMgr, PxPhysics* physics, PxScene* scene, PxVec3 velocity)
{
	// Create an Entity
    mOgreEntity = sceneMgr->createEntity(mId, mMesh);
    mOgreEntity->setCastShadows(true);
 
    // Create a SceneNode and attach the Entity to it
    mOgreNode = sceneMgr->getRootSceneNode()->createChildSceneNode(mId + "Node");
	Ogre::AxisAlignedBox box = mOgreEntity->getBoundingBox();
	Ogre::Vector3 realSizes = box.getSize();
   
    mOgreNode->setPosition(mPosition);
    mOgreNode->attachObject(mOgreEntity);

    Ogre::Vector3 scaler = Ogre::Vector3(mDimensions.x / realSizes.x, mDimensions.y / realSizes.y, mDimensions.z / realSizes.z);
    mOgreNode->scale(scaler);


	mMaterial = physics->createMaterial(1.0f, 1.0f, 1.5f);
    PxGeometry geometry = PxSphereGeometry();
	if(mMesh == "sphere.mesh")
	{
  	  	geometry = PxSphereGeometry(mDimensions.x / 2); // Because it's a radius
	} else {
		geometry = PxBoxGeometry(mDimensions.x / 2, mDimensions.y / 2, mDimensions.z / 2);
	}

    // PxTransform transform = PxTransform();
	// mActor = PxCreateDynamic(*physics, transform, geometry, *mMaterial, PxReal(.1));
	mActor = physics->createRigidDynamic(PxTransform(PxVec3(mPosition.x, mPosition.y, mPosition.z)));
	PxShape* shape = mActor->createShape(PxSphereGeometry(mDimensions.x / 2), *mMaterial);
	PxRigidBodyExt::updateMassAndInertia(*mActor, 2);
	// if(!mActor) {
 //        MessageBox( NULL, "no actor", "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	// 	return;
	// }
	mActor->setLinearVelocity(velocity);
	// And add the actor to a scene:

	scene->addActor(*mActor);

}

