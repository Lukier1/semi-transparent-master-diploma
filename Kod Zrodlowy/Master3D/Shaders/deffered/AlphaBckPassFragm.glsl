#version 420


layout(binding=0) uniform sampler2D gBackground;
layout(binding=1) uniform sampler2D gAccRGBA;
layout(binding=2) uniform sampler2D gBetaD;
layout(binding=3) uniform sampler2D gAlphaDelta;
layout(binding=4) uniform sampler2D gAlphaObjectDepth;
layout(binding=5) uniform sampler2D gBackgroundDepth;

uniform vec2 texSize;
varying vec2 st;
out vec4 frag_color;

float maxComponent(vec4 el)
{
	return max(max(el.x, el.y), max(el.z, el.w));
}

float maxComponent(vec3 el)
{
	return max(max(el.x, el.y), el.z);
}

float minComponent(vec3 el)
{
	return  min(min(el.x, el.y), el.z);
}

vec3 poissonSampling(vec2 coord, sampler2D texSampler, float power)
{
	vec2 poissonDisk[4] = vec2[](
	  vec2( -0.94201624, -0.39906216 ),
	  vec2( 0.94558609, -0.76890725 ),
	  vec2( -0.094184101, -0.92938870 ),
	  vec2( 0.34495938, 0.29387760 )
	);

	//ADD Acne form angle
	float visibility = 1.0;
	float bias = 0.005;
	vec3 acc = vec3(0,0,0);
	for (int i=0;i<4;i++) {
		acc += texture2D(texSampler, coord+poissonDisk[i]/power).xyz/4;
	}
	
	return acc;
}

void main() {
	vec4 backgroundColor = texture2D(gBackground, st);
	vec4 A = texture2D(gAccRGBA, st);
	vec3 B = texture2D(gBetaD, st).rgb;
	
	if(minComponent(B) >= 1.0)
	{
		frag_color.xyz = backgroundColor.xyz;
		frag_color.w = 1.0;
		return;
	}
	vec2 delta = texture2D(gAlphaDelta, st).xy;
	vec3 bckOut;
	if(delta.x != 0 || delta.y != 0) {
	 	bckOut = texture2D(gBackground, st+delta*0.376).xyz; 
	 }
	 else
	 {
		bckOut = texture2D(gBackground, st).xyz;
	 }

	A.w = max(0.0001, A.w);
	// vec3 U = A.xyz * (vec3(0.5) + 0.5 * B / max(0.01, maxComponent(B))) / max(A.w, 0.00001);
	vec3 U = A.xyz/(A.w) ;
	
	frag_color.xyz = bckOut*B+U*(vec3(1)-B); 

	frag_color.w = 1.0;

	//frag_color.xyz = A.xyz/A.w;
}


