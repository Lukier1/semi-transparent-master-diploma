#version 420

varying vec2 st;

uniform int phase;
out vec4 frag_color;


layout(binding=0) uniform sampler2DMS shadowMapMS;
layout(binding=1) uniform sampler2D flattedShadowMap;
layout(binding=2) uniform sampler2D gPositionMap;
layout(binding=3) uniform sampler2D gDepthMap;

uniform mat4 VPShadowMap;


float lastState = 1;

uint TausStep(uint z, uint S1, uint S2, uint S3, uint M) {
	uint b = (((z << S1) ^ z) >> S2);
	return (((z & M) << S3) ^ b);
}

uint LCGStep(uint z, uint A, uint C)
{
  return A*z+C;
}


uint z1, z2, z3, z4;

void setSeed(vec3 seed) {
	vec3 bigSeed = mod(456789*seed, 1013904223);
	z1 = uint(bigSeed.x);
	z2 = uint(bigSeed.y);
	z3 = uint(bigSeed.z);
	z4 = z3^z2^z1;
}

float HybridTaus() {
	z1 = TausStep(z1, 13, 19, 12, 4294967294);
	z2 = TausStep(z2, 2, 25, 4, 4294967288);
	z3 = TausStep(z3, 3, 11, 17, 4294967280);
	z4 = LCGStep(z4, 1664525, 1013904223);

	return 2.3283064365387e-10 * float(
		z1 ^    // p1=2^31-1
		z2 ^   // p2=2^30-1
		z3 ^   // p3=2^28-1
		z4     // p4=2^32
	);
}

float nextRandFloat() {
	return HybridTaus();
}


vec3 fetchColor(ivec2 coordBase, vec3 comp)
{
	vec3 colorResult = vec3(0,0,0);
	float bias = 0.005;
	for(int i = 0; i < 8; ++i)
	{
	
		colorResult += vec3(greaterThan(texelFetch(shadowMapMS, coordBase, i).xyz, comp))/8.0;
	}

	return colorResult;
}

float manhatanDistance(vec2 vec) {
	return abs(vec.x)+abs(vec.y);
}

float tentWeight(vec2 len, float size)
{
	float l = manhatanDistance(len)/size;
	if(l <= 1) {
		return 1 - l;
	}
	else
	{
		l = 0;
	}
}

vec3 oneSideTentFilter(vec2 coord, vec2 mid, float dist)
{
	float sumW = 0;
	vec3 I = vec3(0,0,0);
	float radius = max(mid.x, mid.y);
	
	for(float i = 0; i < radius*2+1; ++i)
	{
		vec2 move = vec2(0, i);
		if(mid.y == 0) {
			move = vec2(i, 0);	
		}
		float w = tentWeight(move-mid, radius);
		I += texture2D(flattedShadowMap, st+((move-mid)*(dist/70.0))).xyz*w; 
		sumW +=  w;
	}

	I/=sumW;

	return I;
}

ivec2 OFFSETS[13];

void initOffsets() {
	OFFSETS[0] = ivec2(0,0);
	OFFSETS[1] = ivec2(3,3);
	OFFSETS[2] = ivec2(-3,3);
	OFFSETS[3] = ivec2(3,-3);
	OFFSETS[4] = ivec2(-3,-3);
	OFFSETS[5] = ivec2(-4,0);
	OFFSETS[6] = ivec2(4,0);
	OFFSETS[7] = ivec2(0,-4);
	OFFSETS[8] = ivec2(0,4);
	OFFSETS[9] = ivec2(-7,0);
	OFFSETS[10] = ivec2(7,0);
	OFFSETS[11] = ivec2(0,-7);
	OFFSETS[12] = ivec2(0,7);
	
}  

//vec2 jitterFunc(vec2 pos) {
//	setSeed(vec3(pos, 1.0));
	//return vec2(nextRandFloat(), nextRandFloat())*1.5;
	//vec2 up = 5*mod(pos, vec2(2,2)) -  (1,1);

	

	//vec2 down = 6*vec2(	manhatanDistance(vec2(nextRandFloat(), nextRandFloat())),
	//					manhatanDistance(vec2(nextRandFloat(), nextRandFloat())));
	
	//return up/down;
//}

ivec2 getOffset(vec2 pos, int i)
{
	if(i > 13 || i < 0)
	{
		return ivec2(0,0);
	}

	return OFFSETS[i];//+ivec2( jitterFunc(pos));
	
}

void main() {
	initOffsets();

	float dist = max(1-texture2D(gDepthMap, st).x, 0.1);
	
	if(phase == 0) {
		float bias = 0.005;
		vec3 WorldPos = texture2D(gPositionMap, st).xyz;
		vec3 ShadowCoord = (vec4(WorldPos, 1.0)*VPShadowMap).xyz;
		vec2 shadowSize = textureSize(shadowMapMS);
		ivec2 iShadowCoord = ivec2((ShadowCoord.xy)*shadowSize);
		vec3 ShadowOut = vec3(0,0,0);
		for(int i = 0; i < 13; ++i) {
			ShadowOut += fetchColor(iShadowCoord+getOffset(vec2(iShadowCoord), i), vec3(ShadowCoord.z-bias));
		}
		ShadowOut /= 13;
		frag_color = vec4(ShadowOut, 1);	
	} 
	else if(phase == 1) {
		frag_color = vec4(oneSideTentFilter(gl_FragCoord.xy, vec2(0,3), dist), 1.0);
		//frag_color = texture2D(flattedShadowMap, st).xyzw; 
	} else if(phase == 2)  {
		//frag_color = texture2D(flattedShadowMap, st).xyzw; 
		frag_color = vec4(oneSideTentFilter(gl_FragCoord.xy, vec2(3,0), dist), 1.0);
	}
	return;
}
