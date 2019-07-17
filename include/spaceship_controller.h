#ifndef SPACESHIP_CONTROLLER_H
#define SPACESHIP_CONTROLLER_H

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/norm.hpp>
#include <math.h>

class spaceship_controller
{
public:
    // glm::vec3 position;
    btRigidBody* body;
    float speed = 60.0f;
    float maneuverability = 2.5f;

    bool moving =false;
    glm::vec3 direction;
    // glm::vec3 rightVec;
    // glm::vec3 up;
    glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f);
    // glm::vec3 worldRight = glm::vec3(1.0f,0.0f,0.0f);

    glm::vec3 force;
    glm::vec3 basis;

    float angleRot =0.0f;

    // bool goForward = false;
    // glm::vec3 forwardForce;

    // bool goBackward = false;
    // glm::vec3 backwardForce;

    bool rotate = false;
    glm::vec2 torque;
    spaceship_controller( )
    {
        body = NULL;
        // up = worldUp;



    }
    spaceship_controller( btRigidBody* rb)
    {
        body = rb;
        direction = getDirection();
        // up = worldUp;
    }

    glm::vec3 getPos()
    {
        // btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
        btTransform trans;
        // myMotionState->getWorldTransform(trans);
        // body->getCenterOfMassPosition();
        btVector3 pos =body->getCenterOfMassPosition();
        // btVector3 pos = trans.getOrigin();

        return glm::vec3(pos.x(),pos.y(), pos.z());
    }


    glm::vec3 getDirection(){

        btVector3 dir = body->getWorldTransform().getBasis()* btVector3(0.0,0.0,-1.0);
        return  glm::vec3 (dir.x(),dir.y(),dir.z()) ;
        // btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
        // btTransform trans;
        // myMotionState->getWorldTransform(trans);

        // btVector3 direction = btVector3(0.0,0.0,-1.0);

        // direction = trans.getBasis()*direction;

        // return glm::vec3(direction.x(),direction.y(), direction.z());
    }

    glm::vec3 getUp(){
        btVector3 up = body->getWorldTransform().getBasis()* btVector3(worldUp.x,worldUp.y,worldUp.z);
        return  glm::vec3 (up.x(),up.y(),up.z()) ;//need to convert coordinate
    }

    glm::mat4 getOpenGLMatrix(){
        btScalar m[16];
        // glm::mat4 phymodel =  glm::mat4(1);
        // rigidBody[i]->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));
        body->getWorldTransform().getOpenGLMatrix(m );
        glm::mat4 phymodel = glm::make_mat4(m);

        // glm::mat4 phymodel =  glm::mat4(1);
        // body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));
        
        return phymodel;

    }

    void addForce(glm::vec3 f){
        force += f;
        moving = true;
    }


    void setTorque(glm::vec2 t){
        // float phy = glm::radians(t.y);
        // float theta = glm::radians(t.x);

        // glm::vec3 newdirection = glm::vec3( cos(theta)*sin(phy), sin(theta), cos(theta)*cos(phy) );

        // torque = glm::normalize(direction- newdirection) * maneuverability;
        torque = t * maneuverability;
        rotate =true;
        moving = true;
    }
    void applyForces(){

        //On applique les forces contraires
        if(body)
        {
            body->setActivationState(ACTIVE_TAG);
            if (force != glm::vec3 (0.0f,0.0f,0.0f)){
                body->applyCentralForce(btVector3(force.x,force.y,force.z));
                force = glm::vec3 (0.0f,0.0f,0.0f);
            }
            // if (goForward){
            //     body->applyCentralForce(btVector3(forwardForce.x,forwardForce.y,forwardForce.z));
            //     goForward = false;
            // }

            // if (goBackward){
            //     body->applyCentralForce(btVector3(backwardForce.x,backwardForce.y,backwardForce.z));
            //     goBackward = false;
            // }

            if (rotate){
                // body->applyTorque(body->getWorldTransform().getBasis() * btVector3(torque.y,torque.x,0.0f));//torque.y
                // body->applyTorque(btVector3(0.0f,1.0f,0.0f )* torque.x);
                // updateVecs();
                body->applyTorque(body->getWorldTransform().getBasis() * btVector3(1.0f,0.0f,0.0f )* torque.y);//btVector3(rightVec.x,rightVec.y,rightVec.z )* torque.y);
                body->applyTorque(body->getWorldTransform().getBasis() * btVector3(0.0f,1.0f,0.0f )* torque.x);//btVector3(rightVec.x,rightVec.y,rightVec.z )* torque.y);
                body->applyTorque(body->getWorldTransform().getBasis() * btVector3(0.0f,0.0f,1.0f )* (angleRot* maneuverability));//btVector3(rightVec.x,rightVec.y,rightVec.z )* torque.y);
                angleRot = 0.0f;
                // updateVecs();
                rotate = false;
                torque = glm::vec2 (0.0f,0.0f);
            }

            //damping both angular and linear velocity
            btVector3 av = body->getAngularVelocity();
            glm::vec3 angularVelocity = glm::vec3(av.x(),av.y(),av.z());
            glm::vec3 dampingAng = angularVelocity;
            dampingAng = dampingAng * 0.30f;
            if (glm::length(dampingAng) <3.0f && glm::length(dampingAng)>0.0f){
                dampingAng = glm::normalize(dampingAng) * 3.0f;
            }

            if (glm::length2(angularVelocity) < 0.5f){
                dampingAng = angularVelocity;
            }
            dampingAng *= -1.0f;

            
            body->applyTorque(btVector3(dampingAng.x,dampingAng.y,dampingAng.z));
            // updateVecs();

            btVector3 lv = body->getLinearVelocity();
            glm::vec3 linearVelocity = glm::vec3(lv.x(),lv.y(),lv.z());
            glm::vec3 dampingLin = linearVelocity;
            dampingLin = dampingLin * 1.5f;
            dampingLin *= -1.0f;

            body->applyCentralForce(btVector3(dampingLin.x,dampingLin.y,dampingLin.z));

            moving = false;
            if(glm::length2(linearVelocity) > 0.0f || glm::length2(angularVelocity) > 0.0f)
            {
                moving = true;
            }
            //update vector
            direction = getDirection();
            
        }
    }


    // void updateVecs()
    // {

    //     // body->getWorldTransform().getBasis() * btVector3(1.0f,0.0f,0.0f )
    //     direction = getDirection();

    //     rightVec = glm::cross(direction, up);
    //     rightVec = glm::normalize(rightVec);

    //     up = glm::cross(rightVec, direction);
    //     up = glm::normalize(up);
    // }


    glm::vec3 getCamPointToLookAT(){
        return getPos() - getUp() * 2.5f;// glm::vec3(0.0f,2.5f,0.0f);
    }



    glm::vec3 getCamPos(){
     return getPos()+ ( getDirection() * -10.0f ) - getUp()* 3.5f; //glm::vec3(0.0f,3.5f,0.0f) ;
    }


};


#endif