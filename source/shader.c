#include "shader.h"

#include <stdlib.h>

GLuint _compile_shader_stage(GLenum type, const char * code)
{
    // Create a blank shader stage, type will determine what stage
    GLuint id = glCreateShader(type);

    // Upload the GLSL code to the driver
    glShaderSource(id, 1, (const GLchar **)&code, NULL);

    // Compile the GLSL code into GPU-specific instructions
    glCompileShader(id);

    // Print compiler errors, if compilation fails
    GLint compiled = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint logLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

        char * log = (char *)malloc(logLength);
        glGetShaderInfoLog(id, logLength, &logLength, log);
        printf("%s\n", log);
        free(log);

        return 0;
    }

    return id;
}

GLuint load_shader(const char * vertex_code, const char * fragment_code)
{
    // Create a blank shader (OpenGL calls this a "program")
    GLuint id = glCreateProgram();

    // Compile each shader stage (OpenGL calls each of these a "shader")
    GLuint vertex_id = _compile_shader_stage(GL_VERTEX_SHADER, vertex_code);
    GLuint fragment_id = _compile_shader_stage(GL_FRAGMENT_SHADER, fragment_code);

    // Attach each shader stage in the shader to prepare for linking
    glAttachShader(id, vertex_id);
    glAttachShader(id, fragment_id);

    // Link all the shader stages together
    glLinkProgram(id);

    // Print linker errors, if linking fails
    GLint linked = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint logLength = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

        char * log = (char *)malloc(logLength);
        glGetProgramInfoLog(id, logLength, &logLength, log);
        printf("%s\n", log);
        free(log);

        return 0;
    }

    // Once linked, we no longer need the shader stages
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    return id;
}