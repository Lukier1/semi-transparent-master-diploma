#version 400

varying vec2 st;
out vec4 frag_color;

layout (binding = 0) uniform sampler2D  bck;
layout (binding = 1) uniform sampler2D  in1; 
layout (binding = 2) uniform sampler2D  in2; 
layout (binding = 3) uniform sampler2D  in3; 
layout (binding = 4) uniform sampler2D  in4; 
layout (binding = 5) uniform sampler2D  in5; 
layout (binding = 6) uniform sampler2D  in6; 
layout (binding = 7) uniform sampler2D  in7; 
layout (binding = 8) uniform sampler2D  in8;

vec4 getIn(int layer)
{
    switch(layer) {
    case 0:
        return texture2D(in1, st);        
    case 1:
        return texture2D(in2, st);
    case 2:
        return texture2D(in3, st);
    case 3:
        return texture2D(in4, st);
    case 4:
        return texture2D(in5, st);
    case 5:
        return texture2D(in6, st);
    case 6:
        return texture2D(in7, st);
    case 7:
        return texture2D(in8, st);

    }
}
void main() {
    frag_color = texture2D(bck, st);
  
    for(int i = 7; i >= 0; --i) {
	 float alpha_src = getIn(i).w;
	 vec3 color_src = getIn(i).xyz;
      
	  // float a0 = frag_color.w + (1-frag_color.w)*getIn(i).w; 
	  // vec3 c0 = frag_color.xyz*frag_color.w + (1-frag_color.w)*getIn(i).xyz*getIn(i).w); 
	  
	   float a0 = 1; //alpha_src + (1-alpha_src)*frag_color.w; 
	   vec3 c0 = color_src + (1-alpha_src)*frag_color.xyz; 
	   
	   //a0 = clamp(a0, 0, 1);
       c0 = clamp(c0, 0, 1);
	   frag_color = vec4(c0, a0);
		//frag_color += getIn(i);
	}
	//frag_color = vec4(getIn(0).r);
}
