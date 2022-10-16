#pragma once

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stdexcept>
#include <Common/bitmap.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

int getNumMipMapLevels2D(int w, int h)
{
    int levels = 1;
    while ((w | h) >> levels)
        levels += 1;
    return levels;
}

uint8_t *genDefaultCheckerboardImage(int *width, int *height)
{
    const int w = 128;
    const int h = 128;

    uint8_t *imgData = (uint8_t *)malloc(w * h * 3); // stbi_load() uses malloc(), so this is safe

    assert(imgData && w > 0 && h > 0);
    assert(w == h);

    if (!imgData || w <= 0 || h <= 0)
        return nullptr;
    if (w != h)
        return nullptr;

    for (int i = 0; i < w * h; i++)
    {
        const int row = i / w;
        const int col = i % w;
        imgData[i * 3 + 0] = imgData[i * 3 + 1] = imgData[i * 3 + 2] = 0xFF * ((row + col) % 2);
    }

    if (width)
        *width = w;
    if (height)
        *height = h;

    return imgData;
}

struct GLTexture
{
    GLTexture()
    {
    }
    GLTexture(GLenum type_, const char *fileName)
        : type(type_)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glCreateTextures(type, 1, &handle);
        glTextureParameteri(handle, GL_TEXTURE_MAX_LEVEL, 0);
        glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

        const char *ext = strrchr(fileName, '.');

        switch (type)
        {
        case GL_TEXTURE_2D:
        {
            int w = 0;
            int h = 0;
            int numMipmaps = 0;
            uint8_t *img = stbi_load(fileName, &w, &h, nullptr, STBI_rgb_alpha);
            if (!img)
            {
                img = genDefaultCheckerboardImage(&w, &h);
                if (!img)
                {
                    throw std::runtime_error("failed to load image %s" + std::string(fileName));
                }
            }
            // numMipmaps = getNumMipMapLevels2D(w, h);
            glTextureStorage2D(handle, 1, GL_RGBA8, w, h);
            glTextureSubImage2D(handle, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
            stbi_image_free((void *)img);
            // glGenerateTextureMipmap(handle);
            // glTextureParameteri(handle, GL_TEXTURE_MAX_LEVEL, numMipmaps - 1);
            glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(handle, GL_TEXTURE_MAX_ANISOTROPY, 16);
            break;
        }
        case GL_TEXTURE_CUBE_MAP:
        {
            int w, h, comp;
            const float *img = stbi_loadf(fileName, &w, &h, &comp, 3);
            assert(img);
            Bitmap in(w, h, comp, eBitmapFormat_Float, img);
            const bool isEquirectangular = w == 2 * h;
            Bitmap out = isEquirectangular ? convertEquirectangularMapToVerticalCross(in) : in;
            stbi_image_free((void *)img);
            Bitmap cubemap = convertVerticalCrossToCubeMapFaces(out);
            const int numMipmaps = getNumMipMapLevels2D(cubemap.w_, cubemap.h_);
            glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTextureParameteri(handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTextureParameteri(handle, GL_TEXTURE_BASE_LEVEL, 0);
            glTextureParameteri(handle, GL_TEXTURE_MAX_LEVEL, numMipmaps - 1);
            glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
            glTextureStorage2D(handle, numMipmaps, GL_RGB32F, cubemap.w_, cubemap.h_);
            const uint8_t *data = cubemap.data_.data();

            for (unsigned i = 0; i != 6; ++i)
            {
                glTextureSubImage3D(handle, 0, 0, 0, i, cubemap.w_, cubemap.h_, 1, GL_RGB, GL_FLOAT, data);
                data += cubemap.w_ * cubemap.h_ * cubemap.comp_ * Bitmap::getBytesPerComponent(cubemap.fmt_);
            }

            glGenerateTextureMipmap(handle);
            break;
        }

        default:
        {
            throw std::runtime_error("not implemented!!!");
        }
        }
    }

    void destroy()
    {
        glDeleteTextures(1, &handle);
    }

    GLenum type;
    GLuint handle;
};
