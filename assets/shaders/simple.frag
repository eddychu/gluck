#version 460 core

layout(std140, binding = 0) uniform PerFrameData
{
	mat4 view;
	mat4 proj;
	vec3 cameraPos;
};
layout (location=0) in vec2 tc;
layout (location=1) in vec3 normal;
layout (location=2) in vec3 worldPos;

layout (location=0) out vec4 out_FragColor;

layout (binding=0) uniform sampler2D texAlbedo;
layout (binding=5) uniform samplerCube texEnvMap;


void main() {
    vec4 Kd = texture(texAlbedo, tc);
    out_FragColor = Kd;
}
