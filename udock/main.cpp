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

//imgui Headers
#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"


// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//bullet header
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

//image library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//model(obj) loading library
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

/*//CPPCartoon
#include "cpdb/cpdb.h"
#include "cartoon.h"
#include "main.h"*/


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
// #include "FPScamera.h"
#include "Energy.h"
#include "billboardshader.h"
#include "marching_cubes.h"
#include "surfaces.h"
#include "timer.h"
#include "pickupRay.h"
#include "motion.h"
#include "render_utils.h"
#include "ConstraintManager.h"
#include "UI.h"
#include "input.h"
#include "render.h"
#include "scene_data.h"
#include "docking_scene.h"
#include "spaceship_scene.h"
#include "enum_scene.h"
#include "spaceship_controller.h"
#include "Gamepad.h"
#include "particles_emitter.h"


// std::vector<float> loadOBJ(std::string inputfile){

    
//     tinyobj::attrib_t attrib;
//     std::vector<tinyobj::shape_t> shapes;
//     std::vector<tinyobj::material_t> materials;
  
//     std::string err;
//     bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(),"../models/");
      
//     if (!err.empty()) { // `err` may contain warning message.
//       std::cerr << err << std::endl;
//     }

//     if (!ret) {
//       exit(1);
//     }
//     // std::cout<<attrib.vertices.size()/3<<std::endl;
//     // std::cout<<attrib.normals.size()/3<<std::endl;
//     // std::cout<<attrib.texcoords.size()/2<<std::endl;
//     // std::cout<<materials.size()<<std::endl;

//     std::vector<float> vertsurf;


//     // Loop over shapes
//     for (size_t s = 0; s < shapes.size(); s++) {
//         // Loop over faces(polygon)
//         size_t index_offset = 0;
//         for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
//             unsigned int fv = shapes[s].mesh.num_face_vertices[f];

//             int materialID =shapes[s].mesh.material_ids[f];
//             // Loop over vertices in the face.
//             for (size_t v = 0; v < fv; v++) {
//                 // access to vertex
//                 tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
//                 tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
//                 tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
//                 tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
//                 vertsurf.push_back(vx);
//                 vertsurf.push_back(vy);
//                 vertsurf.push_back(vz);
//                 //normal
//                 tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
//                 tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
//                 tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
//                 vertsurf.push_back(nx);
//                 vertsurf.push_back(ny);
//                 vertsurf.push_back(nz);

//                 vertsurf.push_back(materials[ materialID].diffuse[0]);
//                 vertsurf.push_back(materials[ materialID].diffuse[1]);
//                 vertsurf.push_back(materials[ materialID].diffuse[2]);
           
//             }
//             index_offset += fv;
//         }
//     }
//     return vertsurf;
// }











GLuint loadImage (std::string filename){

    //load image 
    int w;
    int h;
    int comp;
    unsigned char* image = stbi_load(filename.c_str(), &w, &h, &comp, 0);

    if (image == nullptr)
        throw (std::string("Failed to load texture"));

    std::cout << "image size "<<w<<" "<<h << std::endl;
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D,texture);

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
    return texture;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &
        nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] <<
            std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}



unsigned int initScreenQuadRendering(float* quadVertices){

    // float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    //     // positions   // texCoords
    //     -1.0f,  1.0f,  0.0f, 1.0f,
    //     -1.0f, -1.0f,  0.0f, 0.0f,
    //      1.0f, -1.0f,  1.0f, 0.0f,

    //     -1.0f,  1.0f,  0.0f, 1.0f,
    //      1.0f, -1.0f,  1.0f, 0.0f,
    //      1.0f,  1.0f,  1.0f, 1.0f
    // };

    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    return quadVAO;
}


