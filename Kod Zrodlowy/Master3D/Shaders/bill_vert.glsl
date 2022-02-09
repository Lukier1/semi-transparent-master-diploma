#version 400

layout (location = 0) in vec3 vertex_position;

//uniform vec3 direction; 
//uniform vec2 tex_size;
uniform mat4 mvp_mat;


void main () {

	gl_Position = mvp_mat * vec4(vertex_position, 1.0);
};