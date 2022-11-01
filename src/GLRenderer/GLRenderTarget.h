#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
using namespace glm;
struct GLRenderTargetOptions
{
    GLenum wrap_s = GL_CLAMP_TO_EDGE;
    GLenum wrap_t = GL_CLAMP_TO_EDGE;
    GLenum mag_filter = GL_LINEAR;
    GLenum min_filter = GL_LINEAR;
    bool should_generate_mipmaps = false;
    GLenum format = GL_RGBA8;
    GLenum type = GL_UNSIGNED_BYTE;
    int samples = 0;
};

struct GLRenderTarget
{
    GLRenderTarget(int width, int height, const GLRenderTargetOptions &options = GLRenderTargetOptions())
    {
        glCreateFramebuffers(1, &handle);
        glCreateTextures(GL_TEXTURE_2D, 1, &tex_handle);

        glTextureStorage2D(tex_handle, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glCreateTextures(GL_TEXTURE_1D, 1, &depth_handle);
        
        glNamedFramebufferTexture(handle, GL_COLOR_ATTACHMENT0, tex_handle, 0);
        glNamedFramebufferTexture(handle, GL_DEPTH_ATTACHMENT, depth_handle, 0);
    }

    int width;
    int height;
    vec4 viewport;
    vec4 scissor;
    GLuint handle;
    GLuint tex_handle;
    GLuint depth_handle;
};