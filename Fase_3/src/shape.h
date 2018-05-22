#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <vector>
#include "vertex.h"

class Shape {

private:
    std::vector<Vertex*> vertexes;

public:
    void pushVertex(Vertex* v);
    void pushShape(Shape* s);
    void getVertexAt(int i, Vertex** v);
    void reverse();
    int getSize();
    virtual ~Shape(void);
};

#endif