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

void parseMaterials(XMLElement* element, Model* m){
    float amb[4] = {0.2, 0.2, 0.2, 1};
    float diff[4] = {0.8, 0.8, 0.8, 1};
    float spec[4] = {0, 0, 0, 1};
    float emi[4] = {0, 0, 0, 1};
    float shin = 0;

    element->QueryFloatAttribute("diffR", &diff[0]);
    element->QueryFloatAttribute("diffG", &diff[1]);
    element->QueryFloatAttribute("diffB", &diff[2]);

    element->QueryFloatAttribute("specR", &spec[0]);
    element->QueryFloatAttribute("specG", &spec[1]);
    element->QueryFloatAttribute("specB", &spec[2]);

    element->QueryFloatAttribute("emiR", &emi[0]);
    element->QueryFloatAttribute("emiG", &emi[1]);
    element->QueryFloatAttribute("emiB", &emi[2]);

    element->QueryFloatAttribute("ambR", &amb[0]);
    element->QueryFloatAttribute("ambG", &amb[1]);
    element->QueryFloatAttribute("ambB", &amb[2]);

    element->QueryFloatAttribute("shini", &shin);

    m->pushMaterials(diff, spec, emi, amb, shin);
}

void parseModels(XMLElement* element, Group* g){
    vector<Model*> models;
    string f_path, t_path;

    for(element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {

        Model* m = new Model();

        if(element->Attribute("texture")) {
            t_path = element->Attribute("texture");
            m->setUpTexture(t_path);
        }

        f_path = element->Attribute("file");
        parseFile(f_path,m);

        parseMaterials(element,m);

        g->pushModel(m);
    }
}

void  parseFile(string m_file, Model* m) {
    string line;
    vector<Vertex *> v_list, n_list, t_list;

    ifstream file(m_file);

    if (file.fail()) {
        cout << "Couldn't find file: " << m_file << "." << endl;
    } else {
        while (getline(file, line)){

            if (line[0] == 'V'){
                line.erase(0, 2);
                Vertex *v = new Vertex(line);
                v_list.push_back(v);
            }

            if (line[0] == 'N'){
                line.erase(0, 2);
                Vertex *n = new Vertex(line);
                n_list.push_back(n);
            }

            if (line[0] == 'T'){
                line.erase(0, 2);
                Vertex *t = new Vertex(line);
                t_list.push_back(t);
            }
        }

        m->setUpVBO(v_list, n_list, t_list);
        file.close();
    }
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

void parseLights(XMLElement* element, Group* g){
    float *p = {};

    element->QueryFloatAttribute("posX", &p[0]);
    element->QueryFloatAttribute("posY", &p[1]);
    element->QueryFloatAttribute("posZ", &p[2]);

    if (!strcmp(element->Attribute("type"), "POINT")) p[3] = 1.0f;

    Light* l = new Light(p);

    g->pushLight(l);
}

void readXML(string  f_path, Group* scene, vector<Translation*>* orbits){
    XMLDocument xmlDoc;
    XMLElement *element;

    if (!(xmlDoc.LoadFile(f_path.c_str()))) {
        element = xmlDoc.FirstChildElement("scene")->FirstChildElement("lights");
        if(element) parseLights(element,scene);
        element = xmlDoc.FirstChildElement("scene")->FirstChildElement("group");
        parseGroup(element,scene,orbits);
        }
    else {
        cout << "XML File " + f_path + " could not be found" << endl;
    }
}

