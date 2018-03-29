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
        void setX(float x);
        void setY(float y);
        void setZ(float z);
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
    float getAngle();
    void setAngle(float a);
    void perform();
    virtual ~Rotation(void);
};

class Scale : public Transform{

public:
    Scale(float x, float y, float z);
    void perform();
    virtual ~Scale(void);
};

#endif

