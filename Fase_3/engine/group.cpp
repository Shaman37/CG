#include <GL/glew.h>
#include <GL/glut.h>
#include "group.h"

using std::vector;

Group::Group(){

}

vector<Transform*> Group::getTransforms(){
    return transforms;
}

vector<Model*> Group::getModels(){
    return models;
}

vector<Group*> Group::getChilds(){
    return children;
}

void Group::pushTransform(Transform* t){
    transforms.push_back(t);
}

void Group::pushModel(Model*  m){
    models.push_back(m);
}

void Group::pushChild(Group* c){
    children.push_back(c);
}

Group::~Group(void) {
}

void Model::setUp(vector<Vertex*> vert){
    buff_size[0] = vert.size();

    int index = 0;
    float* vertex_array = (float*) malloc(sizeof(float) * (vert.size()) * 3);

    for(Vertex* v: vert){
        vertex_array[index] = v->getX();
        vertex_array[index+1] = v->getY();
        vertex_array[index+2] = v->getZ();
        index+=3;
    }

    glGenBuffers(1, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buff_size[0] * 3, vertex_array, GL_STATIC_DRAW);

    free(vertex_array);
}


void Model::renderModel(){

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, buff_size[0] * 3);
}

Model::~Model(void){
}
