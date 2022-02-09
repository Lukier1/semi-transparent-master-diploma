#version 420


uniform vec3 transmitive; // TODO Transmitive to TEX or boolean to choose


uniform sampler2D diffuse_texture;
uniform int isTextured;

uniform float alpha;
uniform float eta;
in vec3 positionWorld;
in vec3 normalCamera;
in vec2 texCoord;

out vec4 frag_color;


uint z1, z2, z3, z4;

uint TausStep(uint z, uint S1, uint S2, uint S3, uint M) {
	uint b = (((z << S1) ^ z) >> S2);
	return (((z & M) << S3) ^ b);
}

uint LCGStep(uint z, uint A, uint C)
{
  return A*z+C;
}

void setSeed(vec3 seed) {
	vec3 bigSeed = mod(456789*seed, 1013904223);
	z1 = 128+uint(bigSeed.x);
	z2 = 128+uint(bigSeed.y);
	z3 = 128+uint(bigSeed.z);
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




float calcProbalibityForOneChannel(float t)
{
	const float s = clamp((1/eta  - 1)/2.0, 0, 1);
	const float g = 2 * clamp(1-abs(pow(dot(normalCamera, vec3(0,0,1)), 128*s*s)), 0, 1)-1;
	const float p = min(1, 1 + g*pow(s, 0.2))*alpha*(1-t);
	return p;
}

vec3 calcProbilityForAllChannel(vec3 t)
{
	vec3 p;
	p.x = calcProbalibityForOneChannel(t.x);
	p.y = calcProbalibityForOneChannel(t.y);
	p.z = calcProbalibityForOneChannel(t.z);
	return p;
}

//TODO REFACTOR!!!!
void main() {
	setSeed(positionWorld.xyz);
	vec3 transmitiveColor = isTextured != 0 ? texture2D(diffuse_texture, texCoord).xyz : transmitive;
	vec3 prob = calcProbilityForAllChannel(transmitiveColor);
	vec3 sigma = vec3(nextRandFloat(), nextRandFloat(), nextRandFloat());
	vec3 resultProb = vec3(lessThan(prob, sigma));
	
	vec3 resultOut;
	resultOut.x = max(gl_FragCoord.z, resultProb.x);
	resultOut.y = max(gl_FragCoord.z, resultProb.y);
	resultOut.z = max(gl_FragCoord.z, resultProb.z);

	//resultOut.xyz = vec3(sigma);
	frag_color = vec4(resultOut, 1.0);  	
}
