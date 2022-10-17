#version 460 core

layout(std140, binding = 0) uniform PerFrameData
{
	mat4 view;
	mat4 proj;
	vec3 cameraPos;
};

layout(std140, binding = 1) uniform LightData {
	vec3 lightPos;
	vec3 intensity;
};

layout (location=0) in vec2 tc;
layout (location=1) in vec3 normal;
layout (location=2) in vec3 worldPos;

layout (location=0) out vec4 out_FragColor;

layout (binding=0) uniform sampler2D texAlbedo;
layout (binding=1) uniform sampler2D texEmissive;
layout (binding=2) uniform sampler2D texNormal;
layout (binding=5) uniform samplerCube texEnvMap;


void main() {
	vec3 L = lightPos - worldPos;
	float D = length(L);
	L = normalize(L);
	float diffuse = dot(L, normal) / (D * D);

    vec4 Kd = texture(texAlbedo, tc);
	vec4 Ke = texture(texEmissive, tc);
	vec4 N = texture(texNormal, tc);
    out_FragColor = Ke + Kd * diffuse;
}
