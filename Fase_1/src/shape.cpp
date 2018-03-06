#include <algorithm>
#include "shape.h"

using std::vector;

void Shape::pushVertex(Vertex* v){
    vertexes.push_back(v);
}

void Shape::pushShape(Shape* s){
    vertexes.insert(vertexes.end(),s->vertexes.begin(),s->vertexes.end());
}

void Shape::getVertex(int i,Vertex** v){
    *v = vertexes.at(i);
}

void Shape::reverse(){
    std::reverse(vertexes.begin() , vertexes.end());
}

int Shape::getSize(){
    return vertexes.size();
}

Shape::~Shape(void){
}