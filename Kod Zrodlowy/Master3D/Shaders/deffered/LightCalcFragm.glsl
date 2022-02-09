#version 420

layout(binding=0) uniform sampler2D gPositionMap;
layout(binding=1) uniform sampler2D gColorMap;
layout(binding=2) uniform sampler2D gNormalMap;
layout(binding=3) uniform sampler2D gTexCoordMap;
layout(binding=4) uniform sampler2D gDepthMap;
layout(binding=5) uniform sampler2D gShadowColorMap;
layout(binding=6) uniform samplerCube skyBoxTexture;

in vec3 texcoords;
out vec4 FragColor;

const int LIGHT_NUM = 3;
const int MAX_LIGHTS_NUM = 128;

uniform mat4 VPShadowMap;

uniform unsigned int lightNumber;
uniform vec3 lightsPositions[MAX_LIGHTS_NUM];
uniform vec3 lightsColors[MAX_LIGHTS_NUM];
uniform float lightsPowers[MAX_LIGHTS_NUM];

uniform unsigned int dirLightNumber;
uniform vec3 dirLightDirection[MAX_LIGHTS_NUM];
uniform vec3 dirLightColor[MAX_LIGHTS_NUM];

uniform vec3 ambientLight;

uniform vec3 cameraPosition;

uniform vec2 screen;

vec3 lightPos = vec3(0,0,0);

vec4 CalcPointLight(vec3 worldPos, vec3 normal)
{
	vec3 colOut;
	for(int i = 0; i < lightNumber; ++i) { 
		vec3 lightVec = lightsPositions[i] - worldPos;
		float dist = length(lightVec);
		vec3 lightDir = normalize(lightVec);
		
		float power = dot(normal, lightDir);
		if(power > 0) {
			colOut += lightsColors[i]*lightsPowers[i]/exp(dist/lightsPowers[i]);
		}
	}
	
	return vec4(colOut,1.0);
	
}

// vec4 CalcDirLight2(vec3 worldPos, vec3 normal)
// {
// 	vec3 colOut;
// 	for(int i = 0; i < dirLightNumber; ++i) { 
// 		vec3 lightDir = -dirLightDirection[i];
// 		float power = dot(normal, lightDir);
// 		if(power > 0) {
// 			//TODO TO CHANGE
// 			vec3 incidenceVector = -dirLightDirection[i]; //a unit vector
// 			vec3 reflectionVector = reflect(incidenceVector, normal); //also a unit vector
// 			vec3 surfaceToCamera = normalize(cameraPosition - worldPos); //also a unit vector
// 			float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
// 			float specularCoefficient = pow(cosAngle, 100.0);

// 			vec3 specularComponent = specularCoefficient * vec3(1,1,1) * power; 
// 			float ISpec = pow(clamp(dot(reflect(incidenceVector, normal),surfaceToCamera),0,2),50);
// 			colOut += dirLightColor[i]*power+ISpec*vec3(1,1,1);
// 		}

// 	}
	
// 	return vec4(colOut, 1.0);
	
// }

vec4 CalcDirLight(vec3 worldPos, vec3 normal)
{
	vec3 diffColor = vec3(1,1,1);
	vec3 camDir = -normalize(cameraPosition-worldPos);

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
	
	return vec4(colOut, 1.0);
}




void main()
{
	FragColor = vec4(1, 0, 0, 1);
	//return; 

	vec2 st = vec2(gl_FragCoord.x/screen.x, gl_FragCoord.y/screen.y);

	float Depth = texture2D(gDepthMap, st).x;
	
	if(Depth == 1.0)
	{
	//TODO FOR TESTS
		//FragColor = texture (skyBoxTexture, -texcoords);
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		return;
	}
	
   	vec2 TexCoord = texture2D(gTexCoordMap, st).xy;
   	vec3 WorldPos = texture2D(gPositionMap, st).xyz;
   	vec3 Color = texture2D(gColorMap, st).xyz;
   	vec3 Normal = texture2D(gNormalMap, st).xyz;
   	Normal = normalize(Normal);

	//##################################
	//Shadow
	vec3 I=texture2D(gShadowColorMap, st).xyz;
    //vec3 I = vec3(1.0, 1.0, 1.0);
	//FragColor = vec4(Normal,1.0);
	//FragColor = vec4(-dirLightDirection[0], 1.0);
	//FragColor = vec4(Color,1.0);
	FragColor = vec4(Color,1.0)*(vec4(ambientLight,1.0) + CalcPointLight(WorldPos, Normal) + vec4(I,.0)  * CalcDirLight(WorldPos, Normal));

	
}

