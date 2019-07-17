#ifndef BILLBOARDSHADER_H
#define BILLBOARDSHADER_H

#include "shaders_utils.h"

// #include "stb_image.h"

class billboardShader: public Shader
{   
    public:
    billboardShader(){
        // //load image //to do :make a function
        // int w;
        // int h;
        // int comp;

        // unsigned char* image = stbi_load("../textures/sphere-goodsell2.png", &w, &h, &comp, 0);

        // if (image == nullptr)
        //     throw (std::string("Failed to load texture"));

        // GLuint sphereTexture;
        // glGenTextures(1, &sphereTexture);

        // glBindTexture(GL_TEXTURE_2D, sphereTexture);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // if (comp == 3)
        // {
        //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        //     std::cout << "rgb" << std::endl;
        // }
        // else if (comp == 4)
        // {
        //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        //     glEnable(GL_BLEND);
        //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //     std::cout << "rgba" << std::endl;
        //     glGenerateMipmap(GL_TEXTURE_2D);
        //     glBindTexture(GL_TEXTURE_2D, 0);
        // }
        // stbi_image_free(image);

        //vertex
        unsigned int vertexShader = loadShader(vertex,"../shaders/vertex.glsl");
        //geom
        unsigned int geomShader = loadShader(geometry,"../shaders/expandPoint2Quad_text.geom.glsl");
        //fragment
        unsigned int fragmentShader = loadShader(fragment,"../shaders/textsphere_computed.frag.glsl");

        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, geomShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        //we check for shader program compilation error
        checkShaderCompilation(shaderProgram);
        
        //we don't need those guys anymore
        glDeleteShader(vertexShader);
        glDeleteShader(geomShader);
        glDeleteShader(fragmentShader);

            //Making the link between vertex data and attributes
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(posAttrib);


        
        ID = shaderProgram;
    }
};

#endif


        // //vertex
        // unsigned int vertexShader = loadShader(vertex,"../shaders/vertex.glsl");
        // //geom
        
        // //fragment
        // unsigned int fragmentShader = loadShader(fragment,"../shaders/fragment_red.glsl");

        // unsigned int shaderProgram;
        // shaderProgram = glCreateProgram();
        // glAttachShader(shaderProgram, vertexShader);
        // glAttachShader(shaderProgram, fragmentShader);
        // glLinkProgram(shaderProgram);
        // //we check for shader program compilation error
        // checkShaderCompilation(shaderProgram);
        
        // //we don't need those guys anymore
        // glDeleteShader(vertexShader);
        // glDeleteShader(fragmentShader);

        // ID = shaderProgram;