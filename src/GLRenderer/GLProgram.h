#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>
using namespace std;
struct GLBuffer
{
    GLBuffer()
    {
    }
    GLBuffer(GLsizeiptr size, const void *data, GLbitfield flags)
    {

        glCreateBuffers(1, &handle);
        glNamedBufferStorage(handle, size, data, flags);
    }

    void destroy()
    {
        glDeleteBuffers(1, &handle);
    }

    GLuint handle;
};

struct GLShader
{
    GLShader()
    {
    }
    GLShader(const char *fileName, GLenum type)
        : handle(glCreateShader(type))
    {
        ifstream t(fileName);
        stringstream buffer;
        if (t.is_open() && t.good())
        {
            buffer << t.rdbuf();
            const string text_string = buffer.str();
            const char *text = text_string.c_str();

            glShaderSource(handle, 1, &text, nullptr);
            glCompileShader(handle);
            char message[8192];
            GLsizei length = 0;
            glGetShaderInfoLog(handle, sizeof(message), &length, message);

            if (length)
            {
                printf("%s (File: %s)\n", message, fileName);
                printf("shader file content: \n %s", text);
                throw std::runtime_error("failed to compile shader");
            }
        }
    }

    void destroy()
    {
        glDeleteShader(handle);
    }

    GLuint handle;
};

struct GLProgram
{
    GLProgram()
    {
    }
    GLProgram(const GLShader &shader1, const GLShader &shader2)
    {
        handle = glCreateProgram();
        glAttachShader(handle, shader1.handle);
        glAttachShader(handle, shader2.handle);
        glLinkProgram(handle);
        char buffer[8192];
        GLsizei length = 0;
        glGetProgramInfoLog(handle, sizeof(buffer), &length, buffer);
        if (length)
        {
            printf("%s\n", buffer);
            throw new std::runtime_error("failed to create program");
        }
    }

    void use()
    {
        glUseProgram(handle);
    }

    void destroy()
    {
        glDeleteProgram(handle);
    }

    GLuint handle;
};
