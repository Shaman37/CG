#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <fstream>
#include <iostream>

#include "tinyxml2.h"
#include "group.h"
#include "transforms.h"
#include "../src/shape.h"
#include "../src/vertex.h"

using namespace tinyxml2;

void parseTranslation(XMLElement* element, Group* g,std::vector<Translation*>* orbits);
void parseRotation(XMLElement* element, Group* g);
void parseScale(XMLElement* element, Group* g);
void parseModels(XMLElement* element, Group* g);
void parseFile(std::string  m_file, Model* m);
void parseGroup(XMLElement* element, Group* g,std::vector<Translation*>* orbits);
void readXML(std::string  f_path, Group* scene,std::vector<Translation*>* orbits);

#endif