#version 430

#extension GL_EXT_shader_image_load_store : enable

coherent restrict uniform layout(binding = 0, r32ui) uimage2D counterImage;
coherent restrict uniform layout(binding = 1, rgba32f) image2DArray bufferImage;
coherent uniform layout(binding = 3, r32f) image2DArray alhpaBufferImage;

in vec2 st;

out vec4 frag_color;

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

void fillFragmentArray(ivec2 coords, uint abNumFrag) {
	for(int i=0; i<abNumFrag; i++){
    	vec4 fromBufferImage = imageLoad(bufferImage, ivec3(coords, i));
		fragmentList[i].color.rgb = fromBufferImage.rgb;
		fragmentList[i].color.a = imageLoad(alhpaBufferImage, ivec3(coords, i)).r;
		fragmentList[i].z = fromBufferImage.w;
	}
}

//Swap function
void swapFragArray(int n0, int n1){
	FragmentDesc temp = fragmentList[n1];
	fragmentList[n1] = fragmentList[n0];
	fragmentList[n0] = temp;
}


void bubbleSort(int array_size) {
  for (int i = (array_size - 2); i >= 0; --i) {
    for (int j = 0; j <= i; ++j) {
      if (fragmentList[j].z > fragmentList[j+1].z) {
        swapFragArray(j, j+1);
      }
    }
  }
}



vec4 resolve(ivec2 coords, uint abNumFrag) {
    fillFragmentArray(coords, abNumFrag);
     
    if(abNumFrag > 1 && abNumFrag < ABUFFER_SIZE_M) {
        bubbleSort(int(abNumFrag));
    }
  
    vec4 result = vec4(0,0,0,0);
    
    for(int i = 0; i < int(abNumFrag) ; ++i) {
       vec4 frag = vec4(fragmentList[i].color);
       frag.xyz *= frag.w;
       result = result + (1.0f-result.w)*frag; 
    }

   return vec4(result.xyz+vec3(1)*(1-result.w),1);
}
