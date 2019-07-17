#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

// // gl3w Headers
// #include <gl3w.h>

// GLM headers
#include <glm/glm.hpp>

#include "shaders_utils.h"
// #include "billboardshader.h"
#include <vector>


// class Renderer
// {
//     public:
//     std::vector<float> vertices;
//     unsigned int VAO, VBO;
//     int nbvertices;
//     Shader shader;
//     // constructor reads and builds the shader
//     Renderer(){}

//     void render(){};
//     void setView(glm::mat4){};
//     void setProjection(glm::mat4){};
//     void setModel(glm::mat4){};

// };


// class ContraintRenderer
// {

//     unsigned int cr_VAO, cr_VBO;
//     public:
//     std::vector<float> vertices;
//     int nbvertices;
//     Shader shader;
    
//     glm::mat4 view;
//     glm::mat4 projection;
//     glm::mat4 model;


//     glm::vec3 up;
//     glm::vec3 right;

//     ContraintRenderer(){

//     // The VBO containing the 4 vertices of the particles.
//     // static const GLfloat g_vertex_buffer_data[] = 
//     std::vector<float> v{ 
//          -0.5f, -0.5f, 0.0f,
//           0.5f, -0.5f, 0.0f,
//          -0.5f,  0.5f, 0.0f,
//           0.5f,  0.5f, 0.0f,
//     };

//     vertices=v;
//     nbvertices = vertices.size();

//     // GLuint billboard_vertex_buffer;
//     // glGenBuffers(1, &billboard_vertex_buffer);
//     // glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
//     glGenVertexArrays(1, &cr_VAO);
//     glBindVertexArray(cr_VAO);

//     glGenBuffers(1, &cr_VBO);
//     glBindBuffer(GL_ARRAY_BUFFER, cr_VBO);
//     // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
//     glBufferData(GL_ARRAY_BUFFER,nbvertices * sizeof (float), vertices.data(), GL_DYNAMIC_DRAW);


//     // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//     // glEnableVertexAttribArray(0);


//     // std::vector<float> v
//     // { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//     //     // positions   // texCoords
//     //     -1.0f,  1.0f,  0.0f, 1.0f,
//     //     -1.0f, -1.0f,  0.0f, 0.0f,
//     //      1.0f, -1.0f,  1.0f, 0.0f,

//     //     -1.0f,  1.0f,  0.0f, 1.0f,
//     //      1.0f, -1.0f,  1.0f, 0.0f,
//     //      1.0f,  1.0f,  1.0f, 1.0f
//     // };

//     // vertices=v;
//     // nbvertices = vertices.size();

//     // glGenVertexArrays(1, &cr_VAO);
//     // glBindVertexArray(cr_VAO);

//     // glGenBuffers(1, &cr_VBO);
//     // glBindBuffer(GL_ARRAY_BUFFER, cr_VBO);
//     // // fill the buffer with the data
//     // glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);

//     // how to read the data

//     // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//     // glEnableVertexAttribArray(0);

//     // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
//     // glEnableVertexAttribArray(1);

//     shader= basicShader("../shaders/constraint.vertex.glsl","../shaders/constraint.fragment.glsl");

//     }

//     void setView(glm::mat4 value){
//         view = value;
//     }

//     void setProjection(glm::mat4 value){
//         projection = value;
//     }

//     void setModel(glm::mat4 value){
//         model = value;
//     }


//     void setUp(glm::vec3 value){
//         up = value;
//     }

//     void setRight(glm::vec3 value){
//        right = value;
//     }

//     void render()
//     {

//         shader.use();
//         // Enable depth test
//         glEnable(GL_DEPTH_TEST);
//         // Accept fragment if it closer to the camera than the former one
//         glDepthFunc(GL_LESS);


//         glBindVertexArray(cr_VAO);
//         glBufferData(GL_ARRAY_BUFFER,nbvertices * sizeof (float), vertices.data(), GL_DYNAMIC_DRAW);

//         glEnableVertexAttribArray(0);
//         glBindBuffer(GL_ARRAY_BUFFER, cr_VBO);
//         glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

//         // glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);

        

        
//         // This is equivalent to mlutiplying (1,0,0) and (0,1,0) by inverse(ViewMatrix).
//         // ViewMatrix is orthogonal (it was made this way), 
//         // so its inverse is also its transpose, 
//         // and transposing a matrix is "free" (inversing is slooow)
//         shader.setVec3f("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
//         shader.setVec3f("cameraUp_Worldspace" , view[0][1], view[1][1], view[2][1]);
//         // shader.setVec3f("cameraRigth_Worldspace",right);
//         // shader.setVec3f("cameraUp_Worldspace",up);
//         shader.setMat4f("VP", projection* view );

