// gl3w Headers
#include <gl3w.h>

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

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// C++ Headers
#include <iostream>
#include <math.h> 
#include <time.h> 

//internal header
#include "Atom.h"
#include "Molecule.h"
#include "MolecularHandler.h"
#include "pdb_io.h"
#include "mol2_io.h"
#include "Camera.h"
#include "billboardshader.h"
#include "timer.h"

int main(int argc, char* argv[])
{
    Molecule mol;
    MolecularHandler mh = MolecularHandler(false,false);
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            mol = readPDB(argv[i]);
            mh.addMol(mol);
        }
    }

    //test:
    // mol = readMOL2("1.mol2");
    // mh.addMol(mol);

    //heigth and size-----------------

    float window_width = 1280.0f;
    float window_height = 960.0f;


    //SDL INIT-----------------------------------------------------------------
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //control vsync
    SDL_GL_SetSwapInterval(1);

    SDL_Window* window = SDL_CreateWindow("Minimal Molecule Viewer", 100, 100, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    //initialize gl3w----------------------------------------------------------
    if (gl3wInit())
    {
        fprintf(stderr, "failed to initialize OpenGL\n");
        return -1;
    }

    //Shader work--------------------------------------------------------------
    
    //the data

    //the vertex Array Object store the link between attribute and the VBO (remember in/out of shader)
    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    //the vertext buffer object (vbo) store the data in openGL
    GLuint vbo;
    glGenBuffers(1, &vbo); // Generate 1 buffer

    // bind the buffer (make it the actif buffer and define it's type (here an array buffer))
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    std::vector<float> vertices;
    vertices = mh.getVertices();
    int nbvertices = vertices.size();
    //fill the buffer with the data
    glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);


    //load image 
    int w;
    int h;
    int comp;
    // unsigned char* image = stbi_load("../textures/sphericalnormal3.png", &w, &h, &comp, 0);
    // unsigned char* image = stbi_load("../textures/sphere2.png", &w, &h, &comp, 0);
    unsigned char* image = stbi_load("../textures/sphere-goodsell2.png", &w, &h, &comp, 0);

    if (image == nullptr)
        throw (std::string("Failed to load texture"));

    GLuint sphereTexture;
    glGenTextures(1, &sphereTexture);

    glBindTexture(GL_TEXTURE_2D, sphereTexture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    if (comp == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        std::cout << "rgb" << std::endl;
    }
    else if (comp == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        std::cout << "rgba" << std::endl;
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    stbi_image_free(image);


    // //the Shaders--------------------------------------------------------------
    Shader bbshader = billboardShader();
    bbshader.use();

    //camera initialisation
    Camera cam = Camera();
    cam.setWindowSize(int(window_width), int( window_height));

    //model matrix (used to move the object)
    glm::mat4 model = glm::mat4(1.0);

    //view matrix, to move the camera
    glm::mat4 view = cam.lookAtMol(mol);

    // projection matrix, to decide camera parameters (fov, screen ratio and cliping distance)
    glm::mat4 proj = cam.getProj();



    bbshader.setMat4f("view", view);
    bbshader.setMat4f("proj", proj);
    bbshader.setMat4f("model", model);
    bbshader.setVec2f("windows_size", window_width, window_height);


    //OPTION-------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST); // find a place to put options 


    //fps stuff
    NYTimer * g_timer = new NYTimer();
    g_timer->start();
    float deltaTime;
    unsigned int frames = 0;
    double  frameRate = 30;
    double  averageFrameTimeMilliseconds = 33.333;

    //the event loop-----------------------------------------------------------
    SDL_Event windowEvent;
    char* dropped_filedir;

    bool running = true;
    while (running == true)
    {
        float elapsed = g_timer->getElapsedSeconds(true);
        //event handling
        while (SDL_PollEvent(&windowEvent))

        {

            switch (windowEvent.type)

            {

            case SDL_WINDOWEVENT:
            {
                switch (windowEvent.window.event)
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {

                        cam.setWindowSize(windowEvent.window.data1, windowEvent.window.data2);
                        glViewport(0, 0, windowEvent.window.data1, windowEvent.window.data2);
                        break;
                    }
                }
                break;
            }

                // you can exit the program by clicking the cross button or pressing escape
            case SDL_QUIT: running = false;
                break;


                //mouse wheel control zoom
            case SDL_MOUSEWHEEL: cam.traveling( windowEvent.wheel.y*2)
                ;
                break;
                //you can rotate with the left click
            case SDL_MOUSEMOTION:
            {
                if (windowEvent.motion.state == SDL_BUTTON_LMASK)
                {
                    cam.arcBallRotation(true,windowEvent.motion.x, windowEvent.motion.y);
                }
                else
                {
                    cam.arcBallRotation(false,windowEvent.motion.x, windowEvent.motion.y);
                }
                break;
            }
                //user drag and drop a file
            case SDL_DROPFILE:
            {
                dropped_filedir = windowEvent.drop.file;
                mol = readPDB(dropped_filedir);

                //center camera on new mol
                view = cam.lookAtMol(mol);
                //use molecularHandler to manage new data
                mh.addMol(mol);
                vertices = mh.getVertices();
                nbvertices = vertices.size();
                //fill the buffer with the new data
                glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);
                // Free dropped_filedir memory
                SDL_free(dropped_filedir);
                break;
            }
                // a key is pressed
            case SDL_KEYDOWN:
            {
                switch (windowEvent.key.keysym.sym)
                {
                case SDLK_RIGHT: cam.rotateAround(glm::radians(5.0f));
                    break;
                case SDLK_LEFT: cam.rotateAround(glm::radians(-5.0f));
                    break;

                }
                break;
            }
                // a key is released
            case SDL_KEYUP:
            {
                switch (windowEvent.key.keysym.sym)
                {
                case SDLK_SPACE:
                {

                    view = cam.lookAtMol(mh.getNextMol());
                    std::cout << mh.selectedMol << std::endl;
                    break;
                }
                case SDLK_ESCAPE: running = false;
                    break;
                }
                break;
            }
            }

        }
        // Clear the screen to black
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(0.3f, 0.7f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Change camera
        bbshader.setMat4f("view", cam.getView());
        bbshader.setMat4f("proj", cam.getProj());

        // draw command
        glBindTexture(GL_TEXTURE_2D, sphereTexture);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, nbvertices/3);
        glBindVertexArray(0);


        SDL_GL_SwapWindow(window);

        //fps calculation

        deltaTime += elapsed;
        frames ++;
        //if you really want FPS
        if( deltaTime>1.0){ //every second
            frameRate = (double)frames*0.5 +  frameRate*0.5; //more stable
            averageFrameTimeMilliseconds  = 1/(frameRate==0?0.001:frameRate);
            std::cout<<"CPU time was:"<<averageFrameTimeMilliseconds<<" fps :"<< frames<<std::endl;
            frames = 0;
            deltaTime -= 1;       
        }


    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();


    return 0;

}