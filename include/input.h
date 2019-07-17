#ifndef INPUT_H
#define INPUT_H

#if defined(_WIN32)
// SDL2 Headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#elif defined(__APPLE__)
// SDL2 Headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#else
// SDL2 Headers
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#include <glm/gtx/matrix_interpolation.hpp>

#include <math.h>

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

#include "MolecularHandler.h"
#include "motion.h"

#include "scene_data.h"
#include "spaceship_input.h"

void docking_input(scene_docking_data &docking_data)
{
	SDL_Event windowEvent;
	static btRigidBody* selectedBody;
	static bool mousePressed = false; //true down
    static bool FPS = false;
    static bool fps_forward = false;
    static bool fps_backward = false;
    static bool fps_right = false;
    static bool fps_left = false;
    static float timeAccelerating = 0.0f; 
    

    //gamepad input
    docking_data.gpad->input(docking_data.elapsed);


    float rt =docking_data.gpad->rightTrigger;
    if (rt >0){
        docking_data.d_cam->move(rt * docking_data.d_cam->Direction);
    }

    //backward
    float lt =docking_data.gpad->leftTrigger;
    if (lt >0){
       docking_data.d_cam->move(lt * -docking_data.d_cam->Direction);
    }

    // //rotation of camera
    glm::vec2 rl = docking_data.gpad->rightStick ;
    if (rl.x != 0.0f && rl.y!=0.0f){
        docking_data.d_cam->rotate(rl.x*0.02);
        if (*docking_data.invertedAxis){
            docking_data.d_cam->rotateUp(-rl.y*0.02);
        }
        else{
            docking_data.d_cam->rotateUp(rl.y*0.02);
        } 
    }


    

    // //lateral translation of the ship
    glm::vec3 ls = glm::vec3(docking_data.gpad->leftStick,0.0f);
    
    if (glm::length(ls) > 0){
        docking_data.d_cam->move(-ls.x*docking_data.d_cam->RightVec);
        docking_data.d_cam->move(-ls.y*docking_data.d_cam->UpVec);
           
    }




    //event handling
    while (SDL_PollEvent(&windowEvent))
    {
        ImGui_ImplSdlGL3_ProcessEvent(&windowEvent);
        switch (windowEvent.type)

        {

                // you can exit the program by clicking the cross button or pressing escape
            case SDL_QUIT: docking_data.nextscene = END;
                break;


                //mouse wheel control zoom
            case SDL_MOUSEWHEEL: {
                    docking_data.d_cam->Position += docking_data.d_cam->Direction * (windowEvent.wheel.y*2.0f *(*docking_data.mouseSensitivity));
                    docking_data.d_cam->updateVecs();
                }
                break;
                //you can rotate with the rigth click
            case SDL_MOUSEMOTION:
            {
                if (windowEvent.motion.state == SDL_BUTTON_MMASK)
                {
                    const Uint8 *keybState = SDL_GetKeyboardState(NULL);

                    if (keybState[SDL_SCANCODE_LSHIFT]){
                         docking_data.d_cam->move(docking_data.d_cam->UpVec * (windowEvent.motion.yrel *0.2f *(*docking_data.mouseSensitivity)));
                         docking_data.d_cam->move(docking_data.d_cam->RightVec * (windowEvent.motion.xrel *0.2f *(*docking_data.mouseSensitivity)));
                    }
                    else{
                         docking_data.d_cam->move(docking_data.d_cam->Direction * (windowEvent.motion.yrel *0.2f *(*docking_data.mouseSensitivity)));
                         docking_data.d_cam->move(docking_data.d_cam->RightVec * (windowEvent.motion.xrel *0.2f *(*docking_data.mouseSensitivity))); 
                    }
                }
                if (windowEvent.motion.state == SDL_BUTTON_RMASK)
                {
                    const Uint8 *keybState = SDL_GetKeyboardState(NULL);



                    if (keybState[SDL_SCANCODE_LSHIFT]){
                    //     docking_data.d_cam->FPSRotation(-windowEvent.motion.xrel,-windowEvent.motion.yrel);
                        docking_data.d_cam->rotateAround(windowEvent.motion.xrel *0.002);
                        if(*docking_data.invertedAxis){
                            docking_data.d_cam->rotateUpAround(-windowEvent.motion.yrel*0.002 *(*docking_data.mouseSensitivity));
                        }else{
                            docking_data.d_cam->rotateUpAround(windowEvent.motion.yrel*0.002 *(*docking_data.mouseSensitivity));
                        }
                        docking_data.d_cam->fps = false;
                    }
                    else{
                        
                    //     docking_data.d_cam->TPSRotation(-windowEvent.motion.xrel,-windowEvent.motion.yrel);
                        //fps rotation
                        docking_data.d_cam->rotate(windowEvent.motion.xrel *0.002 *(*docking_data.mouseSensitivity));
                        if(*docking_data.invertedAxis){
                            docking_data.d_cam->rotateUp(-windowEvent.motion.yrel*0.002 *(*docking_data.mouseSensitivity));
                        }else{
                            docking_data.d_cam->rotateUp(windowEvent.motion.yrel*0.002 *(*docking_data.mouseSensitivity));
                        }
                        docking_data.d_cam->fps = true;

                    }
                }
                else
                {
                    // docking_data.d_cam->arcBallRotation(false,windowEvent.motion.x, windowEvent.motion.y);


                }
                
                if (windowEvent.motion.state == SDL_BUTTON_LMASK)
                {   float updatePhy = glm::min(docking_data.elapsed,1.0f/60.0f); //Bullet fait de la merde si sous 60hz...
                    glm::vec3 hitPoint =  glm::vec3();
                    btRigidBody* currentBody = getPointedBody(docking_data.mh->dynamicsWorld,*docking_data.d_cam,hitPoint);
                    if (selectedBody == currentBody){
                        mousePollFunction(true,docking_data.mh->dynamicsWorld,*docking_data.d_cam,updatePhy);
                    }else{
                    mousePollFunction(false,docking_data.mh->dynamicsWorld,*docking_data.d_cam,updatePhy);
                    }
                }else{
                    float updatePhy = glm::min(docking_data.elapsed,1.0f/60.0f); //Bullet fait de la merde si sous 60hz...
                    mousePollFunction(false,docking_data.mh->dynamicsWorld,*docking_data.d_cam,updatePhy);
                }
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            { 	
            	mousePressed =true;
                if (windowEvent.button.button == SDL_BUTTON_LMASK)
                {
                    //double click
                    if (windowEvent.button.clicks == 2){
                        std::cout<<"double "<<std::endl;

                        glm::vec3 hitPoint =  glm::vec3();
                        btRigidBody* body = getPointedBody(docking_data.mh->dynamicsWorld,*docking_data.d_cam,hitPoint);
                        if (body ){
                            glm::vec3 molCenter= glm::vec3(body->getCenterOfMassPosition ().getX(),body->getCenterOfMassPosition ().getY(),body->getCenterOfMassPosition ().getZ());
                            glm::vec3 vec = molCenter - docking_data.d_cam->LookAt;
                            docking_data.d_cam->LookAt += vec;
                            // docking_data.d_cam->Position += vec;
                            //calculate the distance the camera should be so the mol take two third of the screen (3 radius of the mol)
                            //1 get the molecule and the radius
                            Molecule m = docking_data.mh->getMol(body);
                            float radius = m.radius; 
                            // std::cout<<"m radius "<< radius <<std::endl;

                            //2formula
                            float TwoThirdDist = (radius*1.5f)/ tan(glm::radians(docking_data.d_cam->FovY / 2.0f ));
                            // std::cout<<"TwoThirdDist "<< TwoThirdDist <<std::endl;
                            //3 adjust position of camera
                            docking_data.d_cam->Position = docking_data.d_cam->LookAt - (docking_data.d_cam->Direction *TwoThirdDist);
                        }
                    }



                    //Clavier
                    
                    // mousePollFunction(true,mh.dynamicsWorld,cam,elapsed);
                    const Uint8 *keybState = SDL_GetKeyboardState(NULL);
                    //si on click gauche + crtl, on place une contrainte
                    if (keybState[SDL_SCANCODE_LCTRL]){
                        
                        glm::vec3 hitPoint =  glm::vec3();
                        btRigidBody* body = getPointedBody(docking_data.mh->dynamicsWorld,*docking_data.d_cam,hitPoint);
                        if (body ){
                            btScalar m[16];
                            body->getWorldTransform().getOpenGLMatrix(m );

                            glm::mat4 parentphymodel = glm::make_mat4(m);
                            // glm::mat4 parentphymodel = glm::mat4(1);
                            // body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(parentphymodel ));
                            parentphymodel = glm::inverse(parentphymodel);
                            //on multiplie par l'inverse de la matrice physique pour avoir la position relative du point
                            hitPoint =   glm::vec3((parentphymodel)*glm::vec4(hitPoint,1.0f));
                            docking_data.cm->addConstraint(hitPoint,body);
                            // std::cout<<cm.constraints.size()<<std::endl;
                        }
                    }
                    // else if (keybState[SDL_SCANCODE_LSHIFT]){

                    //sinon on selectionne une molecule pour la faire tourner
                    else if (!keybState[SDL_SCANCODE_LSHIFT]){
                    	
                        glm::vec3 hitPoint =  glm::vec3();
                        selectedBody = getPointedBody(docking_data.mh->dynamicsWorld,*docking_data.d_cam,hitPoint);

                    }
                }

                if (windowEvent.button.button == SDL_BUTTON_RIGHT)
                {
               
                    
                 
                    // std::cout<<"rclic end !" <<std::endl;
                    //Clavier
                    // std::cout<<"harh"<<std::endl;

                    const Uint8 *keybState = SDL_GetKeyboardState(NULL);
                    if (keybState[SDL_SCANCODE_LCTRL])
                    {
                        glm::vec3 hitPoint =  glm::vec3();
                        btRigidBody* body = getPointedBody(docking_data.mh->dynamicsWorld,*docking_data.d_cam,hitPoint);
                        //if we touch a mol
                        if (body )
                        {
                        	// std::cout<<"hit"<<std::endl;
                        	//and we touch a constraint
                        	int constraintID = docking_data.cm->getConstraintID(hitPoint);
                        	std::cout<<"ID "<<constraintID<<std::endl;
                        	if (constraintID != -1)
                        	{
                        		//remove the constraint
                        		// if there is more than one element
                        		if (docking_data.cm->constraints.size()>1){
                        			if (constraintID %2 ==0){
                        				//if the id point on the last element of the constraint list and that id is even, then it's an isolated constraint
                        				if ((unsigned int )constraintID ==docking_data.cm->constraints.size()-1){
											docking_data.cm->constraints.erase(docking_data.cm->constraints.begin()+constraintID);
                        				}else{
                        				docking_data.cm->constraints.erase(docking_data.cm->constraints.begin()+constraintID,docking_data.cm->constraints.begin()+constraintID+2);
                        			}
                        			}else{
                        				docking_data.cm->constraints.erase(docking_data.cm->constraints.begin()+constraintID -1 ,docking_data.cm->constraints.begin()+constraintID+1);
                        			}
                        		}
                        		else{
                        			//if there only one element
                        			docking_data.cm->constraints.erase(docking_data.cm->constraints.begin()+constraintID);
                        		}
                        	}	
                        }
                    }

                    else {
                        FPS = true;
                        //when we start the FPS view the mouse is placed at the center of the screen
                        SDL_WarpMouseInWindow(docking_data.window,docking_data.d_cam->screen_width/2, docking_data.d_cam->screen_height/2);
                        //however this generate a motion event so we flush it (we pump event to the queue to be sure to capture it)
                        SDL_PumpEvents();
                        SDL_FlushEvent(SDL_MOUSEMOTION);
                    }
                }
                break;
            
                //user drag and drop a file
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (windowEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    FPS = false;
                } 
            	mousePressed = false;
            	break;
            }
            case SDL_DROPFILE:
            {
                char* dropped_filedir = windowEvent.drop.file;
                Molecule mol = readMOL2(dropped_filedir);

                //use molecularHandler to manage new data
                docking_data.mh->addMol(mol);
                //move camera to new center of scene
                // docking_data.d_cam->lookAtPoint(docking_data.mh->getCenterofScene());
                docking_data.d_cam->setPosition( docking_data.mh->getCenterofScene()+glm::vec3(0.0,0.0,150.0));
               
                // Free dropped_filedir memory
                SDL_free(dropped_filedir);
                break;
            }
                // a key is pressed
            case SDL_KEYDOWN:
            {
                switch (SDL_GetScancodeFromKey(windowEvent.key.keysym.sym))
                // switch (windowEvent.key.keysym.sym)
                {
                    case SDL_SCANCODE_SPACE:
                    // case SDLK_SPACE:
                    {
                        docking_data.cm->applyDockingForces(docking_data.mh->getCenterofScene());
                        break;
                    }


                    //fps control

                     case SDL_SCANCODE_W:
                    // case SDLK_w:
                    {

                            fps_forward = true;
                        break;
                    }   

                    case SDL_SCANCODE_A:
                    // case SDLK_a: 
                    {

                            fps_left = true;
                        
                        break;
                    }

                    case SDL_SCANCODE_D:
                    // case SDLK_d: 
                    {
                        fps_right = true;
                        break;
                    }

                    case SDL_SCANCODE_S:
                    // case SDLK_s: 
                    {
                        
                        fps_backward = true;
                        break;
                    }

                    case SDL_SCANCODE_F:
                    // case SDLK_f: 
                    {
                        docking_data.showCamFocus = !docking_data.showCamFocus;
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
                // a key is released
            case SDL_KEYUP:
            {
                switch (SDL_GetScancodeFromKey(windowEvent.key.keysym.sym))
                // switch (windowEvent.key.keysym.sym)
                {

                    case SDL_SCANCODE_ESCAPE:
                    // case SDLK_ESCAPE :
                    {
                        docking_data.nextscene = END;
                        break;
                    }

                    case SDL_SCANCODE_TAB: 
                    // case SDLK_TAB:
                    {
                        docking_data.surf = !docking_data.surf;
                        docking_data.nextscene = spaceship;
                        break;
                    }


                    case SDL_SCANCODE_O:
                    // case SDLK_o:
 
                    {
                        docking_data.optim = !docking_data.optim;
                        docking_data.freeze = true;
                        break;
                    }

                    //fps control

                     case SDL_SCANCODE_W://beware QWERTY
                    // case SDLK_w:
                    {

                            // docking_data.d_cam->move(docking_data.d_cam->Direction);
                            fps_forward = false;
                            timeAccelerating = 0.0f;
                        break;
                    }   

                    case SDL_SCANCODE_A: //beware QWERTY
                    // case SDLK_a:
                    {
                            fps_left = false;
                        break;
                    }

                    // case SDLK_SCANCODE_Z://beware QWERTY
                    // {
                    //     if ( *docking_data.wasd == false)
                    //     {
                    //         // docking_data.d_cam->move(glm::vec3(0.0f,0.0f,-1.0f));
                    //         // docking_data.d_cam->move(docking_data.d_cam->Direction);
                    //         fps_forward = false;
                    //         timeAccelerating = 0.0f;
                    //     }
                    //     break;
                    // }   

                    // case SDLK_SCANCODE_Q: //beware QWERTY
                    // {
                    //     if ( *docking_data.wasd == false){
                    //         // docking_data.d_cam->move(docking_data.d_cam->RightVec);
                    //         fps_left = false;
                    //     }
                    //     break;
                    // }


                    case SDL_SCANCODE_D: //beware QWERTY
                    // case SDLK_d:
                    {
                        // docking_data.d_cam->move(-docking_data.d_cam->RightVec);
                        fps_right = false;
                        break;
                    }

                    case SDL_SCANCODE_S: //beware QWERTY
                    // case SDLK_s:
                    {
                        // docking_data.d_cam->move(-docking_data.d_cam->Direction);
                        fps_backward = false;
                        break;
                    }
                    

                    default:
                        break;
                }
                break;
            }

        }

        if (windowEvent.type == SDL_CONTROLLERDEVICEADDED) {
            docking_data.gpad->init();
        }

        if (windowEvent.type == SDL_CONTROLLERDEVICEREMOVED) {
            docking_data.gpad->clean();
        }
    } 


    //cam movement
    if (fps_left){
        docking_data.d_cam->move(docking_data.d_cam->RightVec);

    }

    if (fps_right){
        docking_data.d_cam->move(-docking_data.d_cam->RightVec);
    }

    if (fps_forward){
        timeAccelerating += docking_data.elapsed;
        float speed = 1.5f;
        if (timeAccelerating> 0.5){
            speed = 2.2f;
        } 
        if (timeAccelerating> 0.75){
            speed = 2.7f;
        }
        if (timeAccelerating> 1.2){
            speed = 6.0f;
        }
        
        docking_data.d_cam->move(speed * docking_data.d_cam->Direction);
        
    }

    if (fps_backward){
        timeAccelerating += docking_data.elapsed;
        float speed = 1.5f;
        if (timeAccelerating> 0.5){
            speed = 2.2f;
        } 
        if (timeAccelerating> 0.75){
            speed = 2.7f;
        }
        if (timeAccelerating> 1.2){
            speed = 6.0f;
        }
        docking_data.d_cam->move(speed * -docking_data.d_cam->Direction);
    }


    if (FPS){
        //we erase the mouse from the screen
               *docking_data.relativeMouseMode = SDL_TRUE;
    }

    if (mousePressed)
    {
    	// std::cout<<"pressed"<<std::endl;
    	if (windowEvent.button.button == SDL_BUTTON_LMASK)
        {
        	const Uint8 *keybState = SDL_GetKeyboardState(NULL);
            if (keybState[SDL_SCANCODE_LSHIFT])
            {
                glm::vec3 hitPoint =  glm::vec3();
                btRigidBody* body = getPointedBody(docking_data.mh->dynamicsWorld,*docking_data.d_cam,hitPoint);
                if (body ){
                    int constraintID = docking_data.cm->getConstraintID(hitPoint);
                	std::cout<<"ID "<<constraintID<<std::endl;
                	if (constraintID != -1)
                	{
                        btScalar m[16];

                        body->getWorldTransform().getOpenGLMatrix(m );
                        // glm::mat4 phymodel = glm::make_mat4(m);

                		glm::mat4 parentphymodel = glm::make_mat4(m);
                        // body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(parentphymodel ));
                        parentphymodel = glm::inverse(parentphymodel);
                        //on multiplie par l'inverse de la matrice physique pour avoir la position relative du point
                		docking_data.cm->constraints[constraintID].pos = glm::vec3((parentphymodel)*glm::vec4(hitPoint,1.0f)) ;
                	}
                }
            }
        }
    }  
}





void spaceship_input(scene_spaceship_data &spaceship_data)
{
	SDL_Event windowEvent;
	// static btRigidBody* selectedBody;
	static bool mousePressedLeft = false; //true down
    static bool mousePressedRight = false; //true down

    mousePressedLeft = mousePressedLeft;
    mousePressedRight = mousePressedRight;//
    //event handling

	int x,y;
	SDL_GetMouseState(&x,&y);
    // spaceship_data.spaceship->arcBallRotation( x,y );
	// spaceship_data.spaceship->mouse_callback(x,y);

    //game pad input-----------------------------------------------------
    spaceship_data.gpad->input(spaceship_data.elapsed);

    // spaceshipInput(spaceship_data.spaceship, spaceship_data.gpad, &windowEvent );
    // //forward


    // spaceship_data.spaceshipMoving = false;
    // float rt =spaceship_data.gpad->rightTrigger;
    // if (rt >0){
        
    // }

    float rt =spaceship_data.gpad->rightTrigger;
    if (rt >0){
        spaceship_data.spaceshipMoving = true;
        spaceship_data.spaceship->addForce(spaceship_data.spaceship->direction*spaceship_data.spaceship->speed*rt);
        
         // std::cout << "right Trigger " << rt << std::endl;
    }

    //backward
    float lt =spaceship_data.gpad->leftTrigger;
    if (lt >0){
        spaceship_data.spaceship->addForce(-spaceship_data.spaceship->direction*spaceship_data.spaceship->speed*lt);
    }

    //rotation of camera
    glm::vec2 rl = spaceship_data.gpad->rightStick ;
    if (*spaceship_data.invertedAxis){
        rl.y= -rl.y;
    }
    if (rl.x != 0.0f && rl.y!=0.0f){
        spaceship_data.spaceship->setTorque(rl);
    }

    // //lateral translation of the ship
    glm::vec3 ls = glm::vec3(spaceship_data.gpad->leftStick,0.0f);
    //ugly as sin
    btVector3 f = spaceship_data.spaceship->body->getWorldTransform().getBasis()*btVector3(0.0f,ls.y,0.0f);

    
    if (glm::length(ls) > 0){
       spaceship_data.spaceship->addForce(glm::vec3(f.x(),f.y(),f.z()) * (spaceship_data.spaceship->speed*0.8f));
    }

    f = spaceship_data.spaceship->body->getWorldTransform().getBasis()*btVector3(-ls.x,0.0f,0.0f);

    
    if (glm::length(ls) > 0){
       spaceship_data.spaceship->addForce(glm::vec3(f.x(),f.y(),f.z()) * (spaceship_data.spaceship->speed*0.8f));
    }


    //rotation
    if (spaceship_data.gpad-> shoulderLeftPress){
        spaceship_data.spaceship->rotate=true;
        spaceship_data.spaceship->angleRot = 1.0f;
    } 

    if (spaceship_data.gpad-> shoulderRightPress){

        spaceship_data.spaceship->rotate=true;
        spaceship_data.spaceship->angleRot = -1.0f;
    }


    //back button reset the scene
    if (spaceship_data.gpad->backPress && ! spaceship_data.gpad->backPressOld){
        //erase constraint
        spaceship_data.cm->constraints.clear();

        // repostion molecules
        for (unsigned int i =0;i< spaceship_data.mh->molecules.size();i++){
            btTransform startTransform;
            startTransform.setIdentity();

            glm::vec3 ip =  spaceship_data.mh->getInitialPos(i);
            startTransform.setOrigin(btVector3(ip.x,ip.y,ip.z));

            spaceship_data.mh->rigidBody[i]->setActivationState(ACTIVE_TAG);

            btDefaultMotionState* myMotionState = (btDefaultMotionState*)spaceship_data.mh->rigidBody[i]->getMotionState();

            myMotionState->setWorldTransform(startTransform);
            spaceship_data.mh->rigidBody[i]->setWorldTransform(startTransform);
            spaceship_data.mh->rigidBody[i]->setMotionState(myMotionState);
        }
        //position of spaceship
        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3(0.0f,0.0f,150.0f));

        spaceship_data.spaceship->body->setActivationState(ACTIVE_TAG);
        btDefaultMotionState* myMotionState = (btDefaultMotionState*)spaceship_data.spaceship->body->getMotionState();
        
        myMotionState->setWorldTransform(t);
        spaceship_data.spaceship->body->setWorldTransform(t);
        spaceship_data.spaceship->body->setMotionState(myMotionState);

        //spaceship cam pos
        spaceship_data.ss_cam->setPosition( spaceship_data.mh->getCenterofScene()+glm::vec3(0.0,0.0,150.0));
        spaceship_data.ss_cam->setLookAt(glm::vec3(0.0,0.0,0.0));

        spaceship_data.d_cam->setPosition( spaceship_data.mh->getCenterofScene()+glm::vec3(0.0,0.0,150.0));
        spaceship_data.d_cam->setLookAt(glm::vec3(0.0,0.0,0.0));
    }



    if (spaceship_data.gpad->YPress && ! spaceship_data.gpad->YPressOld){
        // spaceship_data.fire = true;
         spaceship_data.viewMode = (spaceship_data.viewMode+1)%NB_VIEW;

        if (spaceship_data.viewMode == thirdPerson){
            spaceship_data.ss_cam->setPosition( spaceship_data.spaceship->getCamPos());
            spaceship_data.ss_cam->setLookAt( spaceship_data.spaceship->getCamPointToLookAT());
        }
        if(spaceship_data.viewMode == firstPerson){
            spaceship_data.ss_cam->setPosition( spaceship_data.spaceship->getPos());
            spaceship_data.ss_cam->setLookAt( spaceship_data.spaceship->getPos() + (100.0f * spaceship_data.spaceship->getDirection()));
        }
        // std::cout << "current viewMode : "<< spaceship_data.viewMode<< std::endl;
    } 

    //fire
    if (spaceship_data.gpad->APress){
        spaceship_data.fire = true;
        spaceship_data.gpad->rumble();
    } 

    // on release
    if (!spaceship_data.gpad->APress && spaceship_data.gpad->APressOld){
        spaceship_data.fire = false;
    }


    // Weapon change
    if (spaceship_data.gpad->dPadRightPress && !spaceship_data.gpad->dPadRightPressOld){
        spaceship_data.currentWeapon = (spaceship_data.currentWeapon+1)%NB_WEAPON;
    }
    if (spaceship_data.gpad->dPadLeftPress && !spaceship_data.gpad->dPadLeftPressOld){
        //c++ doesnt give you "true" modulo but only remainder
        spaceship_data.currentWeapon = ((spaceship_data.currentWeapon -1)%NB_WEAPON + NB_WEAPON)%NB_WEAPON;
    }

    // //rotation of camera
    // glm::vec2 rl = spaceship_data.gpad->rightStick ;
    // if (glm::length(rl) > 0){

    //     rl = rl * 50.0f;

    //     if (*spaceship_data.invertedAxis==false){
    //         spaceship_data.spaceship->MouseRotation(-rl.x,-rl.y);
    //     }else{
    //         spaceship_data.spaceship->MouseRotation(-rl.x,rl.y);
    //     }
     
    // }


    //mouse and keyboard input-------------------------------------------------
    while (SDL_PollEvent(&windowEvent))
    {
        ImGui_ImplSdlGL3_ProcessEvent(&windowEvent);
        switch (windowEvent.type)

        {
    
            // case SDL_WINDOWEVENT:
            // {
            //     switch (windowEvent.window.event)
            //     {
            //         case SDL_WINDOWEVENT_SIZE_CHANGED:
            //         {

            //             spaceship_data.ss_cam->setWindowSize(windowEvent.window.data1, windowEvent.window.data2);
            //             glViewport(0, 0, windowEvent.window.data1, windowEvent.window.data2);
            //             break;
            //         }
            //     }
            //     break;
            // }

                // you can exit the program by clicking the cross button or pressing escape
            case SDL_QUIT: spaceship_data.nextscene = END;
                break;


            case SDL_MOUSEMOTION:
            {
                        // spaceship_data.d_cam->rotate(windowEvent.motion.xrel *0.002);
                        // spaceship_data.d_cam->rotateUp(windowEvent.motion.yrel*0.002);
                        glm::vec2 rl;
                        if (*spaceship_data.invertedAxis){
                            rl = glm::vec2(windowEvent.motion.xrel *0.05 *(*spaceship_data.mouseSensitivity), -windowEvent.motion.yrel *0.05 *(*spaceship_data.mouseSensitivity));
                        }
                        else{
                            rl = glm::vec2(windowEvent.motion.xrel *0.05 *(*spaceship_data.mouseSensitivity),windowEvent.motion.yrel *0.05 *(*spaceship_data.mouseSensitivity));
                        }
                        spaceship_data.spaceship->setTorque(rl);
            
            	// if (*spaceship_data.invertedAxis==false){
             //        spaceship_data.spaceship->MouseRotation(-windowEvent.motion.xrel,windowEvent.motion.yrel);
             //    }else{
             //        spaceship_data.spaceship->MouseRotation(-windowEvent.motion.xrel,-windowEvent.motion.yrel);
             //    }
             //    spaceship_data.spaceship->updateVecs();

                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
            	
                
                if (windowEvent.button.button == SDL_BUTTON_LMASK)
                {
                    mousePressedLeft =true;
                    spaceship_data.fire = true;
                }
                if (windowEvent.button.button == SDL_BUTTON_RMASK)
                {
                    mousePressedRight =true;
                }
                break;
            
                //user drag and drop a file
            }
            case SDL_MOUSEBUTTONUP:
            {
            	if (windowEvent.button.button == SDL_BUTTON_LMASK)
                {
                    mousePressedLeft =false;
                }
                if (windowEvent.button.button == SDL_BUTTON_RMASK)
                {
                    mousePressedRight =false;
                }

            	spaceship_data.fire = false;
            	break;
            }
            case SDL_DROPFILE:
            {
                char* dropped_filedir = windowEvent.drop.file;
                Molecule mol = readMOL2(dropped_filedir);

                //use molecularHandler to manage new data
                spaceship_data.mh->addMol(mol);
                //move camera to new center of scene
                // spaceship_data.ss_cam->lookAtPoint(spaceship_data.mh->getCenterofScene());
                spaceship_data.ss_cam->Position = spaceship_data.mh->getCenterofScene()+glm::vec3(0.0,0.0,150.0);
               
                // Free dropped_filedir memory
                SDL_free(dropped_filedir);
                break;
            }
                // a key is pressed
            case SDL_KEYDOWN:
            {
                switch (SDL_GetScancodeFromKey(windowEvent.key.keysym.sym))
                // switch (windowEvent.key.keysym.sym)
                {
      //               case SDLK_SPACE:
      //               {
      //               	spaceship_data.spaceship->forward();
						// spaceship_data.spaceship->forward();
						// spaceship_data.spaceship->forward();
      //                   break;
      //               }
                    // case SDLK_RIGHT: 
                    // 	spaceship_data.followcam->rotateAround(glm::radians(5.0f));
                    // 	spaceship_data.spaceship->forceCam = false;
                    //     break;
                    // case SDLK_LEFT: 
                    // 	spaceship_data.followcam->rotateAround(glm::radians(-5.0f));
                    // 	spaceship_data.spaceship->forceCam = false;
                    //     break;

                    case SDL_SCANCODE_W://beware QWERTY
                    // case SDLK_w:
                    {
                            // spaceship_data.spaceship->forward();
                            spaceship_data.spaceship->addForce(spaceship_data.spaceship->direction*spaceship_data.spaceship->speed * 2.0f);

                            spaceship_data.spaceshipMoving = true;
                        
                        break;
                    }   

                    case SDL_SCANCODE_A: //beware QWERTY
                    // case SDLK_a:
                    {
                        // spaceship_data.spaceship->rotateAround(glm::radians(3.0f));
                            // spaceship_data.spaceship->strafingleft();
                             btVector3 f = spaceship_data.spaceship->body->getWorldTransform().getBasis()*btVector3(1.0f,0.0f,0.0f);
                            glm::vec3 ls = glm::vec3(f.x(),-f.y(),f.z());

                            spaceship_data.spaceship->addForce(ls * (spaceship_data.spaceship->speed* 2.0f));
                        
                        break;
                    }

                        //rotation
                    case SDL_SCANCODE_Q:
                                    {
                        spaceship_data.spaceship->rotate=true;
                        spaceship_data.spaceship->angleRot = 1.0f;
                        break;
                    } 

                    case SDL_SCANCODE_E:
                    {
                        spaceship_data.spaceship->rotate=true;
                        spaceship_data.spaceship->angleRot = -1.0f;
                        break;
                    }
                    // case SDL_SCANCODE_Z://beware QWERTY
                    // {
                    //     if (*spaceship_data.wasd == false){
                    //         // spaceship_data.spaceship->forward();
                    //         spaceship_data.spaceship->addForce(spaceship_data.spaceship->direction*spaceship_data.spaceship->speed * 2.0f);
                    //         spaceship_data.spaceshipMoving = true;
                    //     }
                    //     break;
                    // }   

                    // case SDL_SCANCODE_Q: //beware QWERTY
                    // {
                    //     // spaceship_data.spaceship->rotateAround(glm::radians(3.0f));
                    //     if (*spaceship_data.wasd == false){
                    //         btVector3 f = spaceship_data.spaceship->body->getWorldTransform().getBasis()*btVector3(1.0f,0.0f,0.0f);
                    //         glm::vec3 ls = glm::vec3(f.x(),-f.y(),f.z());

                    //         spaceship_data.spaceship->addForce(ls * (spaceship_data.spaceship->speed* 2.0f));
                    //         // spaceship_data.spaceship->strafingleft();
                    //     }
                    //     break;
                    // }


                    case SDL_SCANCODE_D: //beware QWERTY
                    // case SDLK_d:
                    {
                        // spaceship_data.spaceship->rotateAround(glm::radians(-3.0f));
                        // spaceship_data.spaceship->strafingright();
                            btVector3 f = spaceship_data.spaceship->body->getWorldTransform().getBasis()*btVector3(-1.0f,0.0f,0.0f);
                            glm::vec3 ls = glm::vec3(f.x(),-f.y(),f.z());

                            spaceship_data.spaceship->addForce(ls * (spaceship_data.spaceship->speed* 2.0f));
                        break;
                    }

                    case SDL_SCANCODE_S: //beware QWERTY
                    // case SDLK_s:
                    {
                            // spaceship_data.spaceship->backward();
                            spaceship_data.spaceship->addForce(-spaceship_data.spaceship->direction*spaceship_data.spaceship->speed * 2.0f);

                        break;
                    }

                    default:
                        break;

                }
                break;
            }
                // a key is released
            case SDL_KEYUP:
            {
            	spaceship_data.spaceshipMoving = false;
                switch (SDL_GetScancodeFromKey(windowEvent.key.keysym.sym))
                // switch (windowEvent.key.keysym.sym)
                {

                    case SDL_SCANCODE_ESCAPE:
                    // case SDLK_ESCAPE: 
                    {
                        // spaceship_data.running = false;
                        spaceship_data.nextscene = END;
                        break;
                    }

                    case SDL_SCANCODE_TAB:
                    // case SDLK_TAB:
                    {
                        // spaceship_data.surf = !spaceship_data.surf;
                        spaceship_data.nextscene = docking;
                        break;
                    }



                    case SDL_SCANCODE_O:
                    // case SDLK_o: 
                    {
                        spaceship_data.optim = !spaceship_data.optim;
                        spaceship_data.freeze = true;
                        break;
                    }

                    case SDL_SCANCODE_V:
                    // case SDLK_v: 
                    {
                        spaceship_data.viewMode = (spaceship_data.viewMode+1)%NB_VIEW;

                        if (spaceship_data.viewMode == thirdPerson){
                            spaceship_data.ss_cam->setPosition( spaceship_data.spaceship->getCamPos());
                            spaceship_data.ss_cam->setLookAt( spaceship_data.spaceship->getCamPointToLookAT());
                        }
                        if(spaceship_data.viewMode == firstPerson){
                            spaceship_data.ss_cam->setPosition( spaceship_data.spaceship->getPos());
                            spaceship_data.ss_cam->setLookAt( spaceship_data.spaceship->getPos() + (100.0f * spaceship_data.spaceship->getDirection()));
                        }
                        std::cout << "current viewMode : "<< spaceship_data.viewMode<< std::endl;
                        break;
                    }
                	

                    case SDL_SCANCODE_R:
                    // case SDLK_r: 
                    {
                        spaceship_data.currentWeapon = (spaceship_data.currentWeapon+1)%NB_WEAPON;

                        break;
                    }

                    default:
                        break;
                	// spaceship_data.spaceship->forward();
                }
                break;
            }

        }

        if (windowEvent.type == SDL_CONTROLLERDEVICEADDED) {
            spaceship_data.gpad->init();
        }

        if (windowEvent.type == SDL_CONTROLLERDEVICEREMOVED) {
            spaceship_data.gpad->clean();
        }
    } 
    if (spaceship_data.fire)
    {   

        switch (spaceship_data.currentWeapon)
        {
            case (constraintMover):
            {
            	//deplacer les contraintes	

            	// spaceship_data.fire = true;

            	glm::vec3 hitPoint =  glm::vec3();

            	glm::vec3 shipPos = spaceship_data.spaceship->getPos();
            	// glm::vec3 rayEnd = glm::vec3(spaceship_data.spaceship->position.m*glm::vec4(spaceship_data.spaceship->direction*50.0,1.0));
                // glm::vec3 rayEnd = spaceship_data.spaceship->position.getPos() + (spaceship_data.spaceship->position.getDirection() * 50.0f);

                // glm::vec3 rayEnd = spaceship_data.spaceship->getPos() + (spaceship_data.spaceship->getDirection() * 50.0f);
                glm::vec3 rayEnd = spaceship_data.spaceship->getPos() + (spaceship_data.ss_cam->Direction * 50.0f);


            	btRigidBody* body = rayPickup(spaceship_data.mh->dynamicsWorld,shipPos,rayEnd,hitPoint);
                spaceship_data.beamHitPoint = hitPoint;
                // btRigidBody* body = getPointedBody(spaceship_data.mh->dynamicsWorld,*spaceship_data.ss_cam,hitPoint);
                
                if (body ){
                    int constraintID = spaceship_data.cm->getConstraintID(hitPoint);
                	if (constraintID != -1)
                	{
                        btScalar m[16];

                        body->getWorldTransform().getOpenGLMatrix(m );
                        // glm::mat4 phymodel = glm::make_mat4(m);

                        glm::mat4 parentphymodel = glm::make_mat4(m);

                		// glm::mat4 parentphymodel = glm::mat4(1);
                  //       body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(parentphymodel ));
                        parentphymodel = glm::inverse(parentphymodel);
                        //on multiplie par l'inverse de la matrice physique pour avoir la position relative du point
                		spaceship_data.cm->constraints[constraintID].pos = glm::vec3((parentphymodel)*glm::vec4(hitPoint,1.0f)) ;
                	}
                }
                
                break;
            }
            case (attract):
            {

                //repousser MOlecules
                // spaceship_data.fire = true;

                glm::vec3 hitPoint =  glm::vec3();

                glm::vec3 shipPos = spaceship_data.spaceship->getPos();
                // glm::vec3 rayEnd = glm::vec3(spaceship_data.spaceship->position.m*glm::vec4(spaceship_data.spaceship->direction*100.0,1.0));
                // glm::vec3 rayEnd = spaceship_data.spaceship->position.getPos() + (spaceship_data.spaceship->position.getDirection() * 200.0f);
                glm::vec3 rayEnd = spaceship_data.spaceship->getPos() + (spaceship_data.spaceship->getDirection() * 200.0f);

                btRigidBody* body = rayPickup(spaceship_data.mh->dynamicsWorld,shipPos,rayEnd,hitPoint);
                spaceship_data.beamHitPoint = hitPoint;
                // btRigidBody* body = getPointedBody(spaceship_data.mh->dynamicsWorld,*spaceship_data.ss_cam,hitPoint);
                
                if (body ){
                    glm::vec3 force = glm::normalize(hitPoint - shipPos)*-0.5f;
                    // glm::vec3 force = glm::normalize(getCenter(body) - shipPos)*-100.0f;

                    // applyImpulse(body,force,hitPoint);
                    // applyForceCenter(body,force);
                    applyImpulseCenter(body,force);

                }
                
                break;
            }
            case (repulse):
            {
                //repousser MOlecules

                // spaceship_data.fire = true;

                glm::vec3 hitPoint =  glm::vec3();

                glm::vec3 shipPos = spaceship_data.spaceship->getPos();
                // glm::vec3 rayEnd = glm::vec3(spaceship_data.spaceship->position.m*glm::vec4(spaceship_data.spaceship->direction*100.0,1.0));
                // glm::vec3 rayEnd = spaceship_data.spaceship->position.getPos() + (spaceship_data.spaceship->position.getDirection() * 200.0f);
                glm::vec3 rayEnd = spaceship_data.spaceship->getPos() + (spaceship_data.spaceship->getDirection() * 200.0f);

                btRigidBody* body = rayPickup(spaceship_data.mh->dynamicsWorld,shipPos,rayEnd,hitPoint);
                spaceship_data.beamHitPoint = hitPoint;
                // btRigidBody* body = getPointedBody(spaceship_data.mh->dynamicsWorld,*spaceship_data.ss_cam,hitPoint);
                
                if (body ){
                    glm::vec3 force = glm::normalize(hitPoint - shipPos)*0.5f;
                    // glm::vec3 force = glm::normalize(getCenter(body) - shipPos)*-100.0f;

                    // applyImpulse(body,force,hitPoint);
                    // applyForceCenter(body,force);
                    applyImpulseCenter(body,force);
                }
                
                break;
            }
            break;
        }
    }     
}

#endif