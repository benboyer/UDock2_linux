#ifndef SPACESHIP_SCENE_H
#define SPACESHIP_SCENE_H
#include "scene_data.h"
#include "enum_scene.h"


bool isSpaceshipCollision(scene_spaceship_data& spaceship_data){
    int numManifolds = spaceship_data.dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i=0;i<numManifolds;i++){
        btPersistentManifold * contactManifold = spaceship_data.dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

        int numContacts = contactManifold->getNumContacts();
        if (numContacts > 0)
        {
            // std::cout << "contact! " << numContacts<< std::endl;
            const btCollisionObject* obA =  contactManifold->getBody0();
            const btCollisionObject* obB =  contactManifold->getBody1();

            btCollisionObject* ss = spaceship_data.spaceship->body;

            if (ss == obA || ss == obB){
            // std::cout << "contact! " << numContacts<< std::endl;
                return true;

            }
        }
    }
    return false;

}


int spaceship_scene( SDL_Window* window, scene_spaceship_data& spaceship_data,NYTimer * g_timer){

	spaceship_data.nextscene = spaceship;

	int nbmol = spaceship_data.mh->molecules.size();
    spaceship_data.elapsed = g_timer->getElapsedSeconds(true);


    // spaceship_data.spaceship->updateSpeed(spaceship_data.elapsed);
    spaceship_input(spaceship_data);

    spaceship_data.spaceship->applyForces();

    // SDL_WarpMouseInWindow(window,spaceship_data.ss_cam->screen_width/2,spaceship_data.ss_cam->screen_height/2);


    // simulation




    // if (spaceship_data.viewMode == thirdPerson)
    // {
        
    //     // glm::vec3 camShipVec = spaceship_data.spaceship->getCamPos() - spaceship_data.followcam->pos;

    //     // if (spaceship_data.spaceship->forceCam)
    //     // {
    //     //     // float distCamShip = glm::length(camShipVec);
    //     //     // if(glm::length(camShipVec) > 0.01)
    //     //     // {
    //     //     //     spaceship_data.followcam->pos += glm::normalize( camShipVec ) *std::min(distCamShip,0.25f);
    //     //     // }

    //     //      spaceship_data.fpsCam->t.getPos= spaceship_data.spaceship->getPos() - glm::normalize(spaceship_data.ss_cam->direction)*10.0f ;

    //     //     // else{
    //     //     //     spaceship_data.followcam->pos += glm::normalize( camShipVec ) *std::max(distCamShip,0.01);
    //     //     // }

    //     // }

    //     //delay to look at
    //     // spaceship_data.followcam->lookAtPoint(spaceship_data.spaceship->getCamPointToLookAT()); //+ glm::vec3(-1.5,1.0,0.0));

    //     // spaceship_data.spaceship->position.setRotMat(spaceship_data.fpsCam->getRotMat());
    //     // spaceship_data.spaceship->position.setRotMat(spaceship_data.ss_cam->getRotMat());

    // }
    // else if (spaceship_data.viewMode == firstPerson){

    //     // spaceship_data.spaceship->position.setRotMat(spaceship_data.fpsCam->t.getRotMat());
    //     // spaceship_data.fpsCam-setPos(spaceship_data.spaceship->getPos());
    //     // spaceship_data.fpsCam->Front =spaceship_data.spaceship->direction;
    //     // spaceship_data.followcam->pos = spaceship_data.spaceship->getPos();
    //     // glm::vec3 direction=glm::vec3(spaceship_data.spaceship->position*glm::vec4(spaceship_data.spaceship->direction,1)) ;
    //     // spaceship_data.followcam->lookAtPoint(spaceship_data.spaceship->getPos() + direction);
    // }





    if (spaceship_data.optim & (nbmol  >= 2 )){
        spaceship_data.mh->newOptimMC(spaceship_data.elapsed,spaceship_data.optim,spaceship_data.prog);

        // spaceship_data.mh->optimMC (spaceship_data.elapsed,spaceship_data.optim,spaceship_data.ener,spaceship_data.prog);
    }
    else{

        float updatePhy = glm::min(spaceship_data.elapsed,1.0f/60.0f); //Bullet fait de la merde si sous 60hz...
        spaceship_data.dynamicsWorld->stepSimulation(updatePhy,10);
        spaceship_data.pe->update(updatePhy);
        // spaceship_data.pe->pos = spaceship_data.spaceship->getPos();
        spaceship_data.pe->pos = spaceship_data.spaceship->getPos() + (-spaceship_data.spaceship->getDirection());
        spaceship_data.pe->dir = -spaceship_data.spaceship->getDirection();
        if (spaceship_data.spaceshipMoving){
            spaceship_data.pe->isEmitting(true);
        }
        else
        {
            spaceship_data.pe->isEmitting(false);
        }
    }

    //faut il stopper le damp quand l'utilisateur interagit avec la molecule?
    if ( nbmol  >= 1 ){
        spaceship_data.mh->dampMols(spaceship_data.elapsed,spaceship_data.damp); // probleme avec ajout mol
    }




    //spaceship and cam movement
    spaceship_data.ss_cam->setUpRef(spaceship_data.spaceship->getUp());

    if (spaceship_data.viewMode == thirdPerson){
        spaceship_data.ss_cam->setPosition( spaceship_data.spaceship->getCamPos());
        spaceship_data.ss_cam->setLookAt( spaceship_data.spaceship->getCamPointToLookAT());
    }
    if(spaceship_data.viewMode == firstPerson){
        spaceship_data.ss_cam->setPosition( spaceship_data.spaceship->getPos());
        spaceship_data.ss_cam->setLookAt( spaceship_data.spaceship->getPos() + (100.0f * spaceship_data.spaceship->getDirection()));
    
    }

    //colission
    if (isSpaceshipCollision(spaceship_data)){
        spaceship_data.gpad->rumble();
    }


    //rendering

    spaceship_render(spaceship_data);
    // UI-----------------------------------------------------------------------
    spaceship_UI(window,spaceship_data);
    ImGui::Render();


    SDL_GL_SwapWindow(window);




    //energy calculation
    spaceship_data.energyCalcTime += spaceship_data.elapsed;

    if( !spaceship_data.optim ){

        if (spaceship_data.energyCalcTime>0.15)
        {
            // mh.energyCalc();
            spaceship_data.mh->threadedEnergyCalc ();
            spaceship_data.energyCalcTime -= 0.15;
        } 
    }
    // fps calculation

    spaceship_data.deltaTime += spaceship_data.elapsed;

    spaceship_data.frames ++;
    if( spaceship_data.deltaTime>1.0){ //every second
        spaceship_data.frameRate = (double)spaceship_data.frames*0.5 +  spaceship_data.frameRate*0.5; //more stable
        spaceship_data.averageFrameTimeMilliseconds  = 1/(spaceship_data.frameRate==0?0.001:spaceship_data.frameRate);
        spaceship_data.fps = spaceship_data.frames;
        spaceship_data.frames = 0;
        spaceship_data.deltaTime -= 1;       
    }
	return spaceship_data.nextscene; //return scene value
}



#endif