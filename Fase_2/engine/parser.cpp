#include "parser.h"

using namespace std;

void parseTranslation(XMLElement* element, Group* g){
    float x = 0;
    float y = 0;
    float z = 0;

    element->QueryFloatAttribute("X",&x);
    element->QueryFloatAttribute("Y",&y);
    element->QueryFloatAttribute("Z",&z);

    Translation* t = new Translation(x,y,z);
    g->pushTransform(t);
}

void parseRotation(XMLElement* element, Group* g){
    float x = 0;
    float y = 0;
    float z = 0;
    float angle = 0;

    element->QueryFloatAttribute("angle",&angle);
    element->QueryFloatAttribute("X",&x);
    element->QueryFloatAttribute("Y",&y);
    element->QueryFloatAttribute("Z",&z);

    Rotation* r = new Rotation(angle,x,y,z);
    g->pushTransform(r);
}

void parseScale(XMLElement* element, Group* g){
    float x = 0;
    float y = 0;
    float z = 0;

    element->QueryFloatAttribute("X",&x);
    element->QueryFloatAttribute("Y",&y);
    element->QueryFloatAttribute("Z",&z);

    Scale* s = new Scale(x,y,z);
    g->pushTransform(s);
}

void parseColour(XMLElement* element, Group *g){
    float red = 0;
    float green = 0;
    float blue = 0;

    element->QueryFloatAttribute("R",&red);
    element->QueryFloatAttribute("G",&green);
    element->QueryFloatAttribute("B",&blue);

    Colour* c = new Colour(red,green,blue);
    g->pushTransform(c);
}

void parseModels(XMLElement* element, Group* g){
    vector<Shape*> models;
    string path;

    for(element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {
        Shape* s = new Shape();

        path = element->Attribute("file");
        parseFile(path,s);
        models.push_back(s);
    }

    g->setModels(models);
}

void parseFile(string m_file, Shape* s){
    string line;
    ifstream file(m_file);

    if (file.fail()) {
        cout<< "Couldn't find file: " << m_file << "." << endl;
    }

    else {
        while (getline(file, line)) {
            Vertex *v = new Vertex(line);
            s->pushVertex(v);
        }
    }

    file.close();
}

void parseGroup(XMLElement* element, Group* g){
    XMLElement* current = element;

    if (!(strcmp(element->Name(), "translate")))
        parseTranslation(element,g);
    else if (!(strcmp(element->Name(), "rotate")))
        parseRotation(element,g);
    else if (!(strcmp(element->Name(), "scale")))
        parseScale(element,g);
    else if (!(strcmp(element->Name(), "models")))
        parseModels(element,g);
    else if (!(strcmp(element->Name(), "colour")))
        parseColour(element,g);
    else if (!(strcmp(element->Name(), "group"))) {
        Group *child = new Group();
        g->pushChild(child);
        element = element->FirstChildElement();

        parseGroup(element,child);
    }

    current = current->NextSiblingElement();
    if(current) parseGroup(current,g);
}

Group* readXML(string  f_path, Group* scene){
    XMLDocument xmlDoc;
    XMLElement *element;

    if (!(xmlDoc.LoadFile(f_path.c_str()))) {
        element = xmlDoc.FirstChildElement("scene")->FirstChildElement(); //ROOT ELEMENT (<group>)

        parseGroup(element, scene);
    }
    else {
        cout << "XML File " + f_path + " could not be found" << endl;
    }

    return scene;
}

