
#version 330 core

// in vec3 FragPos;
in vec3 Normal;
in vec3 ourColor;

out vec4 FragColor;

void main()
{
		// 	//base color
		// // vec3 objectColor = vec3(0.0f, 0.0f, 0.0f);
		// // vec3 objectColor = vec3(0.0f, 1.0f, 1.0f);
		// vec3 objectColor = ourColor;

		// // vec3 objectColor = vec3(0.0f, 0.5f, 0.31f);
		// vec3 lightColor =vec3(1.0f, 1.0f, 1.0f);

		// //ambient ligth
		// float ambientStrength = 0.1f;
		// vec3 ambient = ambientStrength * lightColor;

	 //    //surface ligth
	 //    vec3 norm = normalize(Normal);

	 //    vec3 lightDir = normalize(-vec3(0.0f,-20.0f,-1.0f));
		// // vec3 lightDir = normalize(vec3(0.0f,20.0f,0.0f) - FragPos);
		// // vec3 lightDir = normalize(vec3(1.0f,0.5f,-1.5f) );
		// float diff = max(dot(norm, lightDir), 0.0f);
		// vec3 diffuse1 = diff * lightColor;

		// // // secondary ligth
		// // norm = normalize(Normal);

	 // //    lightDir = normalize(-vec3(10.0f,20.0f,0.0f));
		// // // vec3 lightDir = normalize(vec3(0.0f,20.0f,0.0f) - FragPos);
		// // // vec3 lightDir = normalize(vec3(1.0f,0.5f,-1.5f) );
		// // diff = max(dot(norm, lightDir), 0.0f);
		// // vec3 diffuse2 = diff * lightColor;

	 //    vec3 result = min(( ambient +diffuse1 ),vec3(1.0f, 1.0f, 1.0f))  * objectColor;


	 //    FragColor = vec4(sqrt(result), 1.0f);


    ////////////////////////////////////////////////////////////////////////////////////////////////////

	float ambientLevel = 0.75f;

    // Scaling The Input Vector To Length 1
    // vec3 norm = normalize(Normal);
   	// vec3 lightDir = normalize(-vec3(0.0f,-20.0f,-1.0f));

	vec3 normalized_normal = normalize(Normal);
	vec3 normalized_vertex_to_light_vector = normalize(-vec3(0.0f,-20.0f,-1.0f));


	//Calculating The Diffuse Term And Clamping It To [0;1]
	float DiffuseTerm = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);

	//Pour les bords
	// vec3 normalized_cam_dir = normalize(vertex_in_modelview_space);

	// float Bord = clamp(dot((-1*vs_normal_out), normalized_cam_dir), 0.0, 1.0);

	//Debug
	//final_color = vec4(vs_normal_out,1.0);//AmbientColor + color * DiffuseTerm;
	//final_color = vec4(normalized_vertex_to_light_vector,1.0);//AmbientColor + color * DiffuseTerm;

	// Calculating The Final Color
	vec3 final_color = ourColor * (DiffuseTerm*(1-ambientLevel) + ambientLevel);
	

	// if(Bord < tailleCerne)
	// 	final_color *= 0.4;

	// final_color.a = vs_color_out.a;

	FragColor = vec4(final_color,1.0f);
}