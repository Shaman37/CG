#include "lights.h"

Light::Light(float *p){
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
    pos[3] = p[3];
}

void Light::renderLight() {
    GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat diff[4] = {1.0, 1.0, 1.0, 0};
    GLfloat spec[4] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
}