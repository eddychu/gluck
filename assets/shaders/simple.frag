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
layout (binding=3) uniform sampler2D texMetalRoughness;
layout (binding=4) uniform sampler2D texAO;
layout (binding=5) uniform samplerCube texEnvMap;
layout (binding=6) uniform samplerCube texEnvMapIrradiance;
layout (binding=7) uniform sampler2D texBRDFLUT;
const float M_PI = 3.141592653589793;

vec4 SRGBtoLINEAR(vec4 srgbIn)
{
	vec3 linOut = pow(srgbIn.xyz,vec3(2.2));

	return vec4(linOut, srgbIn.a);
}




struct PBRInfo {
	float NdotL;
	float NdotV;
	float NdotH;
	float LdotH;
	float VdotH;
	float perceptualRoughness;
	vec3 reflectance0;   
	vec3 reflectance90;
	float alphaRoughness;
	vec3 diffuseColor; 
	vec3 specularColor;
	vec3 n;	
	vec3 v;
};

vec3 getIBLContribution(PBRInfo pbrInputs, vec3 n, vec3 relfection)
{
	float lod = pbrInputs.perceptualRoughness;
	vec2 brdfSamplePoint =  clamp(vec2(pbrInputs.NdotV, 1.0-pbrInputs.perceptualRoughness), vec2(0.0, 0.0), vec2(1.0, 1.0));
	vec3 brdf = textureLod(texBRDFLUT, brdfSamplePoint, 0).rgb;
	vec3 cm = vec3(1.0, 1.0, 1.0);
	vec3 diffuseLight = texture(texEnvMapIrradiance, n.xyz * cm).rgb;
	vec3 specularLight = textureLod(texEnvMap, relfection.xyz * cm, lod).rgb;
	vec3 diffuse = diffuseLight * pbrInputs.diffuseColor;
	vec3 specular = specularLight * (pbrInputs.specularColor * brdf.x + brdf.y);
	return diffuse + specular;
}

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

vec3 diffuseBurley(PBRInfo pbrInputs)
{
	float f90 = 2.0 * pbrInputs.LdotH * pbrInputs.LdotH * pbrInputs.alphaRoughness - 0.5;

	return (pbrInputs.diffuseColor / M_PI) * (1.0 + f90 * pow((1.0 - pbrInputs.NdotL), 5.0)) * (1.0 + f90 * pow((1.0 - pbrInputs.NdotV), 5.0));
}

vec3 specularReflection(PBRInfo pbrInputs) 
{
	return pbrInputs.reflectance0 + (pbrInputs.reflectance90 - pbrInputs.reflectance0) * pow(clamp(1.0 - pbrInputs.VdotH, 0.0, 1.0), 5.0);
}

float geometricOcclusion(PBRInfo pbrInputs)
{
	float NdotL = pbrInputs.NdotL;
	float NdotV = pbrInputs.NdotV;
	float rSqr = pbrInputs.alphaRoughness * pbrInputs.alphaRoughness;

	float attenuationL = 2.0 * NdotL / (NdotL + sqrt(rSqr + (1.0 - rSqr) * (NdotL * NdotL)));
	float attenuationV = 2.0 * NdotV / (NdotV + sqrt(rSqr + (1.0 - rSqr) * (NdotV * NdotV)));
	return attenuationL * attenuationV;
}

float microfacetDistribution(PBRInfo pbrInputs)
{
	float roughnessSq = pbrInputs.alphaRoughness * pbrInputs.alphaRoughness;
	float f = (pbrInputs.NdotH * roughnessSq - pbrInputs.NdotH) * pbrInputs.NdotH + 1.0;
	return roughnessSq / (M_PI * f * f);
}

