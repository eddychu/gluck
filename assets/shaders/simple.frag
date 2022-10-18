#version 460 core

layout(std140, binding = 0) uniform PerFrameData
{
	mat4 view;
	mat4 proj;
	vec3 cameraPos;
};

layout(std140, binding = 3) uniform Light
{
	vec4 lightPos;
	vec4 intensity;
};



layout (location=0) in vec2 tc;
layout (location=1) in vec3 normal;
layout (location=2) in vec3 worldPos;

layout (location=0) out vec4 out_FragColor;

layout (binding=0) uniform sampler2D texAlbedo;
layout (binding=1) uniform sampler2D texEmissive;
layout (binding=2) uniform sampler2D texNormal;
layout (binding=3) uniform sampler2D texMetalRoughness;
layout (binding=4) uniform sampler2D texAO;
layout (binding=5) uniform samplerCube texEnvMap;
layout (binding=6) uniform samplerCube texEnvMapIrradiance;
layout (binding=7) uniform sampler2D texBRDFLUT;
const float PI = 3.141592653589793;
const vec3 Fdielectric = vec3(0.04);



mat3 cotangentFrame( vec3 N, vec3 p, vec2 uv )
{
	// get edge vectors of the pixel triangle
	vec3 dp1 = dFdx( p );
	vec3 dp2 = dFdy( p );
	vec2 duv1 = dFdx( uv );
	vec2 duv2 = dFdy( uv );

	// solve the linear system
	vec3 dp2perp = cross( dp2, N );
	vec3 dp1perp = cross( N, dp1 );
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	// construct a scale-invariant frame
	float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );

	// calculate handedness of the resulting cotangent frame
	float w = (dot(cross(N, T), B) < 0.0) ? -1.0 : 1.0;

	// adjust tangent if needed
	T = T * w;

	return mat3( T * invmax, B * invmax, N );
}

vec3 calculateNormal(vec3 n, vec3 v, vec3 normalSample, vec2 uv) {
	vec3 map = normalize(2.0 * normalSample - vec3(1.0));
	mat3 TBN = cotangentFrame(n, v, uv);
	return normalize(TBN * map);
}

vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}



void main() {
	vec3 Kd = texture(texAlbedo, tc).rgb;
	vec3 Ke = texture(texEmissive, tc).rgb;
	float Kao = texture(texAO, tc).r;
	vec4 mrSample = texture(texMetalRoughness, tc);
	float roughness = mrSample.g;
	float metalness = mrSample.b;
	vec3 l = normalize(lightPos.xyz - worldPos);

	vec3 n = normalize(normal);
	vec3 normalSample = texture(texNormal, tc).xyz;
	n = calculateNormal(n, normalize(cameraPos - worldPos), normalSample, tc);

	vec3 F0 = mix(Fdielectric, Kd, metalness);

	// calculate direct lighting
	vec3 v = normalize(cameraPos - worldPos);
	vec3 h = normalize(v + l);
	float NdotL = clamp(dot(n, l), 0.0, 1.0);
	float NdotH = clamp(dot(n, h), 0.0, 1.0);
	float LdotH = clamp(dot(l, h), 0.0, 1.0);
	float VdotH = clamp(dot(v, h), 0.0, 1.0);
	float NdotV = clamp(dot(v, n), 0.0, 1.0);

	vec3 F  = fresnelSchlick(F0, LdotH);
	float D = ndfGGX(NdotH, roughness);
	float G = gaSchlickGGX(NdotV, NdotL, roughness);

	vec3 diffuse = mix(vec3(1.0) - F, vec3(0.0), metalness);

	vec3 diffuseBRDF = diffuse * Kd;

	vec3 specularBRDF = (F * D * G) / max(0.001, 4.0 * NdotV * NdotL);

	vec3 color = (diffuseBRDF + specularBRDF) * intensity.xyz * NdotV;
    out_FragColor = vec4(color + Ke, 1.0);
}
