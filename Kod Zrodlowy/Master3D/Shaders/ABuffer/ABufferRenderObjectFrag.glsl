#version 430

#extension GL_EXT_shader_image_load_store : enable

coherent uniform layout(binding = 0, r32ui) uimage2D counterImage;
coherent uniform layout(binding = 1, rgba32f) image2DArray bufferImage;
coherent uniform layout(binding = 3, r32f) image2DArray alhpaBufferImage;
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
			float ISpec = pow(clamp(dot(reflect(lightDir, normal),camDir),0,2),50);
			colOut += clamp(dirLightColor[i]*vec3(power)*diffColor+ISpec*vec3(1,1,1)*3, vec3(0,0,0), vec3(1,1,1));
		}
	}
	
	return colOut;
	
}


void main() {
	vec4 fragColor = isTextured != 0 ? texture2D(diffuse_texture, texCoord) : vec4(color,1.0);
	float fragAlpha = isTextured != 0 ? fragColor.w*alpha : alpha;
    fragColor  =  vec4(CalcDirLight(normalWorld, fragColor.xyz) + ambientLight*fragColor.xyz,1);

	ivec2 coords=ivec2(gl_FragCoord.xy);
    uint aBufferInd = imageAtomicAdd(counterImage, coords, 1);
    imageStore(bufferImage, ivec3(coords, aBufferInd), vec4(fragColor.xyz, gl_FragCoord.z/gl_FragCoord.w));
	imageStore(alhpaBufferImage, ivec3(coords, aBufferInd), vec4(fragAlpha));
    discard;
}