#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <GL/glut.h>

class Light {
    float pos[4];

public:
    Light(float *p);
    void renderLight();
};

#endif