#include <algorithm>
#include "shape.h"

using std::vector;

void Shape::pushVertex(Vertex* v){
    vertexes.push_back(v);
}

void Shape::pushShape(Shape* s){
    vertexes.insert(vertexes.end(),s->vertexes.begin(),s->vertexes.end());
}

void Shape::getVertexAt(int i,Vertex** v){
    *v = vertexes.at(i);
}

void Shape::reverse(){
    std::reverse(vertexes.begin() , vertexes.end());
}

int Shape::getSize(){
    return vertexes.size();
}

vector<Vertex*>  Shape::getVertexes(){
    return vertexes;
}

void Shape::draw(GLenum draw_mode){
    vector<Vertex *> v_list = getVertexes();
    glPolygonMode(GL_FRONT_AND_BACK,
                  draw_mode); // Changes how our shapes are represented (drawn). Lines, Points or Filled
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < getSize(); i++) {
        glVertex3f(v_list[i]->getX(), v_list[i]->getY(), v_list[i]->getZ());
    }

    glEnd();
}
Shape::~Shape(void){
}