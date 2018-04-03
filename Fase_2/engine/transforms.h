#ifndef _TRANSFORMS_H
#define _TRANSFORMS_H

#include<GL/glut.h>

class Transform{

    float x_var;
    float y_var;
    float z_var;

    public:
        Transform();
        Transform(float x, float y, float z);
        float getX();
        float getY();
        float getZ();
        virtual void perform() { };
};

class Translation : public Transform{

public:
    Translation();
    Translation(float x, float y, float z);
    void perform();
    virtual ~Translation(void);
};

class Rotation : public Transform{
    float angle;

public:
    Rotation(float a, float x, float y, float z);
    void perform();
    virtual ~Rotation(void);
};

class Scale : public Transform{

public:
    Scale(float x, float y, float z);
    void perform();
    virtual ~Scale(void);
};

class Colour : public Transform{

public:
    Colour(float r, float g, float b);
    void perform();
    virtual  ~Colour(void);
};

#endif