//         shader.setVec3f("constraintPos",glm::vec3(0.0,0.0,0.0));
//         shader.setVec2f("constraintSize",10.0,10.0);

//         // glDrawArrays(GL_TRIANGLES, 0, 34);
//         // glBindVertexArray(0);

//                 // This draws a triangle_strip which looks like a quad.
//         glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//         glDisableVertexAttribArray(0);
//     }

// };


class cubeFlatColorRenderer //:public Renderer
{ 


    public:
    std::vector<float> vertices;
    unsigned int VAO, VBO;
    int nbvertices;
    Shader shader;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    glm::vec4 color;


    cubeFlatColorRenderer(){

        std::vector<float> v // float verticesCube[] =
        {
            -0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,   
             0.5f,  0.5f, -0.5f,   
             0.5f,  0.5f, -0.5f,   
            -0.5f,  0.5f, -0.5f,   
            -0.5f, -0.5f, -0.5f,   

            -0.5f, -0.5f,  0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f,  

            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f,  0.5f, 
            -0.5f,  0.5f,  0.5f, 

             0.5f,  0.5f,  0.5f, 
             0.5f,  0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  

            -0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f, -0.5f, 

            -0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f, -0.5f
        };
        vertices=v;
        nbvertices = vertices.size();
        
        // std::cout<<nbvertices<<std::endl;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //fill the buffer with the data
        glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);

        //how to read the data
   
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // shader= basicShader("../shaders/vertex_mesh_normal.glsl","../shaders/fragment_mesh_normal.glsl");
        shader=basicShader("../shaders/flat_color.vertex.glsl","../shaders/flat_color.fragment.glsl");

        color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    }


    void setView(glm::mat4 value){
        view = value;
    }

    void setProjection(glm::mat4 value){
        projection = value;
    }

    void setModel(glm::mat4 value){
        model = value;
    }

    void setColor(glm::vec4 value){
        color = value;
    }

    void render()
    {
        glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);

        shader.use();

        glBindVertexArray(VAO);
        shader.setMat4f("view", view);
        shader.setMat4f("proj", projection);
        shader.setMat4f("model", model);
        shader.setVec4f("color",color);


        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

};


class cubeNormalRenderer //:public Renderer
{   

    public:
    std::vector<float> vertices;
    unsigned int VAO, VBO;
    int nbvertices;
    Shader shader;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    glm::vec4 color;


    cubeNormalRenderer(){

        std::vector<float> v // float verticesCube[] =
        {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };
        vertices=v;
        nbvertices = vertices.size();
        
        // std::cout<<nbvertices<<std::endl;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //fill the buffer with the data
        glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);

        //how to read the data
   
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        shader= basicShader("../shaders/vertex_mesh_normal.glsl","../shaders/fragment_mesh_normal.glsl");
        color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    }


    void setView(glm::mat4 value){
        view = value;
    }

    void setProjection(glm::mat4 value){
        projection = value;
    }

    void setModel(glm::mat4 value){
        model = value;
    }

    void setColor(glm::vec4 value){
        color = value;
    }

    void render()
    {
        glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);

        shader.use();

        glBindVertexArray(VAO);
        shader.setMat4f("view", view);
        shader.setMat4f("proj", projection);
        shader.setMat4f("model", model);
        shader.setVec4f("color",color);


        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

};




    // //On calcule la normale
    // for (int i =0; i< 36*3; i+=9){

    //     glm::vec3 v1 = glm::vec3(vertices[i+3]- vertices[i+0],vertices[i+4] - vertices[i+1],vertices[i+5] - vertices[i+2]);
    //     glm::vec3 v2 = glm::vec3(vertices[i+6]- vertices[i+0],vertices[i+7] - vertices[i+1],vertices[i+8] - vertices[i+2]);
    //     glm::vec3 normal = glm::cross(v2,v1);//v2.vecProd(v1); //!\\ danger ordre
    //     normalize (normal);
    //     std::cout << normal.x<<" "<< normal.y<<" "<< normal.z<< std::endl;

    // }




// class lineRenderer //:public Renderer
// {   

//     public:
//     std::vector<float> vertices;
//     unsigned int VAO, VBO;
//     int nbvertices;
//     Shader shader;

//     glm::mat4 view;
//     glm::mat4 projection;
//     glm::mat4 model;
//     lineRenderer(glm::vec3 P1,glm::vec3 P2){

//  vertices.push_back(P1.x);
//  vertices.push_back(P1.y);
//  vertices.push_back(P1.z);

//  vertices.push_back(P2.x);
//  vertices.push_back(P2.y);
//  vertices.push_back(P2.z);

