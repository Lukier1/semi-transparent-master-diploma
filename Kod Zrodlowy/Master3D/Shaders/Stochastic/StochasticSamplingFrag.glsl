#version 430

in vec2 texCoord;
in vec3 positionWorld, positionCamera, positionOut, positionModel;
in vec3 normalWorld, normalCamera;

out vec4 frag_color;

uniform sampler2D diffuse_texture;
uniform float alpha;
uniform int isTextured;
uniform vec3 color;

//LIGTS
const int MAX_LIGHTS_NUM = 128;

uniform unsigned int lightNumber;
uniform vec3 lightsPositions[MAX_LIGHTS_NUM];
uniform vec3 lightsColors[MAX_LIGHTS_NUM];
uniform float lightsPowers[MAX_LIGHTS_NUM];

uniform unsigned int dirLightNumber;
uniform vec3 dirLightDirection[MAX_LIGHTS_NUM];
uniform vec3 dirLightColor[MAX_LIGHTS_NUM];

uniform vec3 ambientLight;

uniform vec3 cameraPosition;
//###################

void setSeed(vec3 seed);
float nextRandFloat();
void initialize_and_permute(int n);
int calcR(float alpha);
int permutation[64];
vec3 calcPointLight (vec3 normal, vec3 diffColor);
vec3 CalcDirLight(vec3 normal, vec3 diffColor);


int sampleMask[1];


void fillSampleMask(int R) {
	
	sampleMask[0] = 0;
	for(int i = 0; i < R; ++i) {
		sampleMask[0] += int(pow(2,permutation[i]));
	}

	for(int i = 0; i < 1; ++i) {
		gl_SampleMask[i] = sampleMask[i];
	}
}

void main() {

    setSeed(positionModel.xyz);
	int R =  calcR(alpha);
	

	initialize_and_permute(gl_NumSamples);

	fillSampleMask(R);
	
	//frag_color = vec4(0,alpha,0, 0.1);
	frag_color = isTextured != 0 ? texture2D(diffuse_texture, texCoord) : vec4(color,1.0);
 	frag_color  =  vec4(CalcDirLight(normalWorld, frag_color.xyz) + ambientLight*frag_color.xyz, alpha);
}

//###################################
//Lights
vec3 calcPointLight (vec3 normal, vec3 diffColor)
{
	//vec3 Lr = (color)*min(0.01+clamp(dot(lightDir, normalWorld),0,1),1);

	vec3 colOut;
	vec3 CD = normalize(cameraPosition-positionWorld);
	for(int i = 0; i < lightNumber; ++i) { 
		vec3 lightDir = lightsPositions[i] - positionWorld;
		float dist = length(lightDir);
		float power = dot(normal, lightDir);
		
		//float ISpec = pow(clamp(dot(reflect(lightDir, normal),CD),0,1),10);
		colOut += lightsColors[i]*lightsPowers[i]*diffColor/exp(dist/lightsPowers[i]);	
	}
	
	return colOut;
}

vec3 CalcDirLight(vec3 normal, vec3 diffColor)
{
	vec3 camDir = -normalize(cameraPosition-positionWorld);

	vec3 colOut;
	for(int i = 0; i < dirLightNumber; ++i) { 
		vec3 lightDir = -dirLightDirection[i];
		float power = dot(normal, lightDir);
		if(power > 0) {
			//colOut += dirLightColor[i]*power;
			float ISpec = pow(clamp(dot(reflect(lightDir, normal),camDir),0,2),50);
			colOut += clamp(dirLightColor[i]*vec3(power)*diffColor+ISpec*vec3(1,1,1)*3, vec3(0,0,0), vec3(1,1,1));
		}
	}
	
	return colOut;
	
}



//#######################
//Checking if the sample belong to suset

int calcR(float alpha) {
	return int(ceil(alpha*gl_NumSamples + nextRandFloat()));
}


void initialize_and_permute(int n)
{
    int i = 0;
	for (i = 0; i < 16; ++i) {
		permutation[i] = i;
	}
    for (i = 0; i <= n-2; i++) {
        int j = i+int(nextRandFloat()*(n-i));
		int temp = permutation[i];
		permutation[i] = permutation[j];
		permutation[j] = temp;
    }
}


//########################
//RANDOM GENERATOR

//RANDOM STATE
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
