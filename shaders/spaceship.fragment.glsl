#version 330 core

// in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
out vec4 FragColor;

void main()
{
		//base color
	// vec3 objectColor = vec3(0.0f, 0.0f, 0.0f);
	// vec3 objectColor = vec3(1.0f, 0.5f, 0.2f);
	vec3 objectColor = Color;
	// vec3 objectColor = vec3(0.0f, 0.5f, 0.31f);
	vec3 lightColor =vec3(1.0f, 1.0f, 1.0f);

	//ambient ligth
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

    //surface ligth
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(vec3(5.0f,20.0f,5.0f));
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse1 = diff * lightColor;

	lightDir = normalize(-vec3(3.0f,20.0f,0.0f));
	diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse2 = diff * lightColor;



    vec3 result = (ambient + diffuse1 + diffuse2)  * objectColor;


    FragColor = vec4(result, 1.0f);
}