#version 420

in vec2 st;
out vec4 frag_color;



layout(binding=4) uniform sampler2D color1;

void main() {
    frag_color = vec4(texture(color1, st).xyzw);
    //frag_color = vec4(0.5, 0, 0, 1.0);
}