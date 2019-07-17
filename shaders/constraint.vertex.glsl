// #version 330 core
// layout (location = 0) in vec2 aPos;
// // layout (location = 1) in vec2 aTexCoords;

// out vec2 TexCoords;

// uniform vec3 cameraRigth_Worldspace;
// uniform vec3 cameraUP_Worldspace;

// uniform vec3 constraintPos;
// uniform vec2 constraintSize;

// uniform mat4 VP; // view-projection matrix. model not needed as position is in constraintPos; and orientation given by camera



// void main()
// {
// 	vec3 center_worldspace = constraintPos;

// 	vec3 vertexPosition_worldspace = center_worldspace + cameraRigth_Worldspace* aPos.x * constraintSize.x + cameraUP_Worldspace * aPos.y * constraintSize.y;

// 	//output the position of the vertex
//     gl_Position = VP * vec4(vertexPosition_worldspace, 1.0); 
    
//     TexCoords = aTexCoords;
// } 

#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
	vec3 particleCenter_wordspace = BillboardPos;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * BillboardSize.x
		+ CameraUp_worldspace * squareVertices.y * BillboardSize.y;


	// Output position of the vertex
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);



	// Or, if BillboardSize is in percentage of the screen size (1,1 for fullscreen) :
	//vertexPosition_worldspace = particleCenter_wordspace;
	//gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f); // Get the screen-space position of the particle's center
	//gl_Position /= gl_Position.w; // Here we have to do the perspective division ourselves.
	//gl_Position.xy += squareVertices.xy * vec2(0.2, 0.05); // Move the vertex in directly screen space. No need for CameraUp/Right_worlspace here.
	
	// Or, if BillboardSize is in pixels : 
	// Same thing, just use (ScreenSizeInPixels / BillboardSizeInPixels) instead of BillboardSizeInScreenPercentage.


	// UV of the vertex. No special space for this one.
	UV = squareVertices.xy + vec2(0.5, 0.5);
}
