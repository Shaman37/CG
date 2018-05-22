#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <vector>
#include "vertex.h"

class Shape {

private:
    std::vector<Vertex*> vertexes;
    std::vector<Vertex*> normals;
    std::vector<Vertex*> textures;

public:
    void pushVertex(Vertex* v);
    void pushNormal(Vertex* n);
    void pushTexture(Vertex* t);
    void pushShape(Shape* s);
    void getVertexAt(int i, Vertex** v,Vertex** n, Vertex** t);
    void reverse();
    int getSize();
    virtual ~Shape(void);
};

#endif