//     nbvertices = vertices.size();
    

//     glGenVertexArrays(1, &VAO);
//     glBindVertexArray(VAO);

//     glGenBuffers(1, &VBO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     //fill the buffer with the data
//     glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);
//     //how to read the data
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//  // shader= lineShader();
//     shader= basicShader("../shaders/line.vertex.glsl","../shaders/line.fragment.glsl");
//     }

//     void setPoints(glm::vec3 P1,glm::vec3 P2){

//         vertices.clear();
//         vertices.push_back(P1.x);
//         vertices.push_back(P1.y);
//         vertices.push_back(P1.z);

//         vertices.push_back(P2.x);
//         vertices.push_back(P2.y);
//         vertices.push_back(P2.z);
//     }
//     void setView(glm::mat4 value){
//      view = value;
//     }

//     void setProjection(glm::mat4 value){
//      projection = value;
//     }

//     void setModel(glm::mat4 value){
//         model = value;
//     }

//     void render()
//     {
//      glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);

//         shader.use();

//         glBindVertexArray(VAO);
//         shader.setMat4f("view", view);
//         shader.setMat4f("proj", projection);
//         // shader.setMat4f("VP", projection * view);
//         shader.setMat4f("model", model);


//         glDrawArrays(GL_LINES, 0, 2);
//         glBindVertexArray(0);
//     }

// };




std::vector<float> loadOBJ(std::string inputfile){

    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
  
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(),"../models/");
      
    if (!err.empty()) { // `err` may contain warning message.
      std::cerr << err << std::endl;
    }

    if (!ret) {
      exit(1);
    }
    // std::cout<<attrib.vertices.size()/3<<std::endl;
    // std::cout<<attrib.normals.size()/3<<std::endl;
    // std::cout<<attrib.texcoords.size()/2<<std::endl;
    // std::cout<<materials.size()<<std::endl;

    std::vector<float> vertsurf;


    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            unsigned int fv = shapes[s].mesh.num_face_vertices[f];

            int materialID =shapes[s].mesh.material_ids[f];
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                vertsurf.push_back(vx);
                vertsurf.push_back(vy);
                vertsurf.push_back(vz);
                //normal
                tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
                vertsurf.push_back(nx);
                vertsurf.push_back(ny);
                vertsurf.push_back(nz);

                vertsurf.push_back(materials[ materialID].diffuse[0]);
                vertsurf.push_back(materials[ materialID].diffuse[1]);
                vertsurf.push_back(materials[ materialID].diffuse[2]);
           
            }
            index_offset += fv;
        }
    }
    return vertsurf;
}



class objRenderer //:public Renderer
{   

    public:
    std::vector<float> vertices;
    unsigned int VAO, VBO;
    int nbvertices;
    Shader shader;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;

    objRenderer(std::string inputfile){

        vertices =loadOBJ(inputfile);

        nbvertices = vertices.size();
        


        //model vao
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);


        // std::cout<<"vertsurf size : "<<shapes.size()<<std::endl;
        //fill the buffer with the data
        glBufferData(GL_ARRAY_BUFFER, nbvertices* sizeof (float), vertices.data(), GL_STATIC_DRAW);

        //how to understand the data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

        shader=  basicShader("../shaders/spaceship.vertex.glsl","../shaders/spaceship.fragment.glsl");
    }

    objRenderer(){

        std::vector<float> v {0.0f};

        vertices = v;

        nbvertices = vertices.size();
        


        //model vao
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);


        // std::cout<<"vertsurf size : "<<shapes.size()<<std::endl;
        //fill the buffer with the data
        glBufferData(GL_ARRAY_BUFFER, nbvertices* sizeof (float), vertices.data(), GL_STATIC_DRAW);

        //how to understand the data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

        shader=  basicShader("../shaders/spaceship.vertex.glsl","../shaders/spaceship.fragment.glsl");
    }


    void setView(glm::mat4 value){
        view = value;
    }

    void setProjection(glm::mat4 value){
        projection = value;
    }

    void setModel(glm::mat4 value){
        model = value;
    }


    void render()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);

        shader.use();

        // glBindVertexArray(VAO);
        shader.setMat4f("view", view);
        shader.setMat4f("proj", projection);
        shader.setMat4f("model", model);

        glDrawArrays(GL_TRIANGLES, 0, nbvertices/3);
        glBindVertexArray(0);
    }
};





class beamRenderer{

    public:
    
    glm::vec3 pointA, pointB;
    objRenderer objModel;
    Shader shader;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    glm::vec4 color;

    float scale = 0.2f;

