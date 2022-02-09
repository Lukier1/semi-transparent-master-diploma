#version 400

in vec3 position_eye, normal_eye;
in vec2 tex_coord;
in vec3 view_dir_tan, light_dir_tan;
in vec3 loc_normal;

out vec4 frag_colour;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normals_texture;
uniform vec3 Ls, Ld, La, LPos_world;
uniform float LPower;

// light position, specular, diffuse, ambient
uniform vec3 Ks, Kd, Ka;
// color specular, diffuse, ambient
uniform float spec_power;
// specular factor

uniform mat4 view_mat;
uniform uint shader_flag;
const uint NORMALS_MAP_MODE = 0x1;
const uint SPECULAR_MAP_MODE = 0x2;
void main () {
	if (gl_FrontFacing) {
		discard;
	}
    //FOG Config
	//const vec3 fog_colour = vec3 ( 0.2, 0.2, 0.2);
	//const float min_fog_rad = 3.0;
    //const float max_fog_rad = 10.0;
    //float dist = length(-position_eye);
    //float fog_fac = log(( dist - min_fog_rad) / (max_fog_rad - min_fog_rad));
    //fog_fac = clamp(fog_fac, 0.0, 1.0);
    //ambient intesity
	vec3 Ia = La * Ka;
    //Diffuse intesity
	float dot_prod = 0;
	vec3 normal_tan = texture(normals_texture, tex_coord).xyz;
	normal_tan = normalize (normal_tan * 2.0 - 1.0);
	
	vec3 LPos_eye = vec3( view_mat * vec4(LPos_world,1.0));
	vec3 distance_to_light_eye = LPos_eye - position_eye;
	vec3 direction_to_light_eye = normalize(distance_to_light_eye);

    if(shader_flag&NORMALS_MAP_MODE) {
        vec3 direction_to_light_tan = normalize(-light_dir_tan);
        dot_prod = dot(direction_to_light_tan, normal_tan);
        dot_prod = max(dot_prod, 0.0);
    }
	else 
	{
		dot_prod = dot(direction_to_light_eye, normal_eye);
		dot_prod = max(dot_prod, 0.0);
	}


	vec3 Id = Ld*dot_prod*LPower/(length(distance_to_light_eye)+1);
    vec3 Is;
    //Specular intestiy 

	vec4 specMap = vec4(1.0,1.0,1.0,1.0);
    if (shader_flag&SPECULAR_MAP_MODE) {
        specMap =  texture2D(specular_texture, tex_coord);
    }


	if(shader_flag&NORMALS_MAP_MODE) {
		
		vec3 reflection_tan = reflect (normalize (light_dir_tan), normal_tan);
		float dot_prod_specular = dot (reflection_tan, normalize (view_dir_tan));
		dot_prod_specular = max (dot_prod_specular, 0.0)*specMap.x;
		float specular_factor = pow (dot_prod_specular, spec_power);

		Is = Ls*Ks*specular_factor;
		
	}
	else
	{
		vec3 surface_to_viewer_eye = normalize(-position_eye);
		vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
		float dot_prod_specular = dot(half_way_eye, normal_eye);
		dot_prod_specular = max(dot_prod_specular,0.0)*specMap.x;
		float specular_factor = pow(dot_prod_specular, spec_power);
		Is = Ls*Ks*specular_factor;
	}
  

  // Texture
	vec4 texel =  texture2D(diffuse_texture, tex_coord);
	vec3 phong_colour= vec3((Ia+Id)*texel.xyz+Is);
	//frag_colour = vec4( Kd ,1.0);
	frag_colour = vec4(phong_colour, texel.w);;
    //vec4(mix(phong_colour, fog_colour, fog_fac), texel.w); 
};
