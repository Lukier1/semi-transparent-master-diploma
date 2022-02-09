#version 420

in vec2 texCoord;

in vec3 positionWorld;
in vec3 normalWorld;

uniform vec3 color;
uniform float alpha;
uniform int stage;
out vec4 frag_color;
uniform sampler2D depthTexture;


const int MAX_LIGHTS_NUM = 128;

uniform unsigned int dirLightNumber;
uniform vec3 dirLightDirection[MAX_LIGHTS_NUM];
uniform vec3 dirLightColor[MAX_LIGHTS_NUM];

uniform vec3 ambientLight;
uniform vec3 cameraPosition;

uniform int BUFFER_SIZE;

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
    if(stage != 0) {
        vec2 st = vec2(gl_FragCoord.x/1280.0, gl_FragCoord.y/720.0);
        if(gl_FragCoord.z <= texture(depthTexture, st).x ) {
            discard;
        } 
        // gl_FragDepth = gl_FragCoord.z > texture(depthTexture, st).x ? 0.5 : 0
    } 

    vec3 Lr = CalcDirLight(normalWorld, color) + ambientLight*color;
    frag_color = vec4(Lr, alpha);
}