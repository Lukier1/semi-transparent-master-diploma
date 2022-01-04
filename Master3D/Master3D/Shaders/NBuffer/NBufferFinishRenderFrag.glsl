#version 430

#extension GL_EXT_shader_image_load_store : enable

coherent restrict uniform layout(binding = 0, r32ui) uimage2D counterImage;
coherent restrict uniform layout(binding = 1, rgba32f) image2DArray bufferImage;
coherent uniform layout(binding = 3, r32f) image2DArray alhpaBufferImage;

in vec2 st;

out vec4 frag_color;

uniform int BUFFER_SIZE;

const int ABUFFER_SIZE_M = 32;

struct FragmentDesc
{
  vec4 color;
  float z;
};

FragmentDesc fragmentList[ABUFFER_SIZE_M];

void fillFragmentArray(ivec2 coords, uint abNumFrag);
vec4 resolve(ivec2 coords, uint abNumFrag);

void main() {
  uint counterValue = imageLoad(counterImage, ivec2(gl_FragCoord.xy)).x;
   
	frag_color = resolve(ivec2(gl_FragCoord.xy), counterValue);
}
// Z przykaldu a-buffera kod
void fillFragmentArray(ivec2 coords, uint abNumFrag) {
	for(int i=0; i< BUFFER_SIZE; i++){
        vec4 fromBufferImage = imageLoad(bufferImage, ivec3(coords, i));
        fragmentList[i].color.rgb = fromBufferImage.rgb;
        fragmentList[i].z = fromBufferImage.w;
        fragmentList[i].color.a = imageLoad(alhpaBufferImage, ivec3(coords, i)).r;
	}
}


float loadAccWeight(ivec2 coords)
{
	return imageLoad(bufferImage, ivec3(coords, 31)).x;
}

vec4 resolve(ivec2 coords, uint abNumFrag) {
    fillFragmentArray(coords, abNumFrag);
     
    vec4 result = vec4(0,0,0,1);
   // float accumWeight = loadAccWeight(coords);
    
    float accumOcc = 1; 
    for(int i = 0; i < min(BUFFER_SIZE, abNumFrag); ++i) {
       vec4 frag = vec4(fragmentList[i].color.rgba);
       //frag.xyz *= frag.w;
       accumOcc *= frag.w;
       result.xyz = result.xyz + result.w*frag.xyz; 
       result.w = result.w*frag.w;
    }

    //result = vec4(fragmentList[0].color.r, 0,0,1);
   //result.xyz = vec3(1)*(1-result.w) + result.xyz;
   //int last = (BUFFER_SIZE-1) < abNumFrag ? BUFFER_SIZE-1 : int(abNumFrag-1);
   //result.xyz = vec3(fragmentList[0].color.rgb);
   //accumOcc = (1.0 - fragmentList[0].color.a);
  //  result.xyz = vec3(BUFFER_SIZE);
   //result.xyz = vec3(accumWeight/10);
   
   //return vec4(result.z*100,0.5,1,1);
   // return vec4(abNumFrag)/5.0;
  return vec4(result.xyz,1.0)+accumOcc*vec4(1,1, 1,0);
}


