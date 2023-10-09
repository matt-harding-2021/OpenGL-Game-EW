/**\ file objectCollider.cpp */

#include "engine_pch.h"
#include "physics/objectCollider.h"

#include "systems/logging.h"

ObjectCollider::ObjectCollider()
{
}

ObjectCollider::~ObjectCollider()
{
}

void ObjectCollider::newContact(const rp3d::CollisionCallback::CollisionCallbackInfo& collisionInfo)
{
	int body1Index = (int)collisionInfo.body1->getUserData();
	int body2Index = (int)collisionInfo.body2->getUserData();

	LOG_INFO("Bodies {0} and {1} have collided", body1Index, body2Index);
}