int main(int argc, char* argv[])
{
    
    //object that will be use trough all the scene
    btDiscreteDynamicsWorld * dynamicsWorld = initBullet();
    MolecularHandler mh = MolecularHandler(dynamicsWorld);

    //scene init
    scene_docking_data docking_data;
    scene_spaceship_data spaceship_data;
    docking_data.mh = &mh;
    docking_data.dynamicsWorld = dynamicsWorld;
    spaceship_data.mh = &mh;
    spaceship_data.dynamicsWorld = dynamicsWorld;



    Molecule mol;
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            mol = readMOL2(argv[i]);
            mh.addMol(mol);
        }
    }

    //heigth and size-----------------

    // float window_width = 1280.0f; //4:3
    // float window_height = 960.0f;

    float window_width = 1920.0f; //hd 1080 (16:9)
    float window_height = 1080.0f;

    // float window_width = 2048.0f; //2k (17:9)
    // float window_height = 1080.0f;

    // float window_width = 4096.0f; //4k (17:9)
    // float window_height = 2160.0f;

    // float window_width = 3840.0f; //UHD-1 (17:9)
    // float window_height = 2160.0f;
    //SDL INIT-----------------------------------------------------------------
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER  | SDL_INIT_HAPTIC);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //control vsync
    SDL_GL_SetSwapInterval(0);

    SDL_Window* window = SDL_CreateWindow("Minimal Molecule Viewer", 100, 100, window_width, window_height, SDL_WINDOW_OPENGL );
    SDL_GLContext context = SDL_GL_CreateContext(window);


    docking_data.window = window;
    docking_data.context = &context;
    //initialize gl3w----------------------------------------------------------
    if (gl3wInit())
    {
        fprintf(stderr, "failed to initialize OpenGL\n");
        return -1;
    }

     // SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);


    // Setup ImGui binding
    ImGui_ImplSdlGL3_Init(window);

    // Load Fonts
    ImGuiIO& io = ImGui::GetIO();
    // ImFont* font2 = io.Fonts->AddFontDefault();
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("../fonts/consolas.ttf", 16.0f);

    //initialize gamepad--------------------------------------------------------
    Gamepad gpad = Gamepad();
    spaceship_data.gpad = &gpad;
    docking_data.gpad = &gpad;
    //skybox-------------------------------------------------------------------


            std::vector<std::string> faces
    {
        "../skybox/blue_nebulla_right1.png",
        "../skybox/blue_nebulla_left2.png",
        "../skybox/blue_nebulla_top3.png",
        "../skybox/blue_nebulla_bottom4.png",
        "../skybox/blue_nebulla_front5.png",
        "../skybox/blue_nebulla_back6.png"
    };
      
    //         std::vector<std::string> faces
    // {
    //     "../skybox/line_right1.png",
    //     "../skybox/line_left2.png",
    //     "../skybox/line_top3.png",
    //     "../skybox/line_bottom4.png",
    //     "../skybox/line_front5.png",
    //     "../skybox/line_back6.png"
    // };
    //             std::vector<std::string> faces
    // {
    //     "../skybox/miramar_lf.tga",
    //     "../skybox/miramar_rt.tga",
        
        
        
    //     "../skybox/miramar_dn.tga",
    //     "../skybox/miramar_up.tga",

    //     "../skybox/miramar_bk.tga",
    //     "../skybox/miramar_ft.tga"
        
    // };
    unsigned int cubemapTexture = loadCubemap(faces);



    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };



    // skybox vao
    unsigned int VAO_SKY;
    glGenVertexArrays(1, &VAO_SKY);
    glBindVertexArray(VAO_SKY);
    unsigned int VBO_SKY;
    glGenBuffers(1, &VBO_SKY);
    // bind the buffer (make it the actif buffer and define it's type (here an array buffer))
    glBindBuffer(GL_ARRAY_BUFFER, VBO_SKY);

    glBufferData(GL_ARRAY_BUFFER, sizeof( skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    //how to understand the data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    









    // loading sphere model for constraint

    std::vector<float> sphereVertices =loadOBJ("../models/sphere_centered.obj");
   
    //model vao
    unsigned int VAO_SPHERE, VBO_SPHERE;
    glGenVertexArrays(1, &VAO_SPHERE);
    glGenBuffers(1, &VBO_SPHERE);
    glBindVertexArray(VAO_SPHERE);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_SPHERE);


    int nbsphereVertices = sphereVertices.size();

    // std::cout<<"vertsurf size : "<<shapes.size()<<std::endl;
    //fill the buffer with the data
    glBufferData(GL_ARRAY_BUFFER, nbsphereVertices* sizeof (float), sphereVertices.data(), GL_STATIC_DRAW);

    //how to understand the data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);



   //loading mesh model (obj)-------------------------------------------------

    std::vector<float> spaceshipVertices =loadOBJ("../models/tf_centered.obj");
        // std::vector<float> spaceshipVertices =loadOBJ("../models/midShip4_centered.obj");

    //model vao
    unsigned int VAO_SPACESHIP, VBO_SPACESHIP;
    glGenVertexArrays(1, &VAO_SPACESHIP);
    glGenBuffers(1, &VBO_SPACESHIP);
    glBindVertexArray(VAO_SPACESHIP);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_SPACESHIP);


    int nbspaceshipVertices = spaceshipVertices.size();

    // std::cout<<"vertsurf size : "<<shapes.size()<<std::endl;
    //fill the buffer with the data
    glBufferData(GL_ARRAY_BUFFER, nbspaceshipVertices* sizeof (float), spaceshipVertices.data(), GL_STATIC_DRAW);

    //how to understand the data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    //physic model of the ship


    btConvexHullShape*  spaceshipShape = new btConvexHullShape();
    for (int i = 0; i < nbspaceshipVertices; i+=9)
    {
        btVector3 btv = btVector3( spaceshipVertices[i],  spaceshipVertices[i+1],  spaceshipVertices[i+2]);
        spaceshipShape->addPoint(btv);
    }


    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar    mass(1.f);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        spaceshipShape->calculateLocalInertia(mass,localInertia);

    //ship physics
    startTransform.setOrigin(btVector3(0,0,150));

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,spaceshipShape,localInertia);
    btRigidBody* shipBody = new btRigidBody(rbInfo);

    spaceship_data.dynamicsWorld->addRigidBody(shipBody);




    std::vector<float> flameVertices =loadOBJ("../models/flame_centered.obj");
    //model vao
    unsigned int VAO_FLAME, VBO_FLAME;
    glGenVertexArrays(1, &VAO_FLAME);
    glGenBuffers(1, &VBO_FLAME);
    glBindVertexArray(VAO_FLAME);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_FLAME);


    int nbflameVertices = flameVertices.size();

    // std::cout<<"vertsurf size : "<<shapes.size()<<std::endl;
    //fill the buffer with the data
    glBufferData(GL_ARRAY_BUFFER, nbflameVertices* sizeof (float), flameVertices.data(), GL_STATIC_DRAW);

    //how to understand the data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);


















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


    //load images 
    docking_data.optimTexture = (ImTextureID)(size_t)loadImage ("../textures/optim.png");// this weird cast is two fold. first is to convert to the ui type for image, actually a void*. the second part is to make sure the memory size is correct and avoid a warning. 
    spaceship_data.crosshairTexture = (ImTextureID)(size_t)loadImage ("../textures/crosshair.png");

    ImTextureID p [NB_WEAPON];
    p[0] = (ImTextureID)(size_t)loadImage ("../textures/icon1_b.png");
    p[1] = (ImTextureID)(size_t)loadImage ("../textures/icon2_b.png");
    p[2] = (ImTextureID)(size_t)loadImage ("../textures/icon3_b.png");
    
    spaceship_data.weaponTextures = p ;


    //the Shaders--------------------------------------------------------------
    Shader bbshader = billboardShader();
    Shader surfaceShader = basicShader("../shaders/mesh_normal_color.vertex.glsl","../shaders/mesh_normal_color.fragment.glsl");
    Shader spaceshipShader =  basicShader("../shaders/spaceship.vertex.glsl","../shaders/spaceship.fragment.glsl");
    Shader skyboxShader = basicShader("../shaders/skybox.vertex.glsl","../shaders/skybox.fragment.glsl");
    Shader colorShader = basicShader("../shaders/flat_color.vertex.glsl","../shaders/flat_color.fragment.glsl");

    //camera initialisation
    // CameraTrans cam = CameraTrans();
    Camera cam = Camera();
    cam.setWindowSize(int(window_width), int( window_height));
    // cam.setToTps();
    //model matrix (used to move the object)
    glm::mat4 model = glm::mat4(1.0);

    //view matrix, to move the camera
    // cam.lookAtPoint(mh.getCenterofScene());
    cam.setPosition( mh.getCenterofScene()+glm::vec3(0.0,0.0,150.0));
    glm::mat4 view = cam.getView();


    // projection matrix, to decide camera parameters (fov, screen ratio and cliping distance)
    glm::mat4 proj = cam.getProj();


    bbshader.use();
    bbshader.setMat4f("view", view);
    bbshader.setMat4f("proj", proj);
    bbshader.setMat4f("model", model);
    bbshader.setVec2f("windows_size", window_width, window_height);

    surfaceShader.use();
    surfaceShader.setMat4f("view", view);
    surfaceShader.setMat4f("proj", proj);
    surfaceShader.setMat4f("model", model);

    spaceshipShader.use();
    spaceshipShader.setMat4f("view", view);
    spaceshipShader.setMat4f("proj", proj);
    spaceshipShader.setMat4f("model", model);


    skyboxShader.use();
    skyboxShader.setMat4f("view", view);
    skyboxShader.setMat4f("proj", proj);
    skyboxShader.setInt("skybox", 0);

    colorShader.use();
    colorShader.setMat4f("view", view);
    colorShader.setMat4f("proj", proj);
    colorShader.setMat4f("model", model);

    docking_data.d_cam = &cam;
    spaceship_data.d_cam = &cam;


    /// screen shader related stuff===========================================================================================
    //========================================================================================================================

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
 //prepare framebuffer *------------------------------------------------------------------------
    docking_data.texColorBuffer= getTexColorBuffer(window_width, window_height);
    docking_data.texDepthBuffer= getTexDepthBuffer(window_width, window_height);
    docking_data.framebuffer = getScreenFramebuffer(docking_data.texColorBuffer,docking_data.texDepthBuffer);

    spaceship_data.texColorBuffer= getTexColorBuffer(window_width, window_height);
    spaceship_data.texDepthBuffer= getTexDepthBuffer(window_width, window_height);
    spaceship_data.framebuffer = getScreenFramebuffer(spaceship_data.texColorBuffer,spaceship_data.texDepthBuffer);

    docking_data.resizeWindows = false;
    docking_data.fullscreen = false;
    //shader to render the framebuffer on the screen quad
    Shader screenQuadShader = basicShader("../shaders/screen.vertex.glsl","../shaders/screen.fragment.glsl");

    //matrices for motion blur
    glm::mat4 viewProjectionInverse = glm::inverse (view * model);
    screenQuadShader.setMat4f("viewProjectionInverse", viewProjectionInverse);
    glm::mat4 previousViewProjection =  proj * view * model ;
    screenQuadShader.setMat4f("previousViewProjection",  previousViewProjection);

   
    //OPTION-------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST); // find a place to put options 

    //Init Timer
    NYTimer * g_timer = new NYTimer();
    g_timer->start();

    docking_data.frames = 0;
    docking_data.fps = 0;
    docking_data.frameRate = 30;
    docking_data.averageFrameTimeMilliseconds = 33.333;

    spaceship_data.frames = 0;
    spaceship_data.fps = 0;
    spaceship_data.frameRate = 30;
    spaceship_data.averageFrameTimeMilliseconds = 33.333;
    //the event loop-----------------------------------------------------------
    // ContraintRenderer c =    ContraintRenderer();


    ConstraintManager cm =ConstraintManager();
    lineRenderer l = lineRenderer(glm::vec3(50.0,0.0,0.0),glm::vec3(0.0,0.0,0.0));



    docking_data.cm = &cm;
    docking_data.l = &l; 
    // docking_data.c = &c;

    
    spaceship_data.cm = &cm;
    spaceship_data.l = &l;
    // spaceship_data.c = &c;

    // spaceship controll
    spaceship_controller sc = spaceship_controller(shipBody);
    spaceship_data.spaceship = &sc;
    docking_data.spaceship = &sc;
    
    Camera camShip = Camera();
    camShip.setWindowSize(int(window_width), int( window_height));
    spaceship_data.ss_cam = &camShip;
    docking_data.ss_cam = &camShip;
    spaceship_data.ss_cam->setPosition( spaceship_data.spaceship->getCamPos());

    ParticlesEmitter pe = ParticlesEmitter(); 
    spaceship_data.pe = &pe;
    beamRenderer beam = beamRenderer();
    spaceship_data.beam = &beam;


    //skybox

    docking_data.cubemapTexture= cubemapTexture;
    spaceship_data.cubemapTexture=cubemapTexture;
    //VAO
    docking_data.vao = vao;
    docking_data.quadVAO = quadVAO;
    docking_data.VAO_SKYBOX = VAO_SKY;
    docking_data.VAO_SPHERE=VAO_SPHERE;

    spaceship_data.vao = vao;
    spaceship_data.quadVAO = quadVAO;
    spaceship_data.VAO_SPACESHIP = VAO_SPACESHIP;
    spaceship_data.VAO_SKYBOX = VAO_SKY;
    spaceship_data.VAO_SPHERE = VAO_SPHERE;
    spaceship_data.VAO_FLAME  = VAO_FLAME;
    // docking_data.quadVAO = initScreenQuadRendering(quadVertices);


    docking_data.screenQuadShader = screenQuadShader;
    docking_data.surfaceShader = surfaceShader;
    docking_data.colorShader = colorShader;
    docking_data.nbsphereVertices=nbsphereVertices;
    docking_data.skyboxShader = skyboxShader;

    spaceship_data.screenQuadShader = screenQuadShader;
    spaceship_data.surfaceShader = surfaceShader;
    spaceship_data.skyboxShader = skyboxShader;
    spaceship_data.spaceshipShader= spaceshipShader;
    spaceship_data.spaceshipVertices = nbspaceshipVertices;
    spaceship_data.colorShader = colorShader;
    spaceship_data.nbsphereVertices = nbsphereVertices;
    spaceship_data.nbflameVertices = nbflameVertices;

    docking_data.energyCalcTime =0.0f;
    docking_data.damp = true;
    docking_data.ener =0.0f; //energy of the molecular complex
    docking_data.prog = 0.0f; // progress of the optim

    spaceship_data.energyCalcTime =0.0f;
    spaceship_data.damp = true;
    spaceship_data.ener =0.0f; //energy of the molecular complex
    spaceship_data.prog = 0.0f; // progress of the optim
    
    docking_data.running = true;
    docking_data.surf = true;
    docking_data.optim = false;
    docking_data.freeze = false;
    docking_data.showCamFocus = true;
    docking_data.d_cam->fps=true;

    spaceship_data.running = true;
    spaceship_data.surf = true;
    spaceship_data.optim = false;
    spaceship_data.freeze = false;
    spaceship_data.fire = false;
    spaceship_data.spaceshipMoving = false;

    float mouseSens = 1.0f;
    spaceship_data.mouseSensitivity = &mouseSens;
    docking_data.mouseSensitivity = &mouseSens;


    ImVec4 clear_color = ImVec4(0.3f, 0.7f, 0.95f, 1.0f);
    spaceship_data.bgColor = &clear_color;
    docking_data.bgColor = &clear_color;

    bool showSkybox = true;
    spaceship_data.showSkybox=&showSkybox;
    docking_data.showSkybox=&showSkybox;
    bool invertedAxis = false;
    spaceship_data.invertedAxis = &invertedAxis;
    docking_data.invertedAxis =  &invertedAxis;
    bool wasd =false;
    spaceship_data.wasd = &wasd;
    docking_data.wasd = &wasd;

    docking_data.font = font2;
    int current_scene = docking;

    SDL_bool relativeMouseMode = SDL_FALSE ;
    docking_data.relativeMouseMode = &relativeMouseMode;

    spaceship_data.currentWeapon = constraintMover;
    // std::cout<<"showSkybox "<< showSkybox<<std::endl;
    spaceship_data.viewMode = firstPerson;
    while (current_scene != END)
    {
        //used for changing resolution
        if (docking_data.resizeWindows){
            spaceship_data.texColorBuffer = docking_data.texColorBuffer;
            spaceship_data.texDepthBuffer = docking_data.texDepthBuffer;
            spaceship_data.framebuffer = docking_data.framebuffer;
            docking_data.resizeWindows = false;
        }
        SDL_SetRelativeMouseMode(relativeMouseMode);
        switch(current_scene)
        {
            case docking :

                // SDL_ShowCursor(SDL_ENABLE);

                relativeMouseMode = SDL_FALSE; 
                // current_scene = spaceship_scene( window, spaceship_data, g_timer);
                current_scene = docking_scene( window, docking_data, g_timer);
                break;
            
            case spaceship : 
                // SDL_ShowCursor(SDL_DISABLE);
                relativeMouseMode = SDL_TRUE;
                current_scene = spaceship_scene( window, spaceship_data, g_timer);
                break;
        }
        

    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();


    return 0;

}