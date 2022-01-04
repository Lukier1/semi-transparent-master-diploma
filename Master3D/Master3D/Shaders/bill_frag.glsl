#version 400

uniform sampler2D diffuse_texture;
varying vec2 TexCoord0;

out vec4 frag_color;

void main() {
  vec4 texel =  texture2D(diffuse_texture, TexCoord0);
  frag_color = texel;
	//frag_color = vec4(1.0,1.0,1.0,0.5);
}
