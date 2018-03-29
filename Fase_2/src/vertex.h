#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <string>

class Vertex {

    private:
        float x;
        float y;
        float z;

    public:
        Vertex();
        Vertex(float xx, float yy, float zz);
        Vertex(const Vertex &p);
        Vertex(std::string str);
        float getX();
        float getY();
        float getZ();
        virtual ~Vertex(void);
};

#endif
