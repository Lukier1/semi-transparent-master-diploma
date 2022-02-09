#version 430

#extension GL_EXT_shader_image_load_store : enable

layout(binding=10) uniform sampler2D custom_alpha;

in vec2 texCoord;

in vec3 positionWorld, positionCamera, positionOut;
in vec3 normalWorld, normalCamera;

layout (location = 0) out vec4 out1; 
layout (location = 1) out vec4 out2; 
layout (location = 2) out vec4 out3; 
layout (location = 3) out vec4 out4; 
layout (location = 4) out vec4 out5; 
layout (location = 5) out vec4 out6; 
layout (location = 6) out vec4 out7; 
layout (location = 7) out vec4 out8;

coherent restrict uniform layout(binding = 8, r32ui) uimage2D atomicImage;

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

uniform mat4 VPShadowMap;

uniform sampler2D diffuse_texture;
uniform int isTextured;
uniform vec3 color;
uniform vec3 cameraPosition;
uniform mat4 PMat;
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

void saveToOut(vec4 color, int layer)
{
    switch(layer) {
    case 0:
        out1 = color;
        break;
        case 1:
        out2 = color;
        break;
        case 2:
        out3 = color;
        break;
        case 3:
        out4 = color;
        break;
        case 4:
        out5 = color;
        break;
        case 5:
        out6 = color;
        break;
        case 6:
        out7 = color;
        break;
        case 7:
        out8 = color;
        break;
    }
}
void main() {
	
	vec4 fragColor = isTextured != 0 ? texture2D(diffuse_texture, texCoord) : vec4(color,1.0);
	fragColor  =  vec4(CalcDirLight(normalWorld, fragColor.xyz) + ambientLight*fragColor.xyz,1);
    vec2 viewPortSize =vec2(1280, 720);
    vec2 uniTex = gl_FragCoord.xy/viewPortSize;
    vec2 miniMax = vec2(texture2D(custom_alpha, uniTex));
   
    float minZ = -miniMax.x;
    float maxZ  = miniMax.y;
	float dep = maxZ - minZ;
 
    float pos = (gl_FragCoord.z/gl_FragCoord.w-minZ)/dep;
    int index =  int(floor(pos*7.0));
	//int index =  int(floor(pow(8, pos)));
	//	int index =  int(floor(sin(pos*3.14/2)*7.5));
    saveToOut(vec4(fragColor.xyz, 0.8), index);
	//saveToOut(vec4(vec3(minZ), 0.5), index);


	//Atomic test
	vec2 vec = imageSize(atomicImage);
	saveToOut(vec4(vec,  1.0, 1.0), index); 
	saveToOut(vec4(imageLoad(atomicImage, ivec2(gl_FragCoord.xy)).x/1000.0f, 0.0, 0.0, 1.0), index);
	imageAtomicAdd(atomicImage, ivec2(gl_FragCoord.xy), 1);
	//imageStore(atomicImage, ivec2(gl_FragCoord.xy), vec4(1, 0, 1, 1.0));
    
}