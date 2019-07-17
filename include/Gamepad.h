#ifndef GAMEPAD_H
#define GAMEPAD_H

class Gamepad
{

	public:
    SDL_GameController *controller;
    SDL_Haptic *haptic;

    //the time for pressing a button. beyond this time a button is maintened
    float pressTime = 0.2f;

    //for each button
    //A
    bool APress = false;
    bool APressOld = false;
    bool ALongPress = false; // is the user pressing the button for a long time ?
    float APressTime = 0.0f;

    bool YPress = false;
    bool YPressOld = false;
    bool YLongPress = false; // is the user pressing the button for a long time ?
    float YPressTime = 0.0f;

    bool backPress = false;
    bool backPressOld =false;

    bool shoulderRightPress =false;
    bool shoulderLeftPress = false;


    bool dPadLeftPress = false;
	bool dPadLeftPressOld = false;

    bool dPadRightPress = false;
    bool dPadRightPressOld = false;

    int leftStickX = 0;
    int leftStickY = 0;
    glm::vec2 leftStick; 

    int rightStickX = 0;
    int rightStickY = 0;
	glm::vec2 rightStick;

    float rightTrigger = 0.0f;
    float leftTrigger = 0.0f;
    // for axis (stick and trigger) value go from 0 to 32.767, (centered to max) but hardware not exact
    //for stick, a value inside the deadzone is 0% (the rest pose of the stick)
    int deadzone1D = 4500;
    int deadzone2D = 5000;
    int maxvalue = 30000;// same reason


    //for trigger
    float filter1D(int stickCoordinate){
    	float percent = 0.0f;

    	int absvalue = glm::abs(stickCoordinate);
    	
    	//depending of the system the rest position of the trigger before first use is sometime set to 0.5 instead of zero, so we put it to zero.
    	if (absvalue > deadzone1D && absvalue != 16384){
    		
    		percent = static_cast<float> (absvalue - deadzone1D)/( maxvalue - deadzone1D);
    		if (stickCoordinate < 0) { percent *= -1.0f; }
    		percent = glm::clamp(percent, -1.0f, 1.0f);

    	}

    	return percent;
    }

    //for stick
    glm::vec2 filter2D(int stickX,int stickY){
    	glm::vec2 dir;

    	dir.x = static_cast<float>(stickX);
    	dir.y = static_cast<float>(stickY);

    	float length = glm::length(dir);


    	if (length < deadzone2D)
    	{
    		dir = glm::vec2(0);
    	}
    	else
    	{
    		float f = (length - deadzone2D)/ ( maxvalue - deadzone2D );
    		f= glm::clamp(f, 0.0f, 1.0f);
    		dir *= f / length;
    	}

    	return dir;
    }    


		Gamepad(){
			init();
		}

		~Gamepad(){
			clean();
		}

		void init()
		{
		    if (SDL_NumJoysticks()){
		        std::cout << "hello !" << std::endl;
		        controller = SDL_GameControllerOpen(0);
		        haptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(controller));
		        if(SDL_HapticRumbleInit(haptic) == 0){
		            std::cout << "Rumble active !" << std::endl;
		        }
		    }
		}

		void clean()
		{
			std::cout << "goodbye !" << std::endl;
			if (SDL_NumJoysticks()){

		        SDL_HapticClose(haptic);
		        SDL_GameControllerClose(controller);
    		}
		}

		void rumble(){
			SDL_HapticRumblePlay(haptic,0.99f,300);
		}

		void input(float elapsed)
		{

			APressOld = APress;
			YPressOld = YPress;
			backPressOld = backPress;

			dPadRightPressOld = dPadRightPress;
			dPadLeftPressOld = dPadLeftPress;

		    if( SDL_NumJoysticks() && SDL_GameControllerGetAttached(controller) ){
		    	YPress = SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_Y);
		        APress = SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_A);
		        backPress = SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_BACK);
		        dPadRightPress = SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		        dPadLeftPress= SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		        shoulderLeftPress= SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
				shoulderRightPress= SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

		        leftStickX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
		        leftStickY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
		        leftStick = filter2D(leftStickX,leftStickY);

		        rightStickX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
		        rightStickY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
		        rightStick = filter2D(rightStickX,rightStickY);

		        rightTrigger = filter1D(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
		        leftTrigger = filter1D(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT));


		    }

		    if (APress && ! APressOld){
		        // std::cout << "A  pressed !"  << std::endl;
		    }
		    else if (APress && APressOld){
		    	APressTime += elapsed;
		    	if (APressTime > pressTime) ALongPress = true;
		    }
		    else if ( !APress && APressOld){
		        // std::cout << "A  released !"  << std::endl;
		        APressTime = 0.0f;
		        ALongPress = false;

		    }

		    // if (ALongPress){
		    // 	// std::cout << "A!"  << std::endl;
		    // 	//force from 0 to 1, length in milliseconds
		    // 	// SDL_HapticRumblePlay(haptic,0.99f,300);
		    // }

		    // else if (APress && APressOld){
		    // 	APressTime += elapsed;
		    // 	if (APressTime > pressTime) ALongPress = true;
		    // }
		    // else if ( !APress && APressOld){
		    //     // std::cout << "A  released !"  << std::endl;
		    //     APressTime = 0.0f;
		    //     ALongPress = false;

		    // }

		    // if (ALongPress){
		    	// std::cout << "A!"  << std::endl;
		    	//force from 0 to 1, length in milliseconds
		    	// SDL_HapticRumblePlay(haptic,0.99f,300);
		    // }

		 //    glm::vec2 vleft = filter2D(leftStickX,leftStickY);

		 	//    // if (glm::length(vleft) > 0.0f)
			//     // std::cout << "left Stick " << vleft.x <<" "<< vleft.y << std::endl;

			// glm::vec2 vright = filter2D(rightStickX,rightStickY);
		    
		    // if (glm::length(vright) > 0.0f)
			    // std::cout << "right Stick " << vright.x <<" "<< vright.y << std::endl;

			// float rt = filter1D(rightTrigger);
			// if (rt > 0.0f)
			// 	std::cout << "right Trigger " << rt <<" "<< rightTrigger<< std::endl;

		}

};


#endif