    beamRenderer(){
        objModel = objRenderer("../models/cylinder-center.obj");
    }
    beamRenderer(glm::vec3 P1,glm::vec3 P2){
        pointA = P1;
        pointB = P2;
        glm::vec3 AB = glm::normalize(pointB-pointA);
        glm::vec3 orientationCylinder = glm::vec3(0.0f,1.0f,0.0f);
        objModel = objRenderer("../models/cylinder-center.obj");
        
        model = glm::mat4(1);
        // deplacer le cylindre sur middle point A and point B
        model = glm::translate(model,(pointA+pointB)/2.0f);
        //orient it in direction of point B
        glm::vec3 norm = glm::cross(orientationCylinder,AB);
        float cosA = glm::dot(orientationCylinder,AB);
        glm::clamp(cosA,-1.0f,1.0f);
        float angle = (glm::acos(cosA));//glm::degrees
        //std::cout<< "angle : "<< angle <<" acos " << glm::acos(cosA) <<" dot "<< cosA<< std::endl;
        // std::cout<< "norm : "<< norm.x << " " << norm.y <<" " << norm.z << std::endl;
        model *= glm::rotate(glm::mat4(1),angle, norm);
        
        //stretch it to point B
        model *= glm::scale( glm::vec3(scale,length(pointB -pointA),scale));
        // objModel.setModel(modelcylinder);
    }

    void setPoints(glm::vec3 P1,glm::vec3 P2){
        pointA = P1;
        pointB = P2;
        glm::vec3 AB = glm::normalize(pointB-pointA);
        glm::vec3 orientationCylinder = glm::vec3(0.0f,1.0f,0.0f);
        model = glm::mat4(1);
        // deplacer le cylindre sur middle point A and point B
        model = glm::translate(model,(pointA+pointB)/2.0f);
        //orient it in direction of point B
        glm::vec3 norm = glm::cross(orientationCylinder,AB);
        float cosA = glm::dot(orientationCylinder,AB);
        glm::clamp(cosA,-1.0f,1.0f);
        float angle = (glm::acos(cosA));//glm::degrees
        //std::cout<< "angle : "<< angle <<" acos " << glm::acos(cosA) <<" dot "<< cosA<< std::endl;
        // std::cout<< "norm : "<< norm.x << " " << norm.y <<" " << norm.z << std::endl;
        model *= glm::rotate(glm::mat4(1),angle, norm);
        
        //stretch it to point B
        model *= glm::scale( glm::vec3(scale,length(pointB -pointA),scale));
    }
    void setView(glm::mat4 value){
        view = value;
    }

    void setProjection(glm::mat4 value){
        projection = value;
    }

    void render(){
        objModel.setView(view);
        objModel.setProjection(projection);
        objModel.setModel(model);
        objModel.render();
    }
};




class lineRenderer //:public Renderer
{   

    public:
    std::vector<float> vertices;
    unsigned int VAO, VBO;
    int nbvertices;
    Shader shader;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    glm::vec4 color;


    lineRenderer(glm::vec3 P1,glm::vec3 P2){
    color = glm::vec4(0.0,0.0,0.0,1.0);

    vertices.push_back(P1.x);
    vertices.push_back(P1.y);
    vertices.push_back(P1.z);

    vertices.push_back(P2.x);
    vertices.push_back(P2.y);
    vertices.push_back(P2.z);

    nbvertices = vertices.size();
    

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //fill the buffer with the data
    glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);
    //how to read the data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    shader= basicShader("../shaders/line.vertex.glsl","../shaders/line.fragment.glsl");
    }

    void setPoints(glm::vec3 P1,glm::vec3 P2){

        vertices.clear();
        vertices.push_back(P1.x);
        vertices.push_back(P1.y);
        vertices.push_back(P1.z);

        vertices.push_back(P2.x);
        vertices.push_back(P2.y);
        vertices.push_back(P2.z);
    }
    void setView(glm::mat4 value){
        view = value;
    }

    void setProjection(glm::mat4 value){
        projection = value;
    }

    void setModel(glm::mat4 value){
        model = value;
    }

    void setColor(glm::vec4 value){
        color = value;
    }
    void render()
    {
        // glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, nbvertices * sizeof (float), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        shader.use();

        // std::cout << glm::to_string(glm::vec3(vertices[0],vertices[1],vertices[2]))<<" "<<glm::to_string(glm::vec3(vertices[3],vertices[4],vertices[5])) <<std::endl;
        // glBindVertexArray(VAO);
        shader.setMat4f("view", view);
        shader.setMat4f("proj", projection);
        shader.setMat4f("model", model);
        shader.setVec4f("color",color);

        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
        // glBindBuffer(GL_ARRAY_BUFFER,0);
    }

};
#endif