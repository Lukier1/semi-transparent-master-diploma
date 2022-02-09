#version 420

in vec2 texCoord;

in vec3 positionWorld, positionCamera, positionOut;
in vec3 normalWorld, normalCamera;

out vec4 frag_color;

void main() {
    frag_color = vec4(1.0, 0, 0, 1.0);
}