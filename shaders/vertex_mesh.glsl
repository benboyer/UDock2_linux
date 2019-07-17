#version 150

in vec3 position;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model *vec4(position, 1.0);

    FragPos = vec3(model * vec4(position, 1.0));
}
