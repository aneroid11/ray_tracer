#version 330 core

layout (location = 0) in vec2 vertex_pos;

uniform vec3 player_position;
uniform mat4x4 camera_rotation;

out vec3 pl_pos;
out vec3 ray_direction;

const float WINDOW_Z = 2.0;

void main() {
	gl_Position = vec4(vertex_pos, 0.0, 1.0);
	vec4 r = camera_rotation * vec4(vertex_pos.x, vertex_pos.y * 0.56, WINDOW_Z, 1.0);
	//ray_direction = vec3(vertex_pos, WINDOW_Z);
	ray_direction = r.xyz;
	
	pl_pos = player_position;
}
