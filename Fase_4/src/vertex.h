#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
#include <string>
#include <cmath>

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
        static Vertex* normalCalc(Vertex* v);
        virtual ~Vertex(void);
};

#endif
