#ifndef _MODELS_H
#define _MODELS_H

#include <vector>
#include <math.h>
#include "../src/shape.h"

Shape* createPlane(float size);
Shape* createBox(float width, float height, float length, int div);
Shape* createCone(float radius, float height, int slices, int stacks);
Shape* createCylinder(float radius, float height, int slices, int stacks);
Shape* createSphere(float radius, int slices, int stacks);
Shape* createTorus(float distance, float radius, int slices, int stacks);
#endif




