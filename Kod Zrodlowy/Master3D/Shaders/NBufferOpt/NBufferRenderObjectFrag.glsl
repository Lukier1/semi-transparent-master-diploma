#version 430

#extension GL_EXT_shader_image_load_store : enable

coherent uniform layout(binding = 0, r32ui) uimage2D counterImage;
coherent uniform layout(binding = 1, rgba32f) image2DArray bufferImage;
coherent uniform layout(binding = 3, r32f) image2DArray alhpaBufferImage;
coherent restrict uniform layout(binding = 2, r32i) iimage2D lockImage;

in vec2 texCoord;

in vec3 positionWorld, positionCamera, positionOut;
in vec3 normalWorld, normalCamera;

uniform int ind;

const int MAX_LIGHTS_NUM = 128;

uniform unsigned int lightNumber;
uniform vec3 lightsPositions[MAX_LIGHTS_NUM];
uniform vec3 lightsColors[MAX_LIGHTS_NUM];
uniform float lightsPowers[MAX_LIGHTS_NUM];

uniform unsigned int dirLightNumber;
uniform vec3 dirLightDirection[MAX_LIGHTS_NUM];
uniform vec3 dirLightColor[MAX_LIGHTS_NUM];

uniform vec3 ambientLight;

uniform sampler2D diffuse_texture;
uniform int isTextured;
uniform vec3 color;
uniform vec3 cameraPosition;
uniform mat4 PMat;
uniform float alpha;
uniform float eta;
uniform vec3 transmitive;

uniform int BUFFER_SIZE;



struct FragmentDesc
{
  vec4 color;
  float z;
};

FragmentDesc fragmentList[32];

void fillFragmentArray(ivec2 coords, uint abNumFrag);

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


void criticalSection(vec4 fragColor, float fragAlpha, ivec2 coords);

void main() {

	


// 	//From k-buffer 2014 spin-lock
// 	ivec2 coords=ivec2(floor(gl_FragCoord.xy));
		
//     vec4 fragColor = isTextured != 0 ? texture2D(diffuse_texture, texCoord) : vec4(color,1.0);
// 	float fragAlpha = isTextured != 0 ? fragColor.w*alpha : alpha;
//     fragColor  =  vec4(CalcDirLight(normalWorld, fragColor.xyz) + ambientLight*fragColor.xyz,1);

//     while(true) {
	
//         if(imageAtomicExchange(lockImage, coords, 1) == 0) {
// 	       //Monitor-section
// 		   	criticalSection(fragColor, fragAlpha, coords);
//         	imageStore(lockImage, coords, ivec4(0));
// 			discard;
// 		}
//     }

//   discard;
}



void storeFragmentArray(ivec2 coords, uint abNumFrag);

void criticalSection(vec4 fragColor, float fragAlpha, ivec2 coords) {
	
    
	//N-Bufffer algorithm
	FragmentDesc temp;
	FragmentDesc merge;
	FragmentDesc frag;
	
	float fragTransm = (1-fragAlpha);
	//frag.color = vec4(fragColor.xyz, fragAlpha);
	frag.color =vec4(fragAlpha*fragColor.xyz, fragTransm);
	
	frag.z = gl_FragCoord.z/gl_FragCoord.w;

	uint aBufferInd = imageAtomicAdd(counterImage, coords, 1);
	fillFragmentArray(coords, 5);


	bool isLastSwap = false;
	int insertInd = -1; 
	for(int ind = 0; ind < 4; ind++) {
		if (frag.z <= fragmentList[ind].z && insertInd == -1) {
			insertInd = ind;
		}
	}


	if(insertInd == -1) {
		fragmentList[4] = frag;
	} else {
		for(int i = 4; i > insertInd; i--) {
			fragmentList[i] = fragmentList[i-1];
		}
		fragmentList[insertInd] = frag;
	}

		// O jeden za daleko fragment
	if (fragmentList[BUFFER_SIZE].color.a != 1) {
		FragmentDesc first = fragmentList[3];
		FragmentDesc second = fragmentList[4];
		if(insertInd == -1) {
			first = fragmentList[4];
			second = fragmentList[3];
		}
		//merge.color.a =  (1-first.color.a)*(1-second.color.a);
		merge.color.a =  first.color.a*second.color.a;
		
		merge.color.rgb = first.color.rgb+first.color.a*second.color.rgb;
		

		merge.z = min(first.z, second.z);
		fragmentList[BUFFER_SIZE-1] = merge;

		//-----------
	}
	
	storeFragmentArray(coords, 4);
}




void fillFragmentArray(ivec2 coords, uint abNumFrag) {

	for (int i = 0; i < abNumFrag; i++) {
		vec4 fromBufferImage = imageLoad(bufferImage, ivec3(coords, i));
		fragmentList[i].color.rgb = fromBufferImage.rgb;
		fragmentList[i].color.a = imageLoad(alhpaBufferImage, ivec3(coords, i)).r;
		fragmentList[i].z = fromBufferImage.w;
		
	}
}

void storeFragmentArray(ivec2 coords, uint abNumFrag) {
	for (int i = 0; i < abNumFrag; i++) {
		imageStore(bufferImage, ivec3(coords, i), vec4(fragmentList[i].color.rgb, fragmentList[i].z));
		imageStore(alhpaBufferImage, ivec3(coords, i), vec4(fragmentList[i].color.a));
	}
}

