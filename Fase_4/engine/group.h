#ifndef _GROUP_H_
#define _GROUP_H_

#include <GL/glut.h>
#include <IL/il.h>
#include <vector>
#include "transforms.h"
#include "lights.h"
#include "../src/shape.h"

class Model{
    float  buff_size[3];
    GLuint texture, buffers[3];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;

    public:
    void setUpVBO(std::vector<Vertex*> vert,std::vector<Vertex*> norm,std::vector<Vertex*> tex);
    void setUpTexture(std::string t_file);
    void renderModel();
    void pushMaterials(float *d, float *s, float *e, float *a, float shine);
    ~Model(void);
};

class Group{
    std::vector<Transform*> transforms;
    std::vector<Model*> models;
    std::vector<Group*> children;
    std::vector<Light*> lights;

public:
    Group();
    std::vector<Transform*> getTransforms();
    std::vector<Model*> getModels();
    std::vector<Group*> getChilds();
    std::vector<Light*> getLights();
    void pushTransform(Transform* t);
    void pushModel(Model* model);
    void pushChild(Group* c);
    void pushLight(Light* l);
    ~Group(void);
};

class Bullet{
    float posX;
    float posY;
    float posZ;
    float init_h_angle;
    float init_v_angle;
    float life;

public:
    Bullet(float x, float y, float z, float i_h_angle, float i_v_angle);
    void move();
    float getPosX(){return posX;}
    float getPosY(){return posY;}
    float getPosZ(){return posZ;}
    float getLife(){return life;}
    void setLife(float l){life = l;}
};

class Asteroid{
    float posX;
    float posY;
    float posZ;
    float angle;
    float speed;
    float radius;
    bool hit;

public:
    Asteroid(float r, float y, float a, float s);
    void rotate();
    float getX(){return posX;}
    float getY(){return posY;}
    float getZ(){return posZ;}
    bool checkHit(){return hit;}
    void flagHit(){hit = true;}
};

#endif