#ifndef GENERATOR_PARSER_H
#define GENERATOR_PARSER_H

#include <string>
#include <fstream>
#include <iostream>

#include "tinyxml2.h"
#include "group.h"
#include "transforms.h"
#include "../src/shape.h"
#include "../src/vertex.h"

using namespace tinyxml2;

void parseTranslation(XMLElement* element, Group* g);
void parseRotation(XMLElement* element, Group* g);
void parseScale(XMLElement* element, Group* g);
void parseModels(XMLElement* element, Group* g);
void parseFile(std::string  m_file, Shape* s);
void parseGroup(XMLElement* element, Group* g);
Group* readXML(std::string  f_path, Group* scene);

#endif