PBRInfo calculatePBRInputs(vec4 albedo, vec3 normal, vec3 cameraPos, vec4 mrSample)
{
	float perceptualRoughness = 1.0;
	float metallic = 1.0;
	perceptualRoughness = mrSample.g * perceptualRoughness;
	metallic = mrSample.b * metallic;

	perceptualRoughness = clamp(perceptualRoughness, 0.04, 1.0);
	metallic = clamp(metallic, 0.0, 1.0);
	float alphaRoughness = perceptualRoughness * perceptualRoughness;
	vec4 baseColor= albedo;
	vec3 f0 = vec3(0.04);
	vec3 diffuseColor = baseColor.rgb * (vec3(1.0) - f0);
	diffuseColor *= 1.0 - metallic;
	vec3 specularColor = mix(f0, baseColor.rgb, metallic);

	float reflectance = max(max(specularColor.r, specularColor.g), specularColor.b);
	float reflectance90 = clamp(reflectance * 25.0, 0.0, 1.0);
	vec3 specularEnvironmentR0 = specularColor.rgb;
	vec3 specularEnvironmentR90 = vec3(1.0, 1.0, 1.0) * reflectance90;

	vec3 n = normalize(normal);					// normal at surface point
	vec3 v = normalize(cameraPos - worldPos);	// Vector from surface point to camera
	vec3 reflection = -normalize(reflect(v, n));
	PBRInfo pbrInputs;

	pbrInputs.NdotV = clamp(abs(dot(n, v)), 0.001, 1.0);
	pbrInputs.perceptualRoughness = perceptualRoughness;
	pbrInputs.reflectance0 = specularEnvironmentR0;
	pbrInputs.reflectance90 = specularEnvironmentR90;
	pbrInputs.alphaRoughness = alphaRoughness;
	pbrInputs.diffuseColor = diffuseColor;
	pbrInputs.specularColor = specularColor;
	pbrInputs.n = n;
	pbrInputs.v = v;
	return pbrInputs;
}

vec3 calculateLight(inout PBRInfo pbrInputs, vec3 lightDirection, vec3 lightColor)
{
	vec3 n = pbrInputs.n;
	vec3 v = pbrInputs.v;
	vec3 l = normalize(lightDirection);
	vec3 h = normalize(l + v);

	float NdotV = pbrInputs.NdotV;
	float NdotL = clamp(dot(n, l), 0.001, 1.0);
	float NdotH = clamp(dot(n, h), 0.0, 1.0);
	float LdotH = clamp(dot(l, h), 0.0, 1.0);
	float VdotH = clamp(dot(v, h), 0.0, 1.0);

	pbrInputs.NdotL = NdotL;
	pbrInputs.NdotH = NdotH;
	pbrInputs.LdotH = LdotH;
	pbrInputs.VdotH = VdotH;

	vec3 F = specularReflection(pbrInputs);
	float G = geometricOcclusion(pbrInputs);
	float D = microfacetDistribution(pbrInputs);

	vec3 diffuseContrib = (1.0 - F) * diffuseBurley(pbrInputs);
	vec3 specContrib = F * G * D / (4.0 * NdotL * NdotV);
	vec3 color = NdotL * lightColor * (diffuseContrib + specContrib);
	// vec3 color = NdotL * lightColor * diffuseContrib;
	return color;
}

void main() {
	vec4 Kd = texture(texAlbedo, tc);
	vec4 Ke = texture(texEmissive, tc);
	vec4 Kao = texture(texAO, tc);

	vec3 L = lightPos - worldPos;
	float D = length(L);
	L = normalize(L);

	vec3 n = normalize(normal);
	vec3 normalSample = texture(texNormal, tc).xyz;
	n = calculateNormal(n, normalize(cameraPos - worldPos), normalSample, tc);

	vec4 mrSample = texture(texMetalRoughness, tc);
	PBRInfo pbrInputs = calculatePBRInputs(Kd, n, cameraPos, mrSample);
	// vec3 color = getIBLContribution(pbrInputs, n, reflection);
	vec3 color = calculateLight(pbrInputs, normalize(vec3(1.0, 1.0, 1.0)), intensity);
	color = color * ( Kao.r < 0.01 ? 1.0 : Kao.r );
	// color = pow(Ke.rgb + color, vec3(1.0/2.2));

    out_FragColor = vec4(color + Ke.rgb, 1.0);
}
