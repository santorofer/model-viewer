#include "material.h"

#include <assert.h> // assert()
#include <string.h> // strdup()
#include <stdlib.h> // malloc(), free()

#include <glad/glad.h>

void material_init(material_t * material)
{
    assert(material);

    material->name = NULL;
    
    vec4f_init(&material->ambient, 0.0f);
    vec4f_init(&material->diffuse, 0.3f);
    vec4f_init(&material->specular, 0.0f);
    material->shininess = 0.0f;
    material->dissolve = 0.0f;

    for (size_t i = 0; i < 5; ++i) {
        material->texture_map_list[i] = 0;
    }
}

void material_term(material_t * material)
{
    if (!material) {
        return;
    }

    free(material->name);
    material->name = NULL;

    glDeleteTextures(5, material->texture_map_list);

    for (size_t i = 0; i < 5; ++i) {
        material->texture_map_list[i] = 0;
    }
}

void material_bind(material_t * material, GLuint shader)
{
    // Upload material settings to the driver
    glUniform4fv(glGetUniformLocation(shader, "u_Ambient"), 1, material->ambient.data);
    glUniform4fv(glGetUniformLocation(shader, "u_Diffuse"), 1, material->diffuse.data);
    glUniform4fv(glGetUniformLocation(shader, "u_Specular"), 1, material->specular.data);
    glUniform1f(glGetUniformLocation(shader, "u_Shininess"), material->shininess);
    glUniform1f(glGetUniformLocation(shader, "u_Dissolve"), material->dissolve);

    GLint map_location_list[] = {
        glGetUniformLocation(shader, "u_AmbientMap"),
        glGetUniformLocation(shader, "u_DiffuseMap"),
        glGetUniformLocation(shader, "u_SpecularMap"),
        glGetUniformLocation(shader, "u_BumpMap"),
        glGetUniformLocation(shader, "u_ReflectionMap"),
    };

    for (size_t i = 0; i < 5; ++i) {
        // Configure GL_TEXTUREN to point at the texture
        glBindTexture(GL_TEXTURE_2D, material->texture_map_list[i]);
        glActiveTexture(GL_TEXTURE0 + i);
        
        // Configure the map uniform to point to GL_TEXTUREN
        glUniform1ui(map_location_list[i], GL_TEXTURE0 + i);
    }
}