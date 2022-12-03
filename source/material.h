#ifndef MATERIAL_H
#define MATERIAL_H

#include <glmm/glmm.h>
#include <glad/glad.h>

typedef struct
{
    char * name;

    vec4f_t ambient;
    vec4f_t diffuse;
    vec4f_t specular;
    float shininess;
    float dissolve;

    union {
        struct {
            GLuint ambient_map;
            GLuint diffuse_map;
            GLuint specular_map;
            GLuint bump_map;
            GLuint reflection_map;
        };

        GLuint texture_map_list[5];
    };
    
} material_t;

void material_init(material_t * material);

void material_term(material_t * material);

void material_bind(material_t * material, GLuint shader);

#endif // MATERIAL_H