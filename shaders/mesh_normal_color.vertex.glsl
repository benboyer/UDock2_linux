#version 330 core
layout (location = 0) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aPos;

// out vec3 FragPos;
out vec3 Normal;
out vec3 ourColor; // output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;






void main()
{

	vec3 colorPositif = vec3(0.0f,0.0f,1.0f);
	vec3 colorNegatif = vec3(1.0,0.0,0.0);
	vec3 colorNeutre = vec3(1.0,1.0,1.0);
	// vec4 g_color_fond = vec4(0.0,0.347,0.398,1.0);
	// float g_ambient_level = 0.75f;
	// float g_taille_cerne = 0.0f;
	int invertPolarity = 0;



	// note that we read the multiplication from right to left
	gl_Position = proj * view * model * vec4(aPos, 1.0);
	// FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	ourColor = aColor; // set ourColor to the input color we got from the vertex data
	
	//Positif
	if(aColor.r < 1.0)
	{
		if(invertPolarity == 0)
			ourColor = colorPositif * (1-aColor.r) + colorNeutre * aColor.r;
		else
			ourColor = colorNegatif * (1-aColor.r) + colorNeutre * aColor.r;
	}

	//Negatif
	if(aColor.b < 1.0)
	{
		if(invertPolarity == 0)
			ourColor = colorNegatif * (1-aColor.b) + colorNeutre * aColor.b;
		else
			ourColor = colorPositif * (1-aColor.b) + colorNeutre * aColor.b;
	}

	// ourColor.a = aColor.a;
}