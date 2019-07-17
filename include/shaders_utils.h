#ifndef SHADERS_UTILS_H
#define SHADERS_UTILS_H


// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ Headers
#include <iostream>
#include <math.h> 
#include <vector>

class Shader
{
    public:
    // constructor reads and builds the shader
    Shader(){}
    // the program ID
    unsigned int ID;
    // use/activate the shader
    virtual void use()
    {
        glUseProgram(ID);
    }

    // utility uniform functions
    virtual void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    
    virtual void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    
    virtual void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    virtual void setVec2f(const std::string &name, float value1, float value2)const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
    }

    virtual void setVec3f(const std::string &name, float value1, float value2, float value3)const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2,value3);
    }

    virtual void setVec3f(const std::string &name, glm::vec3 p)const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), p.x, p.y, p.z);
    }

    virtual void setVec4f(const std::string &name, float value1, float value2, float value3, float value4)const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2,value3,value4);
    }

    virtual void setVec4f(const std::string &name, glm::vec4 p)const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), p.x, p.y, p.z, p.w);
    }

    virtual void setMat4f(const std::string &name, glm::mat4 value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1, GL_FALSE,  glm::value_ptr(value));
    }

};


char* loadSource(const char *filename)
{
    char *src = NULL; /* code source de notre shader */
    FILE *fp = NULL; /* fichier */
    long size; /* taille du fichier */
    long i; /* compteur */


    /* on ouvre le fichier */
    fp = fopen(filename, "r");
    /* on verifie si l'ouverture a echoue */
    if (fp == NULL)
    {
        //Log::log(Log::ENGINE_ERROR,(std::string("Unable to load shader file ")+ std::string(filename)).c_str());
        std::cout << "Unable to load shader file " << std::endl;
        return NULL;
    }

    /* on recupere la longueur du fichier */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    /* on se replace au debut du fichier */
    rewind(fp);

    /* on alloue de la memoire pour y placer notre code source */
    src = (char*) malloc(size + 1); /* +1 pour le caractere de fin de chaine '\0' */
    if (src == NULL)
    {
        fclose(fp);
        //Log::log(Log::ENGINE_ERROR,"Unable to allocate memory for shader file before compilation");
        std::cout << "Unable to allocate memory for shader file before compilation " << std::endl;
        return NULL;
    }

    /* lecture du fichier */
    for (i = 0; i < size && !feof(fp); i++)
        src[i] = fgetc(fp);

    /* on place le dernier caractere a '\0' */
    src[i] = '\0';

    //sous windows, EOF a virer
    if (src[i - 1] == EOF)
        src[i - 1] = '\0';

    fclose(fp);

    return src;
}


enum shadertype { vertex, geometry, fragment};



unsigned int loadShader(shadertype type,const char *filename){
    const char* shaderSource = loadSource(filename);
    unsigned int shader;
    
     // GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER
    switch (type)
    {
        case vertex:
        {
            shader = glCreateShader(GL_VERTEX_SHADER);
            break;
        }
        case geometry:
        {
            shader = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        }
        case fragment:
        {
            shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }
    }
    
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    //we check for shader compilation error 
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    
     // GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER
        switch (type)
        {
            case vertex:
            {
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                break;
            }
            case geometry:
            {
                std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
                break;
            }
            case fragment:
            {
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                break;
            }
        }
    }
    

    return shader;
}



void checkShaderCompilation(unsigned int shaderProgram){
    //we check for shader program compilation error 
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog <<std::endl;
    }
}


// unsigned int getBlueShaderProgram ()
// {
//     //vertex
//     unsigned int vertexShader = loadShader(vertex,"../shaders/vertex.glsl");

//     //fragment
//     unsigned int fragmentShader = loadShader(fragment,"../shaders/fragment_blue.glsl");

//     unsigned int shaderProgram;
//     shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);
//     //we check for shader program compilation error
//     checkShaderCompilation(shaderProgram); 
    
//     //we don't need those guys anymore
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     return shaderProgram;
// }

// unsigned int getRedShaderProgram ()
// {
//     //vertex
//     unsigned int vertexShader = loadShader(vertex,"../shaders/vertex.glsl");

//     //fragment
//     unsigned int fragmentShader = loadShader(fragment,"../shaders/fragment_red.glsl");

//     unsigned int shaderProgram;
//     shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);
//     //we check for shader program compilation error
//     checkShaderCompilation(shaderProgram);
    
//     //we don't need those guys anymore
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     return shaderProgram;
// }



// class surfShader: public Shader
// {   
//     public:
//     surfShader(){

//         //vertex
//         unsigned int vertexShader = loadShader(vertex,"../shaders/vertex.glsl");
//         //fragment
//         unsigned int fragmentShader = loadShader(fragment,"../shaders/fragment.glsl");

//         unsigned int shaderProgram;
//         shaderProgram = glCreateProgram();

//         glAttachShader(shaderProgram, vertexShader);
//         glAttachShader(shaderProgram, fragmentShader);
//         glLinkProgram(shaderProgram);

//         //we check for shader program compilation error
//         checkShaderCompilation(shaderProgram);
        
//         //we don't need those guys anymore
//         glDeleteShader(vertexShader);
//         glDeleteShader(fragmentShader);
        
//         ID = shaderProgram;
//     }
// };

class basicShader: public Shader
{
    public:
    basicShader(const char *vertexShaderFile, const char *fragmentShaderFile){

        //vertex
        unsigned int vertexShader = loadShader(vertex,vertexShaderFile);
        //fragment
        unsigned int fragmentShader = loadShader(fragment,fragmentShaderFile);

        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        //we check for shader program compilation error
        checkShaderCompilation(shaderProgram);
        
        //we don't need those guys anymore
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        ID = shaderProgram;
    }
};



// class skyShader: public Shader
// {   
//     public:
//     skyShader(){

//         //vertex
//         unsigned int vertexShader = loadShader(vertex,"../shaders/skybox.vertex.glsl");
//         //fragment
//         unsigned int fragmentShader = loadShader(fragment,"../shaders/skybox.fragment.glsl");

//         unsigned int shaderProgram;
//         shaderProgram = glCreateProgram();

//         glAttachShader(shaderProgram, vertexShader);
//         glAttachShader(shaderProgram, fragmentShader);
//         glLinkProgram(shaderProgram);

//         //we check for shader program compilation error
//         checkShaderCompilation(shaderProgram);
        
//         //we don't need those guys anymore
//         glDeleteShader(vertexShader);
//         glDeleteShader(fragmentShader);
        
//         ID = shaderProgram;
//     }
// };




// class lineShader: public Shader
// {   
//     public:
//     lineShader(){

//         //vertex
//         unsigned int vertexShader = loadShader(vertex,"../shaders/line.vertex.glsl");
//         //fragment
//         unsigned int fragmentShader = loadShader(fragment,"../shaders/line.fragment.glsl");

//         unsigned int shaderProgram;
//         shaderProgram = glCreateProgram();

//         glAttachShader(shaderProgram, vertexShader);
//         glAttachShader(shaderProgram, fragmentShader);
//         glLinkProgram(shaderProgram);

//         //we check for shader program compilation error
//         checkShaderCompilation(shaderProgram);
        
//         //we don't need those guys anymore
//         glDeleteShader(vertexShader);
//         glDeleteShader(fragmentShader);
        
//         ID = shaderProgram;
//     }
// };



#endif