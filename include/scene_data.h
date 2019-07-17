#ifndef SCENE_DATA_H
#define SCENE_DATA_H

#include "MolecularHandler.h"
#include "render_utils.h"
#include "enum_scene.h"
#include "spaceship_controller.h"
#include "Gamepad.h"
#include "CameraTrans.h"
#include "particles_emitter.h"

struct scene_docking_data
{
	//for the state machine in the main
	int nextscene;
	
	// stuff shared between scene
	SDL_Window* window;
	SDL_GLContext* context;

	SDL_bool* relativeMouseMode;
	 
	btDiscreteDynamicsWorld* dynamicsWorld;
	MolecularHandler* mh;
	ConstraintManager* cm;
	Camera* d_cam;//camera for the docking scene
	Camera* ss_cam;//camera for the spaceship scene
	spaceship_controller* spaceship;
	float* mouseSensitivity;


	lineRenderer* l;
	Gamepad* gpad;

	bool showCamFocus;
	bool* showSkybox;
	ImVec4* bgColor;
	// ContraintRenderer* c;

	//stuff that need only to exist in the scene
	//rendering stuff
	unsigned int vao;
	unsigned int quadVAO;
	unsigned int VAO_SKYBOX;
	unsigned int VAO_SPHERE;
	unsigned int texColorBuffer;
	unsigned int texDepthBuffer;
	unsigned int framebuffer;
	unsigned int cubemapTexture;

	int nbsphereVertices;
	//shader
	Shader screenQuadShader;
	Shader surfaceShader;
	Shader skyboxShader;
	Shader colorShader;

	//option stuff
	bool running ;
	bool surf ;
	bool optim;
	bool freeze;
	bool damp;
	bool* invertedAxis;
	bool* wasd;
	//time stuff
	float energyCalcTime;
	float elapsed;
	float averageFrameTimeMilliseconds;
	double  frameRate;
	unsigned int frames;
	float deltaTime;

	int fps; 

	double ener; //energy of the molecular complex
	float prog; //progress of optimisation

	//ui
	ImFont* font; //ui font
	ImTextureID optimTexture ;//image to use when doing optimisation
};


enum ViewMode { firstPerson,thirdPerson, NB_VIEW};//NB_VIEW should always be at the end, it give the size of the enum
enum Weapons {constraintMover,attract,repulse,NB_WEAPON};

struct scene_spaceship_data
{
	int currentWeapon;
	//for the state machine in the main
	int nextscene;
	// stuff shared between scene
	btDiscreteDynamicsWorld* dynamicsWorld;
	MolecularHandler* mh;
	ConstraintManager* cm;
	Camera* d_cam;//camera for the docking scene
	Camera* ss_cam;//camera for the spaceship scene
	spaceship_controller* spaceship;
	lineRenderer* l;
	Gamepad* gpad;

	ImVec4* bgColor;
	bool* showSkybox;

	ParticlesEmitter* pe;
	beamRenderer* beam;

	float* mouseSensitivity;
	// ContraintRenderer* c;

	//stuff that need only to exist in the scene

	

	//rendering stuff
	unsigned int vao;
	unsigned int quadVAO;
	unsigned int VAO_SPACESHIP;
	unsigned int VAO_SKYBOX;
	unsigned int VAO_SPHERE;
	unsigned int VAO_FLAME;

	unsigned int texColorBuffer;
	unsigned int texDepthBuffer;
	unsigned int framebuffer;
	unsigned int cubemapTexture;

	int spaceshipVertices;
	int nbsphereVertices;
	int nbflameVertices;

	int nbskyboxVertices;
	float* skyboxVertices;
	//shader
	Shader screenQuadShader;
	Shader surfaceShader;
	Shader spaceshipShader;
	Shader skyboxShader;
	Shader colorShader;

	//option stuff
	bool running ;
	bool surf ;
	bool optim;
	bool freeze;
	bool damp;
	bool fire; //is the spaceship firing ?
	bool spaceshipMoving; //is the spacehip Moving ?
	int viewMode;
	bool* invertedAxis;
	bool* wasd;
	//time stuff
	float energyCalcTime;
	float elapsed;
	float averageFrameTimeMilliseconds;
	double  frameRate;
	unsigned int frames;
	float deltaTime;

	int fps; 

	double ener; //energy of the molecular complex
	float prog; //progress of optimisation

	ImTextureID crosshairTexture;
	ImTextureID* weaponTextures;

	glm::vec3 beamHitPoint;
};

#endif