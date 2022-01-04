#version 420

in vec3 positionWorld, positionCamera, positionOut;
//TODO usunac niepottrzebne
in vec3 normalWorld, normalCamera;
in vec2 texCoord;

out vec4 alphaAcc;
layout(binding=2) uniform sampler2D gBackgroundDepth;
layout(binding=10) uniform sampler2D gShadowMap;

layout (location = 0) out vec4 alphaAccRGBA; 
layout (location = 1) out vec4 alphaBckD; 
layout (location = 2) out vec2 alphaDelta;
 
const int MAX_LIGHTS_NUM = 128;



uniform unsigned int lightNumber;
uniform vec3 lightsPositions[MAX_LIGHTS_NUM];
uniform vec3 lightsColors[MAX_LIGHTS_NUM];
uniform float lightsPowers[MAX_LIGHTS_NUM];

uniform unsigned int dirLightNumber;
uniform vec3 dirLightDirection[MAX_LIGHTS_NUM];
uniform vec3 dirLightColor[MAX_LIGHTS_NUM];

uniform vec3 ambientLight;

uniform mat4 VPShadowMap;

uniform sampler2D diffuse_texture;
uniform int isTextured;
uniform vec3 color;
uniform vec3 cameraPosition;
uniform mat4 PMat;
// float alpha = 0.6;
uniform float alpha;
uniform float eta;
uniform vec3 transmitive;

float weight;

vec2 refractOffset(vec3 n, vec3 X, float eta)
{
	float deltaZ = 2; 
	vec3 w0 = -normalize(X);
	n = normalize(n);
	
	float c = dot(n,w0);
	
	if(c < 0)
	{
		return vec2(0,0);
	}

	float k = 1 - eta*eta*(1-c*c);
	
	if(k < 0.0)
	{
	
	}
	vec3 wi = -eta*w0 - ( eta - c	+ sqrt(k)) * n;
	//vec3 wi = -eta*w0 + ( eta * c - sqrt(k)) * n;
	wi = normalize(wi);
	
	float proj =  dot(wi,  -vec3(0,0,1));
	
	vec3 B = X + wi*deltaZ/proj;
	vec3 predX = X - w0*deltaZ/dot(-w0, -vec3(0,0,1));

	return (PMat*vec4(B,1.0)).xy - (PMat*vec4(predX,1.0)).xy; 
}


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

void discardWhenIsCovered() {
	if(gl_FragCoord.z > texture2D(gBackgroundDepth, vec2(gl_FragCoord.x/1280, gl_FragCoord.y/720)).x)
	{
		discard;
	}
}

void main() {
	//TODO Dodac wysylanie rozdzielczo�ci

	discardWhenIsCovered();
	
	vec4 fragColor = isTextured != 0 ? texture2D(diffuse_texture, texCoord) : vec4(color,1.0);
	
	//LIGHT PART
	vec3 Lr;
	vec4 ShadowCoord = (vec4(positionWorld, 1.0)*VPShadowMap);
	vec3 projCoords = ShadowCoord.xyz / ShadowCoord.w; 		
	//projCoords = projCoords * 0.5 + 0.5;
	float IShadow =  texture2D(gShadowMap, projCoords.xy).x < gl_FragCoord.z ? 1 : 0;
	Lr  = calcPointLight(normalWorld, fragColor.xyz)+CalcDirLight(normalWorld, fragColor.xyz)*IShadow +ambientLight*fragColor.xyz;



	vec3 camDir = -normalize(cameraPosition-positionWorld);
	float normalCoeficient = dot(normalWorld, camDir);

	//vec3 Transmitive = frag_color.xyz;
	vec3 t = fragColor.xyz*(1.0-Lr);
	//vec3 t = transmitive*(1-Lr);
	//Transimssion
	//Brakuje tutaj mnozenia przez 1/3

	
	float netCoverage = alpha*(1.0 - dot(t, vec3(1.0/3.0))); 
	 
	weight = alpha*clamp(pow(10.0*(1.0-0.99*gl_FragCoord.z)*netCoverage, 3), 0.1, 30.0);
	
	//Color of object

	alphaAccRGBA = vec4(Lr*alpha, netCoverage)*weight; 

	alphaBckD.xyz = alpha * (vec3(1.0) - t); // [ alpha*(vec3(1)-t)] ->  [alpha * (vec3(1.0) - t) * (1.0 / 3.0);]
	alphaBckD.w = 0.0f;

	//Weighted 
//	weight = alpha*clamp(pow(gl_FragCoord.z, -20), 0.1, 30);
	float absZ = abs(gl_FragCoord.z/gl_FragCoord.w);
	//weight = alpha*clamp( (3000 * pow(1- gl_FragCoord.z, 3)), 0.1, 3000); //W10
	//weight = alpha*clamp((10/(0.00001+pow(absZ/5,2)+pow(absZ/200, 6))), 0.01, 3000); // W7
	weight = alpha*clamp((0.03/(0.00001+pow(absZ/200, 4))), 0.01, 3000); //W9???
	
	alphaAccRGBA = vec4(Lr*alpha, alpha)*weight;  //brakuje alpha Lr -> Lr*alpha

	alphaBckD.xyz = vec3(alpha);
	

	//Difraction
	if(eta != 1.0) {
		vec2 refractPix = refractOffset(normalCamera, positionCamera, eta);
		alphaDelta = refractPix* netCoverage*1/4;
	}
	else {
		alphaDelta = vec2(0,0);
	}
}
