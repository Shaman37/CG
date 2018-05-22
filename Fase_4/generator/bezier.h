#ifndef GENERATOR_BEZIER_H
#define GENERATOR_BEZIER_H

#include "../src/shape.h"
#include <string>
#include <fstream>
#include <iostream>

Shape* bezier_Parse(std::string patch_path, int tesselation);

#endif
