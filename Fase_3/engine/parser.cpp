#include "parser.h"

using namespace std;

void parseTranslation(XMLElement* element, Group* g,vector<Translation*>* o){
    float x = 0;
    float y = 0;
    float z = 0;
    float time = 0;

    element->QueryFloatAttribute("time",&time);
    element->QueryFloatAttribute("X",&x);
    element->QueryFloatAttribute("Y",&y);
    element->QueryFloatAttribute("Z",&z);

    Translation* t = new Translation(x,y,z,time);

    XMLElement* t_points = element->FirstChildElement();

    for(;t_points;t_points = t_points->NextSiblingElement()){
        float px = 0;
        float py = 0;
        float pz = 0;

        t_points->QueryFloatAttribute("X",&px);
        t_points->QueryFloatAttribute("Y",&py);
        t_points->QueryFloatAttribute("Z",&pz);

        Vertex* p = new Vertex(px,py,pz);
        t->addPoint(p);
    }

    if(t->getCurveSize() != 0) {t->generateCurve(); o->push_back(t);}

    g->pushTransform(t);
}

void parseRotation(XMLElement* element, Group* g){
    float x = 0;
    float y = 0;
    float z = 0;
    float angle = 0;
    float time = 0;

    element->QueryFloatAttribute("time",&time);
    element->QueryFloatAttribute("angle",&angle);
    element->QueryFloatAttribute("X",&x);
    element->QueryFloatAttribute("Y",&y);
    element->QueryFloatAttribute("Z",&z);

    Rotation* r = new Rotation(time,angle,x,y,z);
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
    vector<Model*> models;
    string path;

    for(element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {

        Model* m = new Model();

        path = element->Attribute("file");
        parseFile(path,m);

        g->pushModel(m);
    }

}

void  parseFile(string m_file, Model* m){
    string line;
    vector<Vertex*> v_list;

    ifstream file(m_file);

    if (file.fail()) {
        cout<< "Couldn't find file: " << m_file << "." << endl;
    }

    else {
        while (getline(file, line)) {
            Vertex *v = new Vertex(line);
            v_list.push_back(v);
        }
    }

    m->setUp(v_list);

    file.close();
}

void parseGroup(XMLElement* element, Group* g,vector<Translation*>* o){
    XMLElement* current = element;

    if (!(strcmp(element->Name(), "translate")))
        parseTranslation(element,g,o);
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

        parseGroup(element,child,o);
    }

    current = current->NextSiblingElement();
    if(current) parseGroup(current,g,o);
}

void readXML(string  f_path, Group* scene, vector<Translation*>* orbits){
    XMLDocument xmlDoc;
    XMLElement *element;

    if (!(xmlDoc.LoadFile(f_path.c_str()))) {
        element = xmlDoc.FirstChildElement("scene")->FirstChildElement(); //ROOT ELEMENT (<group>
        parseGroup(element,scene,orbits);
    }
    else {
        cout << "XML File " + f_path + " could not be found" << endl;
    }
}

