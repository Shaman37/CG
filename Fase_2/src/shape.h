#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <vector>
#include <GL/glut.h>
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
        std::vector<Vertex*> getVertexes();
         void draw(GLenum draw_mode);
        virtual ~Shape(void);
};

#endif
