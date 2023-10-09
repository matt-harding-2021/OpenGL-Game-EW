/** \file objectCollider.h */

#pragma once
//#include "systems/log.h"
#include "reactphysics3d.h"

class ObjectCollider : public rp3d::EventListener
{
public:
	ObjectCollider();
	~ObjectCollider();
	void newContact(const rp3d::CollisionCallback::CollisionCallbackInfo& collisionInfo);
private:

};
