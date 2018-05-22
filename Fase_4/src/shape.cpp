#include <algorithm>
#include "shape.h"

using std::vector;

void Shape::pushVertex(Vertex* v){
    vertexes.push_back(v);
}

void Shape::pushNormal(Vertex* n){
    normals.push_back(n);
}

void Shape::pushTexture(Vertex* t){
    textures.push_back(t);
}

void Shape::pushShape(Shape* s){
    vertexes.insert(vertexes.end(),s->vertexes.begin(),s->vertexes.end());
    //normals.insert(normals.end(),s->normals.begin(),s->normals.end());
    //textures.insert(textures.end(),s->textures.begin(),s->textures.end());
}

void Shape::getVertexAt(int i,Vertex** v, Vertex** n, Vertex** t){
    *v = vertexes.at(i);
    //*n = normals.at(i);
    //*t = textures.at(i);
}

void Shape::reverse(){
    std::reverse(vertexes.begin() , vertexes.end());
}

int Shape::getSize(){
    return vertexes.size();
}

Shape::~Shape(void){
}