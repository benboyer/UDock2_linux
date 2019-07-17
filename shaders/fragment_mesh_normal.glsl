#version 330 core

// in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
		//base color
	// vec3 objectColor = vec3(0.0f, 0.0f, 0.0f);
	vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

	// vec3 objectColor = vec3(0.0f, 0.5f, 0.31f);
	vec3 lightColor =vec3(1.0f, 1.0f, 1.0f);

	//ambient ligth
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

    //surface ligth
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-vec3(0.0f,-20.0f,-1.0f));
	// vec3 lightDir = normalize(vec3(0.0f,20.0f,0.0f) - FragPos);
	// vec3 lightDir = normalize(vec3(1.0f,0.5f,-1.5f) );
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse1 = diff * lightColor;

	// // secondary ligth
	// norm = normalize(Normal);

 //    lightDir = normalize(-vec3(10.0f,20.0f,0.0f));
	// // vec3 lightDir = normalize(vec3(0.0f,20.0f,0.0f) - FragPos);
	// // vec3 lightDir = normalize(vec3(1.0f,0.5f,-1.5f) );
	// diff = max(dot(norm, lightDir), 0.0f);
	// vec3 diffuse2 = diff * lightColor;

    vec3 result = min(( ambient +diffuse1 ),vec3(1.0f, 1.0f, 1.0f))  * objectColor;


    FragColor = vec4(sqrt(result), 1.0f);
}