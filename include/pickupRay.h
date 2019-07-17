


#ifndef __PICKUPRAY_H
#define __PICKUPRAY_H

// gl3w Headers
// #include <gl3w.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"


///Jette un rayon et recuper la forme en collision
btRigidBody* rayPickup( btDiscreteDynamicsWorld * dynamicsWorld,  glm::vec3 & rayFrom, glm::vec3  & rayTo, glm::vec3& hitPoint)
{
	btVector3 bRayFrom(rayFrom.x,rayFrom.y,rayFrom.z);
	btVector3 bRayTo(rayTo.x,rayTo.y,rayTo.z);

	btCollisionWorld::ClosestRayResultCallback rayCallback(bRayFrom,bRayTo);
	dynamicsWorld->rayTest(bRayFrom,bRayTo,rayCallback);
	if (rayCallback.hasHit())
	{
		btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (body)
		{
			hitPoint = glm::vec3(rayCallback.m_hitPointWorld.x(),rayCallback.m_hitPointWorld.y(),rayCallback.m_hitPointWorld.z());
			return body;
		}
	}

	return NULL;
}

glm::vec3 unProjectMousePos(Camera cam, glm::vec3 mouse){

	glm::vec4 viewport(0.0f, 0.0f, cam.screen_width , cam.screen_height);
	glm::vec3 worldpos;

	mouse.y = viewport[3] - mouse.y;//important (SDL )

	worldpos = glm::unProject(mouse, cam.getView(), cam.getProj(), viewport);

	// std::cout << worldpos.x << " " << worldpos.y << " " << worldpos.z << std::endl;
	return worldpos;
}




btRigidBody* getPointedBody(btDiscreteDynamicsWorld * dynamicsWorld,Camera cam, glm::vec3& hitPoint)
{
	int x = 0;
	int y = 0;

	SDL_GetMouseState(&x,&y);

	//Selection
	glm::vec3 from = cam.Position;

	glm::vec3 to = unProjectMousePos(cam,glm::vec3(x,y,1.0));

	btRigidBody* body = rayPickup(dynamicsWorld,from,to,hitPoint);

	return body;
}




#endif
