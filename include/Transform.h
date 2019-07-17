#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/gtx/matrix_interpolation.hpp>



class Transform
{
public:
    glm::mat4 m; //transformation matrice 
    Transform* parent; // parent Transform
        
    Transform()
    {
        m = glm::mat4();
        parent = NULL;
    }

    Transform(Transform* p)
    {
        m = glm::mat4();
        parent = p;
    }

    Transform(glm::mat4 mat )
    {
        m = mat;
        parent = NULL;
    }

    Transform(glm::mat4 mat ,Transform* p)
    {
        m = mat;
        parent = p;
    }

    glm::mat4 getMat(){
        if (parent != NULL){
            return parent->getMat() * m;
        }
        return m;
    }

    glm::vec3 getPos(){
        return glm::vec3(getMat() * glm::vec4(0.0f,0.0f,0.0f,1.0f));
    }

    void setPos(glm::vec3 newPos)
    {
        translate(newPos - getPos());
    }

    glm::mat4 getRotMat(){
        return glm::extractMatrixRotation(getMat());    
    }

    glm::vec3 getDirection(){
        return glm::vec3(getRotMat() * glm::vec4(0.0f,0.0f,-1.0f,1.0f) );
    }

    //be sure to pass a matrice with no scale (best to only pass getRotMat result)
    void setRotMat(glm::mat4 rot){
        rot[3][0] = m[3][0];
        rot[3][1] = m[3][1];
        rot[3][2] = m[3][2];
        m = rot;
    }

    void setRotMat(Transform t){
        glm::mat4 rot = t.getRotMat();
        rot[3][0] = m[3][0];
        rot[3][1] = m[3][1];
        rot[3][2] = m[3][2];
        m=rot;
    }

    void rotate( float angle,glm::vec3 axis){
        m= glm::rotate(m,angle,axis);
    }

    void translate(glm::vec3 v){
        m = glm::translate(m,v);
    }
};

#endif