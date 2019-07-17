		#version 330 core
		out vec4 FragColor;
		  
		in vec2 TexCoords;

		uniform sampler2D screenTexture;
		uniform sampler2D depthTexture;

		uniform float screen_width;
		uniform float screen_height;

		uniform mat4 viewProjectionInverse;
		uniform mat4 previousViewProjection;

		const float offset = 1. /1280.0;

		const vec2 offsets[9] = vec2[](
					vec2(-offset, offset),
					vec2( 0.0f, offset),
					vec2(offset, offset),
					vec2(-offset, 0.0f),
					vec2(0.0f, 0.0f),
					vec2( offset, 0.0f),
					vec2(-offset, -offset),
					vec2( 0.0f, -offset),
					vec2(offset, -offset)
					);

				// // sharpen
				const float kernel[9] = float[](
					0.8, 1, 0.8,
					1,-8.8, 1,
					0.8, 1, 0.8
				);




		const float near = 1.0;
		const float far = 500.0;

		float LinearizeDepth(float depth)
		{
			float z = depth * 2.0 - 1.0; // back to NDC
			// float z = depth;
			return (2.0 * near * far) / (far + near - z * (far - near));
		}

		// float LinearizeDepth(float z)
		// {
		// 	float zNear = 1.0; // camera z near
		// 	float zFar = 10000.0; // camera z far

		// 	// float z_n = z;
		// 	float z_n = 2.0 * z - 1.0;
		// 	float z_e = (2.0 * zNear * zFar) / (zFar + zNear - z_n * (zFar - zNear));

		// 	return z_e;
		// }


		void main()
		{ 


			//////////////////////////////////////
			/// CONTOUR
			//////////////////////////////////////
			float depthBase = texture( depthTexture , TexCoords ).r;	
			float depth = LinearizeDepth(depthBase);

				
				// vec3 sampleTex[9];
				float sampleTex[9];
				for (int i =0; i<9; i++) 
				{
					sampleTex[i] =abs(depth - LinearizeDepth(texture(depthTexture, TexCoords.st + offsets[i] /** (1-depth/1000)*/).r));//LinearizeDepth(abs(depth -
				}
				// vec3 col = vec3(0.0);
				float edge = 0;
				for (int i =0; i<9; i++)
				{
					// col += sampleTex[i] * kernel[i];
					edge+= sampleTex[i] * kernel[i];
				}

				edge /= 20.0;

				edge *= (1-depth/600);

				edge = min(edge,0.7);
				//vec4 edgeColor = vec4(vec3(edge),1.0);
				// float edgeAVG = (edge.x+edge.y+edge.z)/3.0;


			// float xstep = 1.0/screen_width;
			// float ystep = 1.0/screen_height;
			// float ratio = screen_width / screen_height;
			// float x,y,z;
			// float sommedepth=0;
			// float nb = 0;



			// for(x=-1;x<1;x++)                               
			// {
			// 	for(y=-1;y<1;y++)
			// 	{
			// 		vec2 coord =  TexCoords;
			// 		coord.x += x*xstep;
			// 		coord.y += y*ystep;
			// 		float depthLocal = texture( depthTexture , coord ).r;	
			// 		depthLocal = LinearizeDepth(depthLocal);
			// 		sommedepth += abs(depth - depthLocal) / depth; //plus on est loing, plus les differences de depth sont grandes cause perspective
			// 		nb++;
			// 	}
			// }
			
			// sommedepth /= nb;

			 // float depthBase = texture( depthTexture , vec2(TexCoords)).r;	
			// float depth = LinearizeDepth(depthBase);



			// Get the depth buffer value at this pixel. 
		   	float zOverW = texture(depthTexture,  vec2(TexCoords)).r;
		   	// zOverW = LinearizeDepth(depthBase);

			// H is the viewport position at this pixel in the range -1 to 1.
			vec4 H = vec4(TexCoords.x * 2.0f - 1.0f, (1.0f - TexCoords.y) * 2.0f - 1.0f,zOverW, 1.0f);
			// Transform by the view-projection inverse.
		   	vec4 D = viewProjectionInverse * H;
			// Divide by w to get the world position.
		   	vec4 worldPos = D / D.w;


			// Current viewport position
			vec4 currentPos = H;
			// Use the world position, and transform by the previous view-projection matrix.
		  	vec4 previousPos = previousViewProjection * worldPos;
			// Convert to nonhomogeneous points [-1,1] by dividing by w.
			previousPos /= previousPos.w;

			// Use this frame's position and last frame's to compute the pixel velocity.
		  	// vec2 velocity = (currentPos.xy - previousPos.xy)/2.0f;
		  	vec2 velocity = (currentPos.xy - previousPos.xy)/70.0f;



			int numSamples = 5;
			vec4 color = texture( screenTexture , vec2(TexCoords));




			
			vec2 sampleCoord = TexCoords + velocity;
			
			for(int i = 1; i < numSamples; ++i)
			{
				sampleCoord+=velocity;
				// vec2 offset =  numSamples * velocity; 
			  	// Sample the color buffer along the velocity vector.
			  	vec4 currentColor = texture( screenTexture , vec2( sampleCoord));
			  	// Add the current color to our color sum.
			  	color += currentColor;
			}
			// Average all of the samples to get the final blur color.


		    FragColor = color/ numSamples;

		    //if (edge > 0.15)
		    {
		    	FragColor = mix(FragColor, vec4(vec3(0.0),1.0),edge );
			}

		    // FragColor = edgeColor;
			// FragColor =vec4(sommedepth,sommedepth,sommedepth,1.0);
			//FragColor =vec4(depth/500.0,depth,depth,1.0);
			// if (TexCoords.x>0.5)
			// 	FragColor =vec4(depthBase,depthBase,depthBase,1.0);

			
		    	//Cerné
				// if(sommedepth > 0.1)
				// {
				// 	FragColor =vec4(0.0,0.0,0.0,1.0);
				// } 































		     // FragColor = vec4(velocity ,0.0f,1.0f);
		     // FragColor = vec4(zOverW,zOverW,zOverW ,1.0f);
		    // FragColor =D;
		    // FragColor =texture( depthTexture , vec2(TexCoords));

		 	// FragColor =texture( screenTexture , vec2(TexCoords));

		    // FragColor =texture( depthTexture , vec2(T exCoords)) + texture( screenTexture , vec2(TexCoords));

		 	///pixeling shader for fun!

		 	// float Pixels = 512.0;
		 	// float Pixels = 1024.0;
		 	// float dx = 10.0*(1.0 / Pixels);
		 	// float dy = 15.0*(1.0 / Pixels);

		 	// vec2 pixCoord = vec2 (dx * floor(TexCoords.x / dx), dy * floor(TexCoords.y / dy));

		 	// FragColor =texture( screenTexture ,pixCoord);
		} 