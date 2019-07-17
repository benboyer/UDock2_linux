#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 TexCoord;
uniform vec2 windows_size;

void build_quad(vec4 position)
{    

	float screenRatio = windows_size.x/windows_size.y;
	float size = 1.7f;//carbon wan der waals radius angstrom
	float rsize = size * screenRatio ;

    gl_Position = position + vec4(-size, -rsize, 0.0f, 0.0f);    // 1:bottom-left
    // TexCoord = vec2(0.0, 0.0);
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();   
    gl_Position = position + vec4( size, -rsize, 0.0f, 0.0f);    // 2:bottom-right
    // TexCoord = vec2(0.0, 1.0);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();
    gl_Position = position + vec4(-size,  rsize, 0.0f, 0.0f);    // 3:top-left
    // TexCoord = vec2(1.0, 0.0);
    TexCoord = vec2(0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4( size,  rsize, 0.0f, 0.0f);    // 4:top-right
    // TexCoord = vec2(1.0, 1.0);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_quad(gl_in[0].gl_Position);
}  