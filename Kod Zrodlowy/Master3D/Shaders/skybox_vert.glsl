#version 430

in vec3 vp;
uniform mat4 P, V; // Perspective, view

out vec3 texcoords;

void main() {
	//Deleting camera translation 
	mat4 V_ED = V;
	V_ED[0][3] = 0;
	V_ED[1][3] = 0;
	V_ED[2][3] = 0;
	//V_ED[3][0] = 0;
	//V_ED[3][1] = 0;
	//V_ED[3][2] = 0;
	

	//V_ED[0][1] = -V_ED[0][1];
//	V_ED[1][1] = -V_ED[1][1];
	//V_ED[2][1] = -V_ED[2][1];
	//V_ED = mat4(mat3(V));
	texcoords = vp;
	gl_Position = vec4(vp,1.0)*V_ED*P;
}