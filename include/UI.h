#ifndef UI_H
#define UI_H

//imgui Headers
#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

#include <ctime>
#include <iostream>

#include "MolecularHandler.h"
#include "Camera.h"


#include "scene_data.h"

unsigned int getTexColorBuffer(int window_width,int window_height){
    // generate texture
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,  window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texColorBuffer;
}

unsigned int getTexDepthBuffer(int window_width,int window_height){
    //generate frame buffer for depth
    unsigned int texDepthBuffer;
    glGenTextures(1, &texDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, texDepthBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    return texDepthBuffer;
}
unsigned int getScreenFramebuffer(unsigned int texColorBuffer,unsigned int texDepthBuffer){

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); 
    
    // attach texture to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepthBuffer, 0);

    //check if the frame buffer is complete (correct)
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    //unbind it to avoid mistake
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return framebuffer;
}

void scorePlot( float energy, int screen_width,int screen_height){

        		//energy bar
        int width = screen_width;
    	int height = screen_height;
    	int plotWidth = 500;
    	int plotHeight = 100;

        ImGui::SetNextWindowPos(ImVec2(width/2 - plotWidth/2+10 ,height/16),true);
        ImGui::SetNextWindowSize(ImVec2(plotWidth+20,plotHeight +30),true);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0,0,0,0));//for transparency
        ImGui::Begin("Optimisation",(bool*)false,ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);

        static bool animate = true;
        static float values[90] = { 0 };
        static int values_offset = 0;
        static float refresh_time = 0.0f;
        if (!animate || refresh_time == 0.0f)
            refresh_time = ImGui::GetTime();
        while (refresh_time < ImGui::GetTime()) // fixed 60 hz rate f
        {
            values[values_offset] = energy;
            values_offset = (values_offset+1) % IM_ARRAYSIZE(values);
            refresh_time += 1.0f/60.0f;
        }

        static float maxEner = 0.0;
        maxEner = std::min (energy,maxEner);
        

		ImGui::Text("Score : %5.2lf Min : %5.2lf",energy,maxEner);
        ImGui::PlotLines("", values, IM_ARRAYSIZE(values), values_offset,  "", maxEner -1, 1.0f, ImVec2(plotWidth,plotHeight));
        ImGui::End();
        ImGui::PopStyleColor(); //for transparency
}



void changeResolution (SDL_Window* window,scene_docking_data& docking_data, int w, int h){
  // std::cout <<"screen_resolution "<< item << std::endl;

                    int old_h,old_w;
                    SDL_GetWindowSize(window,&old_h,&old_w);
                    if (h!=old_h || w != old_w){

                        // SDL_DestroyWindow(window);
                        // window =  SDL_CreateWindow("Minimal Molecule Viewer", 100, 100, h, w, SDL_WINDOW_OPENGL );//recreate windows with new dimension
                        // SDL_GL_MakeCurrent(window,docking_data.context);
                        SDL_SetWindowSize(window,w, h);

                        // SDL_GetWindowSize(window,&h, &w);
                        glViewport(0,0,w,h);
                        docking_data.d_cam->screen_width = w;//camera for the docking scene
                        docking_data.d_cam->screen_height = h;
                        docking_data.ss_cam->screen_width = w;//camera for the spaceship scene
                        docking_data.ss_cam->screen_height = h;
                        //suposedly prevent bug if relative mouse mode activated, shouldn't be a problem at the moment but I'm sure it come up eventually(always do)
                        if (SDL_GetRelativeMouseMode())
                        {
                            SDL_SetRelativeMouseMode(SDL_FALSE);
                            SDL_SetRelativeMouseMode(SDL_TRUE);
                        }
                        // glDeleteBuffers(1,&docking_data.framebuffer);
                        glDeleteTextures(1,&docking_data.texColorBuffer);
                        glDeleteTextures(1,&docking_data.texDepthBuffer);
                        glDeleteFramebuffers(1,&docking_data.framebuffer);
                        // glDeleteFramebuffers(1,&docking_data.texColorBuffer);
                        // glDeleteFramebuffers(1,&docking_data.texDepthBuffer);
                        // unsigned int tcbuff =getTexColorBuffer(w,h);
                        // docking_data.texColorBuffer= &tcbuff;
                        // unsigned int tdbuff= getTexDepthBuffer(w,h);
                        // docking_data.texDepthBuffer= &tdbuff;
                        // unsigned int scFbuff = getScreenFramebuffer(tcbuff,tdbuff);
                        // docking_data.framebuffer = &scFbuff;

                        docking_data.texColorBuffer=getTexColorBuffer(w,h);
                        docking_data.texDepthBuffer=getTexDepthBuffer(w,h);
                        docking_data.framebuffer = getScreenFramebuffer(docking_data.texColorBuffer,docking_data.texDepthBuffer);
                        docking_data.resizeWindows = true;
                      }
}

