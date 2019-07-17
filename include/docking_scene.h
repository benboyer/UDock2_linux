#ifndef DOCKING_SCENE_H
#define DOCKING_SCENE_H


#include "scene_data.h"
#include "enum_scene.h"

int docking_scene( SDL_Window* window, scene_docking_data& docking_data,NYTimer * g_timer){
	docking_data.nextscene = docking;

	int nbmol = docking_data.mh->molecules.size();
    docking_data.elapsed = g_timer->getElapsedSeconds(true);

    docking_input(docking_data);

    // simulation

    if (docking_data.optim & (nbmol  >= 2 )){
        docking_data.mh->newOptimMC(docking_data.elapsed,docking_data.optim,docking_data.prog);
        // docking_data.mh->optimMC (docking_data.elapsed,docking_data.optim,docking_data.ener,docking_data.prog);
    }
    else{
        float updatePhy = glm::min(docking_data.elapsed,1.0f/60.0f); //Bullet fait de la merde si sous 60hz...
        docking_data.dynamicsWorld->stepSimulation(updatePhy,10);
    }

    //faut il stopper le damp quand l'utilisateur interagit avec la molecule?
    if ( nbmol  >= 1 ){
        docking_data.mh->dampMols(docking_data.elapsed,docking_data.damp); // probleme avec ajout mol
    }

    //rendering
    docking_render(docking_data);
    // UI-----------------------------------------------------------------------
    docking_UI(window,docking_data);
    ImGui::Render();


    SDL_GL_SwapWindow(window);




    //energy calculation
    docking_data.energyCalcTime += docking_data.elapsed;

    if( !docking_data.optim ){

        if (docking_data.energyCalcTime>0.15)
        {
            // mh.energyCalc();
            docking_data.mh->threadedEnergyCalc ();
            docking_data.energyCalcTime -= 0.15;
        } 
    }
    // fps calculation

    docking_data.deltaTime += docking_data.elapsed;

    docking_data.frames ++;

    //every quarter second we send a ray to position the focus of the cam on the surface of the mol the user is looking at.
    if( docking_data.d_cam->fps && docking_data.deltaTime>0.05f){

        
        
        glm::vec3 hitPoint =  glm::vec3();

        glm::vec3 pos= docking_data.d_cam->Position ;

        glm::vec3 rayEnd= pos + (85.0f * docking_data.d_cam->Direction );

        btRigidBody* body = rayPickup(docking_data.mh->dynamicsWorld,pos, rayEnd ,hitPoint);
        //if we touch a mol
        bool rayPickupMol = false;
        if (body){
            int rsize = docking_data.mh->rigidBody.size();

            for (int i = 0; i<rsize; i++){
                if (body == docking_data.mh->rigidBody[i] ){
                    // std::cout<<"found in rigidBody !"<<std::endl;
                    rayPickupMol = true;
                }
            }
        }
        if (rayPickupMol )
        {
            // std::cout<<"focus adapt!"<<std::endl;

            glm::vec3 backward = hitPoint- (75.0f* docking_data.d_cam->Direction);
            

            glm::vec3 backHitPoint =  glm::vec3();

            btRigidBody* bodybackward = rayPickup(docking_data.mh->dynamicsWorld,pos, backward ,backHitPoint);

            //we check if the mol is inside by sending a ray back, if it hit the same mol then inside /!\ not perfect if torus for exemple
            
            if (body == bodybackward){
                docking_data.d_cam->LookAt = pos + (75.0f * docking_data.d_cam->Direction );
                // std::cout<<"in"<<std::endl;
            }else if(glm::length2(pos - hitPoint)>3){
                docking_data.d_cam->LookAt = hitPoint - docking_data.d_cam->Direction ;
                // std::cout<<"hit!"<<std::endl;
            }
        }
        
    }


    if( docking_data.deltaTime>1.0){ //every second
        docking_data.frameRate = (double)docking_data.frames*0.5 +  docking_data.frameRate*0.5; //more stable
        docking_data.averageFrameTimeMilliseconds  = 1/(docking_data.frameRate==0?0.001:docking_data.frameRate);
        docking_data.fps = docking_data.frames;
        docking_data.frames = 0;
        docking_data.deltaTime -= 1;       
    }
	return docking_data.nextscene; //return scene value
}

#endif