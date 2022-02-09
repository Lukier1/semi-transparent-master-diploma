#version 420

in vec3 positionWorld;
in vec3 normalWorld;

uniform vec3 color;
uniform float alpha;
out vec4 frag_color;

layout(binding=9) uniform sampler2D depthTexture2;
layout(binding=4) uniform sampler2D color1;

uniform int isLayerDraw;


const int MAX_LIGHTS_NUM = 128;

uniform unsigned int dirLightNumber;
uniform vec3 dirLightDirection[MAX_LIGHTS_NUM];
uniform vec3 dirLightColor[MAX_LIGHTS_NUM];

uniform vec3 ambientLight;
uniform vec3 cameraPosition;

uniform int BUFFER_SIZE;


vec2 st = vec2(gl_FragCoord.x/1280.0, gl_FragCoord.y/720.0);

float getMaxDepth() {

    float d2 = texture(depthTexture2, st).x;
    return d2;
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

void main() {
    if (gl_FragCoord.z <= getMaxDepth()) {
        discard;    
    }

    vec3 Lr = CalcDirLight(normalWorld, color) +ambientLight*color;
    frag_color = vec4(Lr, alpha);
  
}
