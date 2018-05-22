#ifndef _GROUP_H_
#define _GROUP_H_

#include <GL/glut.h>
#include <vector>
#include "transforms.h"
#include "../src/shape.h"

class Model{

    float  buff_size[1];
    GLuint buffers[1];

    public:
    void setUp(std::vector<Vertex*> vert);
    void renderModel();
    ~Model(void);
};

class Group{

        std::vector<Transform*> transforms;
        std::vector<Model*> models;
        std::vector<Group*> children;

    public:
        Group();
        std::vector<Transform*> getTransforms();
        std::vector<Model*> getModels();
        std::vector<Group*> getChilds();
        void pushTransform(Transform* t);
        void pushModel(Model* model);
        void pushChild(Group* c);
        ~Group(void);
};

#endif