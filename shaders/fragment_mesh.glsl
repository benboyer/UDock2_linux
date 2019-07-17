#version 150


// uniform vec3 triangleColor;

in vec3 FragPos;

out vec4 outColor;



void main()
{

	//base color
	// vec3 objectColor = vec3(0.0f, 0.0f, 0.0f);
	vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

	// // vec3 objectColor = vec3(0.0f, 0.5f, 0.31f);
	// vec3 lightColor =vec3(1.0f, 1.0f, 1.0f);

	// //ambient ligth
	// float ambientStrength = 0.5f;
	// vec3 ambient = ambientStrength * lightColor;

 //    //surface ligth
	// vec3 lightDir = normalize(vec3(1.0f,0.5f,-1.5f));
	// float diff = max(dot(FragPos, lightDir), 0.0f);
	// vec3 diffuse = diff * lightColor;

 //    vec3 result = (ambient + diffuse)  * objectColor;

 //    outColor = vec4(result, 1.0f);
    outColor = vec4(objectColor,1.0f);
}