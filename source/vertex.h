#ifndef VERTEX_H
#define VERTEX_H

#include <glmm/glmm.h>

typedef enum
{
    VERTEX_LOCATION_POSITION = 0,
    VERTEX_LOCATION_NORMAL = 1,
    VERTEX_LOCATION_TEXCOORD = 2,

} vertex_location_t;

typedef struct
{
    vec3f_t position;
    vec3f_t normal;
    vec2f_t texcoord;

} vertex_t;

static_assert(
    sizeof(vertex_t) == (sizeof(vec3f_t) + sizeof(vec3f_t) + sizeof(vec2f_t)),
    "vertex_t is not tightly packed"
);

#endif // VERTEX_H