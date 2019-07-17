#ifndef CAMERA_OLD_H
#define CAMERA_OLD_H

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
// #include "Molecule.h"

class Camera
{
public:
    glm::vec3 pos; //current position of the camera
    glm::vec3 up; //up vector of the camera
    glm::vec3 worldUp; //up vector reference for the world
    glm::vec3 center; //the point the camera currently look at
    glm::mat4 view; // the view matrix
    glm::mat4 proj; // the projection/perspective matrix

    float limit;// the closest distance from the point looked at wich the camera can go  
    glm::vec3 direction; //direction of the camera (vector from pos to center)
    glm::vec3 normVec; //give the right axis of the camera
    

    float screen_width ;
    float screen_height;
    float FOV_angle;
    
    Camera()
    {
        pos = glm::vec3(0.0f, 0.0f, 20.0f);
        center = glm::vec3(0.0f, 0.0f, 0.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        direction = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
        limit = 1.0f;
        FOV_angle = 60.0f;
        updateVecs();
    }

    glm::mat4 lookAtMol(Molecule mol)
    {
        center = mol.barycenter;
        updateVecs();
        return getView();
    }



    glm::mat4 lookAtPoint(glm::vec3 point)
    {
        center = point ;
        updateVecs();
        return getView();

    }

    glm::mat4 getView()
    {
        view = glm::lookAt(pos, center, up);
        return view;
    }
    
    glm::mat4 getProj()
    {    
        // projection matrix, to decide camera parameters (fov, screen ratio and cliping distance)
        proj = glm::perspective(glm::radians(FOV_angle), screen_width / screen_height, 1.0f, 500.0f);
        return proj;
    }

    // move the camera along the direction vector (toward the point the camera is looking at). limit the closest distance possible so the camera can't go trough
    void traveling(float dist){
        if (distance(center, pos+(direction*dist))>limit)
        {
            pos += direction * dist;
        }
        updateVecs();

    }
    /**
     * On recalcule les vecteurs utiles au déplacement de la camera (_Direction, _NormVec, _UpVec)
     * on part du principe que sont connus :
     * - la position de la camera
     * - le point regarde par la camera
     * - le vecteur up de notre monde
     */
    void updateVecs(void)
    {
        direction = center - pos;
        direction = glm::normalize(direction);

        normVec = glm::cross(direction, worldUp);
        normVec = glm::normalize(normVec);

        up = glm::cross(normVec, direction);
        up = glm::normalize(up);
    }

    /**
     * Rotation droite gauche en troisième personne
     */
    void rotateAround(float angle)
    {
        pos -= center;
        // pos = glm::rotate(pos, angle, worldUp);
        pos = glm::rotate(pos, angle, up);//seems better !
        pos += center;
        updateVecs();
    }

    /**
     * Rotation haut bas en troisième personne
     */
    void rotateUpAround(float angle)
    {
        pos -= center;

        //On ne monte pas trop haut pour ne pas passer de l'autre coté
        glm::vec3 previousPos = pos;
        pos = glm::rotate(pos, angle, normVec);

        glm::vec3 normPos = pos;
        normPos = glm::normalize(normPos);

        float newAngle = glm::dot(normPos, worldUp);
        if (newAngle > 0.99 || newAngle < -0.99)
            pos = previousPos;

        pos += center;
        updateVecs();
    }

    void arcBallRotation(bool button_pressed,int x, int y)
    {

        static int lastx = -1; //remember, the static keyword indicate a persistent variable (live during the whole program)
        static int lasty = -1;
        if (button_pressed)
        {
            if (lastx == -1 && lasty == -1)
            {
                lastx = x;
                lasty = y;
            }

            int dx = x - lastx;
            int dy = y - lasty;

            rotateAround((float) -dx / 300.0f);
            rotateUpAround((float) -dy / 300.0f);

        }
        lastx = x;
        lasty = y;
    }

    void setWindowSize(int w,int h)
    {
        screen_width = float(w);
        screen_height = float(h);
    }

    glm::mat4 getRotMat(){
        glm::vec3 A,B;

        A = direction;
        B = glm::vec3(0.0,1.0,0.0);

        glm::vec3 axis = glm::cross(B,A);
        float angle = acos(glm::dot(B,A)/(glm::length(B)*glm::length(A)));
        return glm::rotate(angle,axis);
    }

};
#endif