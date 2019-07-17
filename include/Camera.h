#ifndef CAMERA_H
#define CAMERA_H

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera
{
public:

	typedef enum {
		PROJECTION_ORTHO,
		PROJECTION_PERSPECTIVE
	} PROJTYPE;

	
	bool fps =false;//camera mode
	
	glm::vec3 Position; ///< Position de la camera
	glm::vec3 LookAt; ///< Point regarde par la camera
	glm::vec3 Direction; ///< Direction de la camera
	glm::vec3 UpVec; ///< Vecteur up de la camera
	glm::vec3 RightVec; ///< Si on se place dans la camera, indique la droite	
	glm::vec3 UpRef; ///< Ce qu'on considère comme le "haut" dans notre monde (et pas le up de la cam)
	

	PROJTYPE ProjType; ///< Type de projection
	float screen_width ;
    float screen_height;
	float FovY; ///< fov angle sur y in degrees
	float Ratio; ///< with / height
	float Left; ///< Left Plane
	float Right;
	float Bottom;
	float Top;
	float Near;
	float Far;
			
	Camera()
	{
		Position = glm::vec3(0.0f, -20.0f, 0.0f);
		LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
		UpRef = glm::vec3(0.0f, 1.0f,0.0f );
		UpVec = UpRef;
		FovY = 60.0f;
		Ratio = 800.0f / 600.0f;
		Near = 1.0f;
		Far = 500.0f;
		setProjectionPerspective(FovY, Ratio, Near, Far);
		updateVecs();
	}

	void setProjectionPerspective(float fovy, float ratio, float nearPlane, float farPlane) {
		ProjType = PROJECTION_PERSPECTIVE;
		FovY = fovy;
		Ratio = ratio;
		Near = nearPlane;
		Far = farPlane;
	}

	// void setProjectionOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
	// 	ProjType = PROJECTION_ORTHO;
	// 	Left = left;
	// 	Right = right;
	// 	Bottom = bottom;
	// 	Top = top;
	// 	Near = nearPlane;
	// 	Far = farPlane;
	// }

	/**
	* Mise a jour de la camera
	*/
	virtual void update(float elapsed)
	{

	}

	void setWindowSize(int w,int h)
    {
        screen_width = float(w);
        screen_height = float(h);
    }

	/**
	* Definition du point regarde
	*/
	void setLookAt(glm::vec3 lookat)
	{
		LookAt = lookat;
		updateVecs();
	}

	/**
	* Definition de la position de la camera
	*/
	void setPosition(glm::vec3 pos)
	{
		Position = pos;
		updateVecs();
	}

	/**
	* Definition du haut de notre monde
	*/
	void setUpRef(glm::vec3 upRef)
	{
		UpRef = upRef;
		updateVecs();
	}

	/**
	* Deplacement de la camera d'un delta donné
	*/
	void move(glm::vec3 delta)
	{
		Position += delta;
		LookAt += delta;
		updateVecs();
	}

	/**
	* Deplacement de la camera d'un delta donné
	*/
	void moveTo(const glm::vec3 & target)
	{
		this->move(target - Position);
	}

	/**
	* On recalcule les vecteurs utiles au déplacement de la camera (Direction, RightVec, UpVec)
	* on part du principe que sont connus :
	* - la position de la camera
	* - le point regarde par la camera
	* - la vecteur up de notre monde
	*/
	void updateVecs(void)
	{
		Direction = LookAt;
		Direction -= Position;
        Direction = glm::normalize(Direction);


		UpVec = UpRef;
		RightVec = glm::cross(Direction, UpVec);
		RightVec = glm::normalize(RightVec);


		UpVec = glm::cross(Direction, RightVec);
		UpVec = glm::normalize(UpVec);
	}

	/**
	* Rotation droite gauche en subjectif
	*/
	void rotate(float angle)
	{
		LookAt -= Position;
		LookAt = glm::rotate(LookAt, angle, UpRef);
		LookAt += Position;
		updateVecs();
	}

	/**
	* Rotation haut bas en subjectif
	*/
	void rotateUp(float angle)
	{
		glm::vec3 previousPos = Position;

		LookAt -= Position;
		LookAt = glm::rotate(LookAt, angle, RightVec);

		LookAt += Position;

		glm::vec3 normPos = Position;
        normPos = glm::normalize(normPos);
		float newAngle = glm::dot(normPos, UpRef);
		if (newAngle > 0.99f || newAngle < -0.99f)
			Position = previousPos;

		updateVecs();
	}

	/**
	* Rotation droite gauche en troisième personne
	*/
	void rotateAround(float angle)
	{
		Position -= LookAt;
		Position = glm::rotate(Position, angle, UpRef);
		Position += LookAt;
		updateVecs();
	}

	/**
	* Rotation haut bas en troisième personne
	*/
	void rotateUpAround(float angle)
	{
		Position -= LookAt;

		//On ne monte pas trop haut pour ne pas passer de l'autre coté
		glm::vec3 previousPos = Position;
		Position = glm::rotate(Position, angle, RightVec);
		glm::vec3 normPos = Position;
        normPos = glm::normalize(normPos);
		float newAngle = glm::dot(normPos, UpRef);
		if (newAngle > 0.99 || newAngle < -0.99)
			Position = previousPos;

		Position += LookAt;
		updateVecs();
	}

	// /**
	// * Calcul du bon repère de départ pour la matrice monde
	// */
	// void look(void)
	// {
	// 	glMatrixMode(GL_PROJECTION);
	// 	glLoadIdentity();
	// 	if (ProjType == PROJECTION_PERSPECTIVE) {
	// 		gluPerspective(FovY, Ratio, Near, Far);
	// 	}
	// 	if (ProjType == PROJECTION_ORTHO) {
	// 		glOrtho(Left, Right, Bottom, Top, Near, Far);
	// 	}
		
	// 	glMatrixMode(GL_MODELVIEW);
	// 	glLoadIdentity();

	// }

    glm::mat4 getProj()
    {    
        // projection matrix, to decide camera parameters (fov, screen ratio and cliping distance)
        return glm::perspective(glm::radians(FovY), screen_width / screen_height,  Near, Far); 
    }
	glm::mat4 getView()
    {
        return glm::lookAt(Position, LookAt, UpVec);
    }
    
};
#endif