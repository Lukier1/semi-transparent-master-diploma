#version 430

out float frag_color;

uniform float alpha;

void main() {
 	frag_color = alpha;
}