void optionWindow (SDL_Window* window,scene_docking_data& docking_data,bool* open){
			int width = docking_data.d_cam->screen_width;
    		int height = docking_data.d_cam->screen_height;
            ImGui::SetNextWindowPos(ImVec2(width/2 - width/4 ,height/2-height/4),true);
            ImGui::SetNextWindowSize(ImVec2(width/2,height/2),true);
            if(!ImGui::Begin("Gaphical Option",open,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove))
            {
		        ImGui::End();
		    }else{
                static int item = 2;
                ImGui::Combo("screen resolution", &item, " 1024x768\0 1280x960\0 1920x1080(hd 1080)\0 2048x1080(2K)\0 4096x2160(4k)\0 3840x2160(UHD-1)\0\0");
            // std::cout <<"screen_resolution "<< item << std::endl;
                if(item == 0){changeResolution (window,docking_data,1024,768);}
                if(item == 1){changeResolution (window,docking_data,1280,960);}
                if(item == 2){changeResolution (window,docking_data,1920,1080);}
                if(item == 3){changeResolution (window,docking_data,2048,1080);}
                if(item == 4){changeResolution (window,docking_data,4096,2160);}
                if(item == 5){changeResolution (window,docking_data,3840,2160);}
         
                ImGui::Checkbox("skybox", docking_data.showSkybox);//already a pointer
                ImGui::ColorEdit3("background color", (float*)docking_data.bgColor);
                ImGui::Checkbox("invert Y axis", docking_data.invertedAxis);//already a pointer
                ImGui::SliderFloat("mouse sensitivity (default 1)",docking_data.mouseSensitivity,0.001f,3.0f,"ratio = %.3f");
                
                // bool fullscreen;
                ImGui::Checkbox("fullscreen", &docking_data.fullscreen);
                if (docking_data.fullscreen){
                    SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
                    SDL_DisplayMode dm ;
                    SDL_GetDesktopDisplayMode(0, &dm);
                    std::cout << dm.w << " " << dm.h <<std::endl;
                    changeResolution (window,docking_data,dm.w,dm.h);
                }
                else{
                    SDL_SetWindowFullscreen(window,0);
                }

	            ImGui::End();
        }
}


