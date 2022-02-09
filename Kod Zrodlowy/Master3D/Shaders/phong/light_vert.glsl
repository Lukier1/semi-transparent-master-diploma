#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vt;
layout (location = 3) in vec4 vertex_tanget;

uniform mat4 mv_mat, mvp_mat, world_mat, view_mat;
uniform vec3 LPos_world;

out vec3 position_eye, normal_eye;
out vec2 tex_coord;
out vec3 view_dir_tan;
out vec3 light_dir_tan;
out vec3 loc_normal;

void main() {
	gl_Position = mvp_mat * vec4(vertex_position, 1.0);

	tex_coord = vt;
	position_eye = vec3(mv_mat*vec4(vertex_position,1.0));
	normal_eye = normalize(mat3(mv_mat)*vec3(vertex_normal));

	vec3 vertex_bitangent = cross(vertex_normal, vertex_tanget.xyz) * vertex_tanget.w;
	

	vec3 campos_wor = (inverse(view_mat)*vec4(0.0,0.0,0.0,1.0)).xyz;
	vec3 light_dir_wor = -normalize(LPos_world - vec3(vec4(vertex_position, 1.0) * world_mat));
		
	vec3 campos_loc = vec3(inverse(world_mat) * vec4(campos_wor,1.0));
	vec3 light_dir_loc = vec3(inverse(world_mat) * vec4(light_dir_wor,0.0));
	vec3 view_dir_loc = normalize(campos_loc - vertex_position);

	view_dir_tan = vec3 (
		dot (vertex_tanget.xyz, view_dir_loc),
		dot (vertex_bitangent, view_dir_loc),
		dot (vertex_normal, view_dir_loc)
	);
	// work out light direction in _tangent space_
	light_dir_tan = vec3 (
		dot (vertex_tanget.xyz, light_dir_loc),
		dot (vertex_bitangent, light_dir_loc),
		dot (vertex_normal, light_dir_loc)
	);

	loc_normal = (vertex_normal+vec3(0.5,0.5,0.5))*0.5;


}