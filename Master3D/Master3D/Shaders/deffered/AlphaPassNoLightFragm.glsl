#version 420

in vec3 positionCamera;
in vec3 normalCamera;
in vec2 texCoord;

out vec4 alphaAcc;
layout(binding=2) uniform sampler2D gBackgroundDepth;

layout (location = 0) out vec4 alphaAccRGBA; 
layout (location = 1) out vec4 alphaBckD; 
layout (location = 2) out vec2 alphaDelta;
 
uniform vec3 ambientLight;


uniform vec3 cameraPosition;
uniform mat4 PMat;

//Object properties
uniform sampler2D diffuse_texture;


//Alpha object properties
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

	vec3 wi = -eta*w0 + ( eta * c - sqrt(k)) * n;
	wi = normalize(wi);
	
	float proj =  dot(wi,  -vec3(0,0,1));
	
	vec3 B = X + wi*deltaZ/proj;
	vec3 predX = X - w0*deltaZ/dot(-w0, -vec3(0,0,1));

	return (PMat*vec4(B,1.0)).xy - (PMat*vec4(predX,1.0)).xy; 
}


void discardWhenIsCovered() {
	if(gl_FragCoord.z > texture2D(gBackgroundDepth, vec2(gl_FragCoord.x/1280, gl_FragCoord.y/720)).x)
	{
		discard;
	}
}

void main() {
	//TODO Dodac wysylanie 

	discardWhenIsCovered();
	
	vec4 fragColor = texture2D(diffuse_texture, texCoord);
	vec3 t = vec3(1)-transmitive;

	//LIGHT PART
	vec3 Lr;
	Lr  = ambientLight;

	float netCoverage = alpha*(1.0 - dot(t, vec3(1.0/3.0)));
	
	weight = clamp(pow(10*(1-0.99*gl_FragCoord.z)*netCoverage, 3), 0.01, 30);
	
	//Color of object
	alphaAccRGBA = vec4(alpha*Lr, netCoverage)*weight; 
	
	//Transimssion
	alphaBckD.xyz = alpha*(vec3(1)-t)*(1.0/3.0);
	alphaBckD.w = 0.0f;
	
	//Difraction
	if(eta != 1.0) {
		vec2 refractPix = refractOffset(normalCamera, positionCamera, eta);
		alphaDelta = refractPix* netCoverage;
	}
	else {
		alphaDelta = vec2(0,0);
	}
}
