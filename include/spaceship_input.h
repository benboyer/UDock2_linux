#ifndef SPACESHIP_INPUT_H
#define SPACESHIP_INPUT_H




void spaceshipInput(spaceship_controller* spaceship, Gamepad* gpad, SDL_Event* windowEvent )
{

    // //forward
    // float rt =gpad->rightTrigger;
    // if (rt >0){
    //     spaceship->forward(rt);
    // }

    // //backward
    // float lt =gpad->leftTrigger;
    // if (lt >0){
    //     spaceship->backward(lt);
    // }

    //forward
    float rt =gpad->rightTrigger;
    if (rt >0){
        spaceship->addForce(spaceship->direction*spaceship->speed*rt);
        
         // std::cout << "right Trigger " << rt << std::endl;
    }

    //backward
    float lt =gpad->leftTrigger;
    if (lt >0){
        spaceship->addForce(-spaceship->direction*spaceship->speed*lt);
    }

    //rotation of camera
    glm::vec2 rl = gpad->rightStick ;
    if (rl.x != 0.0f && rl.y!=0.0f){
    	spaceship->setTorque(rl);
    }

    //rotation along direction


    // //lateral translation of the ship
    glm::vec3 ls = glm::vec3(gpad->leftStick,0.0f);
    //ugly as sin
    btVector3 f = spaceship->body->getWorldTransform().getBasis()*btVector3(0.0f,ls.y,0.0f);

    
    if (glm::length(ls) > 0){
       spaceship->addForce(glm::vec3(f.x(),f.y(),f.z()) * (spaceship->speed*0.8f));
    }

    f = spaceship->body->getWorldTransform().getBasis()*btVector3(-ls.x,0.0f,0.0f);

    
    if (glm::length(ls) > 0){
       spaceship->addForce(glm::vec3(f.x(),f.y(),f.z()) * (spaceship->speed*0.8f));
    }


    //rotation
    if (gpad-> shoulderLeftPress){
        spaceship->rotate=true;
        spaceship->angleRot = 1.0f;
    } 

    if (gpad-> shoulderRightPress){

        spaceship->rotate=true;
        spaceship->angleRot = -1.0f;
    }

    // //fire
    // if (spaceship_data.gpad->APress){
    //      spaceship_data.fire = true;
    //      spaceship_data.gpad->rumble();
    // } 

    // // on release
    // if (!spaceship_data.gpad->APress && spaceship_data.gpad->APressOld){
    //      spaceship_data.fire = false;
    // }


    // // Weapon change
    // if (spaceship_data.gpad->dPadRightPress && !spaceship_data.gpad->dPadRightPressOld){
    //     spaceship_data.currentWeapon = (spaceship_data.currentWeapon+1)%NB_WEAPON;
    // }
    // if (spaceship_data.gpad->dPadLeftPress && !spaceship_data.gpad->dPadLeftPressOld){
    //     //c++ doesnt give you "true" modulo but only remainder
    //     spaceship_data.currentWeapon = ((spaceship_data.currentWeapon -1)%NB_WEAPON + NB_WEAPON)%NB_WEAPON;
    // }

    // //rotation of camera
    // glm::vec2 rl = gpad->rightStick ;
    // if (glm::length(rl) > 0){

    // 	setTorque()
    //     // rl = rl * 50.0f;

    //     // if (invertedAxis==false){
    //     //     spaceship.(-rl.x,-rl.y);
    //     // }else{
    //     //     spaceship.MouseRotation(-rl.x,rl.y);
    //     // }
     
    // }


     switch (windowEvent->type)
    {   

        case SDL_KEYDOWN:
        {
            switch (windowEvent->key.keysym.sym)
            {


            // case SDLK_z://beware QWERTY
            //     {
            //         spaceship->forward(1.0f);
            //         break;
            //     }
            }
            break;
        }
        break;
    }
}


#endif