void docking_UI( SDL_Window* window,scene_docking_data& docking_data ){
	// UI-----------------------------------------------------------------------
        ImGui_ImplSdlGL3_NewFrame(window); 
        ImGui::PushFont(docking_data.font);
        ImGui::Begin("my Window",(bool*)false,ImGuiWindowFlags_MenuBar |ImGuiWindowFlags_AlwaysAutoResize);  
		
 

        //menu
        static bool showOption = false;

        if (showOption)optionWindow (window,docking_data,&showOption);


		 


        if(ImGui::BeginMenuBar())
        {
        
            if(ImGui::BeginMenu("Menu")){
                
                ImGui::MenuItem("Option",NULL,&showOption);
               
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        

        ImGui::Text("avg Frame in ms : %f",docking_data.averageFrameTimeMilliseconds);
        ImGui::Text("FPS : %i",docking_data.fps);
        ImGui::Text("Score : %5.3lf",docking_data.mh->energy);





        if (ImGui::Button("Save"))
        {
     //        // writePDB(Molecule mol, const std::string filename, char mode)

	        std::time_t t = std::time(0);   // get time now
	    	std::tm* now = std::localtime(&t);

	    	std::string filename = "../savefile/Molecular_Complex-";
	    	filename += std::to_string(now->tm_year + 1900) + '-' + std::to_string(now->tm_mon + 1)+'-' +std::to_string(now->tm_mday ) ; //add date
			filename +=  '-' + std::to_string(now->tm_hour)+":"+std::to_string(now->tm_min)+":"+std::to_string(now->tm_sec);// add time
			filename += ".pdb";
	    	std::cout << filename << std::endl;
	 
	 		
			int nbmol = docking_data.mh->molecules.size();

            int nbAtom=1;
			std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			FILE* file = fopen(filename.c_str(), "a");
			fprintf(file, "REMARK   1  score %f \n",docking_data.mh->energy);
			for (int i = 0; i < nbmol; ++i)
			{
				Molecule mol = docking_data.mh->molecules[i];
				glm::mat4 transformation =  docking_data.mh->getModelMatrix(i);


			    for (glm::uint j = 0; j < mol.size(); j++)
			    {

			        const char  chainID = alphabet.at(i%26);

			        Atom at = mol.atoms[j];
			        const char* atomname = at.atomType.c_str();
			        const char* residName = at.residueType.c_str();
			        int residnumber = at.residueId;
			        // chainID = at.chain.c_str();

			        int atomnumber = at.atomId;
                    atomnumber = nbAtom;

                    nbAtom++;

			        glm::vec3 coord = glm::vec3(transformation * glm::vec4(at.pos,1.0));
			        double x = coord.x;
			        double y = coord.y;
			        double z = coord.z;



			        fprintf(file, "ATOM  %5d %-5s%3s %1c%4d    %8.3f%8.3f%8.3f", atomnumber, atomname, residName, chainID, residnumber, x, y, z);
			        fprintf(file, "\n");
			    }
                fprintf(file, "TER");
                fprintf(file, "\n");
			}
		    fclose(file);
		}

        std::string s;
        if (docking_data.freeze){
            s = "unfreeze";
        }
        else{
            s = "freeze";
        }
        if (ImGui::Button(s.c_str()))
        {
            docking_data.freeze = !docking_data.freeze;
            int nbmol = docking_data.mh->molecules.size();
            // std::cout<<"hello "<<s.c_str()<<" "<<freeze<<std::endl;
            //we free the mol to move again
            for (int i = 0; i < nbmol; i++)
            {  
                setFreeze(docking_data.mh->rigidBody[i],docking_data.freeze);
                stopMol(docking_data.mh->rigidBody[i]);
            }
        }

        if (ImGui::Button("clear Constraint"))
        {
            docking_data.cm->constraints.clear();
        }

        if (ImGui::Button("reset"))
        {
        	//erase constraint
            docking_data.cm->constraints.clear();

            // repostion molecules
            for (unsigned int i =0;i< docking_data.mh->molecules.size();i++){
            	btTransform startTransform;
        		startTransform.setIdentity();

            	glm::vec3 ip =  docking_data.mh->getInitialPos(i);
         		startTransform.setOrigin(btVector3(ip.x,ip.y,ip.z));

	            docking_data.mh->rigidBody[i]->setActivationState(ACTIVE_TAG);

                btDefaultMotionState* myMotionState = (btDefaultMotionState*)docking_data.mh->rigidBody[i]->getMotionState();

                myMotionState->setWorldTransform(startTransform);
                docking_data.mh->rigidBody[i]->setWorldTransform(startTransform);
                docking_data.mh->rigidBody[i]->setMotionState(myMotionState);
            }

            //position of spaceship
            btTransform t;
            t.setIdentity();
            t.setOrigin(btVector3(0.0f,0.0f,150.0f));

            docking_data.spaceship->body->setActivationState(ACTIVE_TAG);
            btDefaultMotionState* myMotionState = (btDefaultMotionState*)docking_data.spaceship->body->getMotionState();
            
            myMotionState->setWorldTransform(t);
            docking_data.spaceship->body->setWorldTransform(t);
            docking_data.spaceship->body->setMotionState(myMotionState);
            
            docking_data.d_cam->setPosition( docking_data.mh->getCenterofScene()+glm::vec3(0.0,0.0,150.0));
            docking_data.d_cam->setLookAt(glm::vec3(0.0,0.0,0.0));
            docking_data.ss_cam->setPosition( docking_data.mh->getCenterofScene()+glm::vec3(0.0,0.0,150.0));
            docking_data.ss_cam->setLookAt(glm::vec3(0.0,0.0,0.0));

        }

        if (ImGui::Button("clear"))
        {
            docking_data.cm->constraints.clear();
            docking_data.mh->clear();
            std::cout << "clear" << std::endl;
        }


        int nbmol = docking_data.mh->molecules.size();
        static int e = 0;
        if (nbmol>=2)
        {
            docking_data.mh->active[e] = false;
            for (int i = 0; i < nbmol; i++)
            {
                ImGui::RadioButton(("mol" + std::to_string(i+1)).c_str(), &e, i);
            }
            docking_data.mh->active[e] = true;
        }
        ImGui::End();

        //loading bar if there an optim occuring
        if (docking_data.optim & (docking_data.mh->molecules.size()>=2)){



        	int width = docking_data.d_cam->screen_width;
        	int height = docking_data.d_cam->screen_height;
        	
        	//background image
        	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0,0,0,0));//for transparency

        	ImGui::SetNextWindowPos(ImVec2(width/2 - 256,height/2- 256),true);
            ImGui::SetNextWindowSize(ImVec2(512,512+20),true);
        	ImGui::Begin("OptimBG",(bool*)false,ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
			ImGui::Image(docking_data.optimTexture, ImVec2(512, 512), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,0));
			ImGui::End();
        	

			//progress bar
            ImGui::SetNextWindowPos(ImVec2(width/2 - width*0.15,height/2-15),true);
            ImGui::SetNextWindowSize(ImVec2(width*0.3 +10,200),true);
            ImGui::Begin("Optimisation",(bool*)false,ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
                        // Typically we would use ImVec2(-1.0f,0.0f) to use all available width, or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
            // ImGui::Text("Optimisation");
            ImGui::ProgressBar(docking_data.prog/100.0, ImVec2(-1.0f,0.0f));
            // ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            // ImGui::Text("Progress Bar");
            // ImGui::Text("Optimisation: %4.1f %%",docking_data.prog);
            ImGui::End();
            //for transparency
            ImGui::PopStyleColor(); 
        }
           
     //    		//energy bar
        scorePlot( docking_data.mh->energy,docking_data.d_cam->screen_width,docking_data.d_cam->screen_height);
        ImGui::PopFont();

}


void spaceship_UI( SDL_Window* window,scene_spaceship_data& spaceship_data ){
	// UI-----------------------------------------------------------------------
        ImGui_ImplSdlGL3_NewFrame(window); 
		scorePlot( spaceship_data.mh->energy,spaceship_data.ss_cam->screen_width,spaceship_data.ss_cam->screen_height);

        int width = spaceship_data.ss_cam->screen_width;
        int height = spaceship_data.ss_cam->screen_height;
        
        if (spaceship_data.viewMode == firstPerson){
            //crosshair image
            ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0,0,0,0));//for transparency

            ImGui::SetNextWindowPos(ImVec2(width/2 -35 ,height/2 -35),true);
            ImGui::SetNextWindowSize(ImVec2(50+20,50+20),true);
            ImGui::Begin("crosshairBG",(bool*)false,ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
            ImGui::Image(spaceship_data.crosshairTexture, ImVec2(50, 50), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,0));
            ImGui::End();
            //for transparency
            ImGui::PopStyleColor(); 
        }
        //weapon image
        ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0,0,0,0));//for transparency

        ImGui::SetNextWindowPos(ImVec2(width/10 -35 , width/14 -35),true);
        ImGui::SetNextWindowSize(ImVec2(50+20,50+20),true);

        ImGui::Begin("weaponBG",(bool*)false,ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
        
        ImGui::Image(spaceship_data.weaponTextures[spaceship_data.currentWeapon], ImVec2(50, 50), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,0));

        ImGui::End();

        //for transparency
        ImGui::PopStyleColor(); 




}




#endif