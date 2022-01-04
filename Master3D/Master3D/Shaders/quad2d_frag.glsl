#version 400

uniform sampler2D diffuse_texture;
uniform vec2 texSize;
varying vec2 st;
out vec4 frag_color;
float pixelSize = 2.0/texSize.x;
vec3 gaussFilter(float sigma) 
{
    vec3 outFilter= vec3(0,0,0);
    float sumG = 0;
    for(int x = -5; x <= 5; ++x)
  {
        for(int y = -5; y <= 5; ++y)
	{
            float G = (1/(2*3.14*sigma*sigma))*exp(-(x*x+y*y)/(2*sigma*sigma));
            sumG+=G;
            outFilter +=  G*vec3(texture2D(diffuse_texture, st+vec2(pixelSize*x,pixelSize*y)));
        }

  }

  return outFilter/sumG;
}


vec3 Laplacian() {
    vec3 outFilter= vec3(0,0,0);
    float sumG = 0;
    for(int x = -1; x <= 1; ++x)
	{
        for(int y = -1; y <= 1; ++y)
		{
            float G = 1;
            if(y == 0 && x == 0)
			{
                G = -8;
            }
			sumG+=G;
            outFilter +=  G*vec3(texture2D(diffuse_texture, st+vec2(pixelSize*x,pixelSize*y)));
        }

	}
	return outFilter;
}

void main() {
	vec4 tex_out = texture2D(diffuse_texture, st);


	frag_color = vec4(vec3(tex_out.xyz/tex_out.w), 1.0);
	
	//frag_color = vec4(vec3(tex_out.w/1000.0), 1.0);
	//frag_color = texture2D(diffuse_texture, st);
	/*
    float pixelSize = 3.0/texSize.x;
    vec3 outFilter= vec3(0,0,0);
    float sumG = 0;
    for(int x = -1; x <= 1; ++x)
	{
        for(int y = -1; y <= 1; ++y)
		{
            float G = 1;
            if(y == 0 && x == 0)
			{
                G = -8;
            }

			sumG+=G;
            outFilter +=  G*vec3(texture2D(diffuse_texture, st+vec2(pixelSize*x,pixelSize*y)));
        }
	}
	float sum = (outFilter.x+outFilter.y+outFilter.z)/3;
	if(sum > 1.0)
	{
		//frag_color = vec4(1.0, 0.0, 0.0, 1.0);
		frag_color = vec4(gaussFilter(2.0),1.0);
	}

	else
	{
		
	}
	*/
  	//frag_color.xyz = Laplacian();
  //frag_color.xyz = vec3(sum,sum,sum);
 // frag_color.w = 1.0;
  //frag_color = vec4(outFilter/sumG,1.0); 
  /*
  vec4 texel1 =  texture2D(diffuse_texture, st+vec2(sizeKer,sizeKer));
  
  vec4 texel2 =  texture2D(diffuse_texture, st+vec2(sizeKer,-sizeKer));
  
  vec4 texel3 =  texture2D(diffuse_texture, st+vec2(-sizeKer, sizeKer));
  
  vec4 texel4 =  texture2D(diffuse_texture, st+vec2(-sizeKer, -sizeKer));
  
  frag_color = (texel1+texel2+texel3+texel4)*0.25;
 //frag_color.y=tan(frag_color.y*3.14*10);
 //frag_color.x=sin(frag_color.x*3.14*10);*/
}
