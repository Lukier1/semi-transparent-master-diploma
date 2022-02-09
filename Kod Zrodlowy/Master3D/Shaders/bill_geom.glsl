#version 400

layout (points) in;

layout (triangle_strip, max_vertices = 4) out;

varying out vec2 TexCoord0;

uniform mat4 P_mat;

void main() {
	vec4 size = vec4(vec3(P_mat*vec4(1.0, 1.0, 0, 1)),0);
	for(int i = 0; i < gl_in.length (); ++i)
	{
		gl_Position = gl_in[i].gl_Position+size;
		TexCoord0 = vec2(0,0);
		EmitVertex();
		
		gl_Position.x = gl_in[i].gl_Position.x + size.x;
		gl_Position.y = gl_in[i].gl_Position.y - size.y;
		TexCoord0 = vec2(1,0); 
		EmitVertex();

		gl_Position.y = gl_in[i].gl_Position.y + size.y;
		gl_Position.x = gl_in[i].gl_Position.x - size.x; 
		TexCoord0 = vec2(0,1);
		EmitVertex();
		
		gl_Position.y = gl_in[i].gl_Position.y - size.y;
		gl_Position.x = gl_in[i].gl_Position.x - size.x;
		TexCoord0 = vec2(1,1); 
		EmitVertex();
	}
}