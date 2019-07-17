#ifndef CAMERA_TRANS_H
#define CAMERA_TRANS_H

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Transform.h"

class CameraTrans
{

public:

    bool fps =false;//camera mode

    Transform t ;// the position of the cam
    Transform focus; // the focus point that the cam look at

    float screen_width ;
    float screen_height;
    float FOV_angle;

    float MouseSensitivity;
    // Euler Angles
    float Yaw;
    float Pitch;

	CameraTrans(){
		t = Transform();
		// t.m = glm::translate(glm::mat4(),glm::vec3(0.0,1.0,-1.0));
		FOV_angle = 60.0f;
		Yaw = 0;
        Pitch = 0;
        MouseSensitivity = 0.2f;
	}

	void setWindowSize(int w,int h)
    {
        screen_width = float(w);
        screen_height = float(h);
    }

	glm::mat4 getView()
    {
        return glm::inverse(t.getMat());
    }
    
    glm::mat4 getProj()
    {    
        // projection matrix, to decide camera parameters (fov, screen ratio and cliping distance)
        return glm::perspective(glm::radians(FOV_angle), screen_width / screen_height, 1.0f, 500.0f);
    }

    glm::vec3 getPos()
    {
        return t.getPos();
    }

    void setPos(glm::vec3 newPos)
    {
        t.setPos(newPos);
    }
    void translate(glm::vec3 v)
    {
    	t.translate(v);
    }
    
    void rotate(float angle,glm::vec3 axis)
	{
    	t.rotate(angle, axis);
    }


    void setToFps(){

        glm::mat4 worldm = t.getMat();//give the world position
        glm::mat4 localm = t.m;//give the relative position to focus

        t.parent = NULL;
        t.m = worldm;//keep the cam at the same place

        focus.parent = &t;
        focus.m = glm::inverse(localm);
    }

    void setToTps(){

        glm::mat4 worldm = focus.getMat();//give the world position
        glm::mat4 localm = focus.m;//give the relative position to focus

        focus.parent = NULL;
        focus.m = worldm;//keep the cam at the same place

        t.parent = &focus;
        t.m = glm::inverse(localm);
    }

    void TPSRotation(float xoffset, float yoffset,  GLboolean constrainPitch = true)
    {

        Yaw += xoffset * MouseSensitivity;
        Pitch += yoffset * MouseSensitivity;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;


        focus.setRotMat(glm::rotate(glm::mat4(),glm::radians(Yaw),glm::vec3(0.0,1.0,0.0))* glm::rotate(glm::mat4(),glm::radians(Pitch),glm::vec3(1.0,0.0,0.0)));

        // t.setRotMat(  glm::rotate(glm::mat4(),glm::radians(Yaw),glm::vec3(0.0f,1.0f,0.0f)) );// * t4.getRotMat());

        // return t;
    }


    void FPSRotation(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {

        Yaw += xoffset * MouseSensitivity;
        Pitch += yoffset * MouseSensitivity;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        //update the transform

        t.setRotMat(glm::rotate(glm::mat4(),glm::radians(Yaw),glm::vec3(0.0,1.0,0.0))* glm::rotate(glm::mat4(),glm::radians(Pitch),glm::vec3(1.0,0.0,0.0)));

    }
};
#endif