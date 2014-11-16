#version 330

uniform mat4 modelViewProjection;
uniform mat4 modelView;
uniform mat4 view;

layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

out vec3 normal;
out vec2 uv;
out vec3 light_dir;
out vec3 view_dir;

void main() {
	vec3 pos = (modelView * vec4(in_vertex, 1.0)).xyz;
	normal = mat3(modelView) * in_normal;
	uv = in_uv;

	light_dir = ((view * vec4(0.0, 0.0, -0.5, 0.0)).xyz - pos);
	view_dir = -pos;
	gl_Position = modelViewProjection * vec4(in_vertex, 1.0);
}