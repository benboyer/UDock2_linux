
#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;


float near = 1.0f; 
float far  = 150.0f; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0f - 1.0f; // Back to NDC 
    return (2.0f * near * far) / (far + near - z * (far - near));	
}


void main()
{

	vec4 texcolor = texture(ourTexture, TexCoord);

	if (texcolor.a > 0.5f ){
		//base color
		// vec4 objectColor = vec4(0.0f, 0.5f, 0.31f,1.0f);
		// vec4 objectColor = vec4(0.76f, 0.88f, 0.92f,1.0f);
		// vec4 objectColor = vec4(0.89f, 0.92f, 0.43f,1.0f);
		// vec4 objectColor = vec4(1.0f, 0.7f, 0.34f,1.0f);
		vec4 objectColor = vec4(0.0f, 0.6f, 0.41f,1.0f);
		
		vec4 result = texcolor * objectColor;

		color =result;
		// color =vec4(0.0f, 0.5f, 0.31f,1.0f);
	}
	// //base color
	// vec3 objectColor = vec3(0.0f, 0.5f, 0.31f);
	// vec3 lightColor =vec3(1.0f, 1.0f, 1.0f);

	// //ambient ligth
	// float ambientStrength = 0.1f;
	// vec3 ambient = ambientStrength * lightColor;

	// //surface ligth
	// vec3 lightDir = normalize(vec3(1.0f,1.0f,0.0f));
	// float diff = max(dot(texcolor.xyz, lightDir), 0.0f);
	// vec3 diffuse = diff * lightColor;

	//depth cuing
	//float depth =  LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	//vec3 depthCue = depth * lightColor;

	// vec3 result = (ambient + diffuse ) * objectColor; //- depthCue*0.5 ;

	

    // color = texcolor;
	//try to manage depth
	//float size = 1.7f;
	//gl_FragDepth +=  texcolor.a;//*size; 

	else{
	 	 discard;
	}
	


}  

