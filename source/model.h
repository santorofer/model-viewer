#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

struct model
{
    float x;
};

bool load_obj(const char * filename);

#endif // MODEL_H