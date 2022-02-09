#version 420

layout(binding=0) uniform sampler2D texAlphaObjectColor;
layout(binding=1) uniform sampler2D texAccAlpha;

varying vec2 st;

out vec4 frag_color;

void main() {
    vec4 accAlpha = texture2D(texAccAlpha, st);
    vec4 alphaObjectColor = texture2D(texAlphaObjectColor, st);
    frag_color = vec4(alphaObjectColor.rgb, accAlpha.x);
    //frag_color = vec4(alphaObjectColor.rgb, 1);
    //frag_color = vec4(1-accAlpha.x, 0, 0, 1);
}