#version 430


in vec2 texCoord;
in vec3 positionWorld, positionCamera, positionOut;
in vec3 normalWorld, normalCamera;

out vec4 frag_color;

layout(binding=3) uniform sampler2DMS colorMapMS;
layout(binding=4) uniform sampler2DMS depthMapMS;
layout(binding=5) uniform sampler2D alphaAcc;

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

vec3 calcPointLight (vec3 normal, vec3 diffColor);
vec3 CalcDirLight(vec3 normal, vec3 diffColor);


float calcVis(ivec2 coord) {
    float z = gl_FragCoord.z;
    float acc_alpha_to_z = 1;
    int i = 0;

    for(int i = 0;  i < 8; ++i) {
        float z_i = texelFetch(depthMapMS, coord, i).x;
        float a_i = texelFetch(colorMapMS, coord, i).a;
        if(z_i == 1) {
            return z_i;
        }
        
        if(z > z_i) {
            acc_alpha_to_z*=(1-a_i);
        }
    }
    return acc_alpha_to_z; 
}

float calcVis2(ivec2 coord) {
    float z = gl_FragCoord.z;
    int count = 0;
    int S=0;

    for(int i = 0;  i < 8; ++i) {
        float z_i = texelFetch(depthMapMS, coord, i).x;
       if(z_i != 0) {
           ++S;
       }
        
        if(z <= z_i) {
           ++count;
        }
    }
    if(S == 0) {
        return 0;
    }
    //return float(S)/8.0; 
    return count/8.0;
}


void main() {
    
    vec2 msDepthSize = textureSize(depthMapMS);
    ivec2 msDepthCoord = ivec2(gl_FragCoord.xy);
    double z = gl_FragCoord.z;

    float wholeAlpha = 1 - texture2D(alphaAcc, gl_FragCoord.xy/vec2(1280,720)).x;
    vec3 outColor = vec3(0,0,0);
    int count = 0; 
  
    for(int i = 0;  i < 8; ++i) {
        vec3 color = texelFetch(colorMapMS, msDepthCoord, i).rgb;
        float alpha = texelFetch(colorMapMS, msDepthCoord, i).a;
        
            outColor += color;
            ++count;
        
    }

    outColor /= count;
    
  //  outColor = vec3((8-count));
    
    float vis = calcVis2(msDepthCoord);
    //vis = gl_FragCoord.z<vis ? 1 : 0;
    frag_color = isTextured != 0 ? texture2D(diffuse_texture, texCoord) : vec4(color,1.0);
 	
  //  if(z > 0.89 || z < 0.85)
    //    discard;
    vec4 c_rgba = vec4(CalcDirLight(normalWorld, frag_color.xyz) + ambientLight*frag_color.xyz, 1.0);
    //frag_color =  vis*alpha*c_rgba;
    
    //vis = 1-vis;
    //frag_color = vec4(vis,0,0,alpha);
    //frag_color = vec4( texelFetch(colorMapMS, msDepthCoord, 0).xw, 0, 1);
    //float accAlpha = 0.6;
    //outColor = frag_color.xyz;
    //frag_color = vec4(vec3(count/8.0),1.0);// vec4(wholeAlpha*outColor + (1-wholeAlpha)*vec3(1), 1.0);
    frag_color = vec4(wholeAlpha*outColor + (1-wholeAlpha)*vec3(1), 1.0);
       // frag_color =  texture2D(diffuse_texture, texCoord);
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


