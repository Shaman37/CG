#include "group.h"

using std::vector;

Group::Group(){
}

Group::Group(vector<Transform*> trfms, vector<Shape*> m, vector<Group*> c){
    transforms = trfms;
    models = m;
    children = c;
}

vector<Transform*> Group::getTransforms(){
    return transforms;
}

vector<Shape*> Group::getModels(){
    return models;
}

vector<Group*> Group::getChilds(){
    return children;
}

void Group::pushTransform(Transform* t){
    transforms.push_back(t);
}

void Group::setModels(vector<Shape*> m_list){
    models = m_list;
}

void Group::pushChild(Group* c){
    children.push_back(c);
}

Group::~Group(void) {
}