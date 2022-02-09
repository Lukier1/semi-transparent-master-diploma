#version 420


layout (location = 0) out vec2 posOut; 

uniform mat4 MMat;
void main() {
	float dep = gl_FragCoord.z/gl_FragCoord.w;
	posOut = vec2(-dep, dep);
}
