#ifndef _GROUP_H_
#define _GROUP_H_

#include <vector>
#include "transforms.h"
#include "../src/shape.h"

class Group {

        std::vector<Transform*> transforms;
        std::vector<Shape*> models;
        std::vector<Group*> children;

    public:
        Group();
        Group(std::vector<Transform*> trfms, std::vector<Shape*> m, std::vector<Group*> c);
        std::vector<Transform*> getTransforms();
        std::vector<Shape*> getModels();
        std::vector<Group*> getChilds();
        void pushTransform(Transform* t);
        void setModels(std::vector<Shape*> m_list);
        void pushChild(Group* c);
        ~Group(void);
};

#endif