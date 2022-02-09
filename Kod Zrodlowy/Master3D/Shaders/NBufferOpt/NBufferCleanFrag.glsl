#version 450

#extension GL_EXT_shader_image_load_store : enable

coherent restrict uniform layout(binding = 0, r32ui) uimage2D counterImage;
coherent restrict uniform layout(binding = 1, rgba32f) image2DArray bufferImage;
coherent uniform layout(binding = 3, r32f) image2DArray alhpaBufferImage;
coherent restrict uniform layout(binding = 2, r32i) iimage2D lockImage;

in vec2 st;

out vec4 frag_color;
const int ABUFFER_SIZE_M = 32;
void main() {
    for(int i = 0; i < ABUFFER_SIZE_M; ++i) {
        imageStore(bufferImage, ivec3(gl_FragCoord.xy, i), vec4(0.0, 0.0, 0.0, 10000));
        imageStore(alhpaBufferImage, ivec3(gl_FragCoord.xy, i), vec4(1.0));
    }
    
    imageStore(counterImage, ivec2(gl_FragCoord.xy), uvec4(0));
    imageStore(lockImage, ivec2(gl_FragCoord.xy), ivec4(0));

    discard;
}