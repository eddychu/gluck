#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;

constexpr const uint64_t INVALID_TEXTURE = 0xFFFFFFFF;
struct Material
{
    vec4 emissiveColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 albedoColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 roughness = vec4(1.0f, 1.0f, 0.0, 0.0f);
    uint64_t ambientOcclusionMap_ = INVALID_TEXTURE;
    uint64_t emissiveMap_ = INVALID_TEXTURE;
    uint64_t albedoMap_ = INVALID_TEXTURE;
    /// Occlusion (R), Roughness (G), Metallic (B) https://github.com/KhronosGroup/glTF/issues/857
    uint64_t metallicRoughnessMap_ = INVALID_TEXTURE;
    uint64_t normalMap_ = INVALID_TEXTURE;
    uint64_t opacityMap_ = INVALID_TEXTURE;
};