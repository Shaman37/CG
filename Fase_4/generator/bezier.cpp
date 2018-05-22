#include "bezier.h"

using namespace std;

void processPatch(int tess, int n);
Vertex* bezierCalculate(float u, float v, int n);
Vertex* vertexCalculate(float t, float *p1, float *p2, float *p3, float *p4);

int p_size;
int* patches;
Shape* model = new Shape();
vector<Vertex*> controls;

Shape* bezier_Parse(string patch_path, int tesselation){
    string line;
    ifstream file(patch_path);

    if (file.fail()) {
        cout<< "Couldn't find file: " << patch_path  << "." << endl;
    }

    getline(file,line);

    p_size = stoi(line);
    patches = (int *) malloc (sizeof(int) * p_size*16);

    for(int i= 0; i != p_size; i++){
        getline(file,line);

        for(int j = 0; j< 16; j++){
            int pos = line.find(",");
            string token = line.substr(0,pos);
            patches[(j+(16*i))] = atoi(token.c_str());
            line.erase(0,pos+1);
        }
    }

    getline(file,line);

    while(getline(file,line)){
        Vertex* v = new Vertex(line);
        controls.push_back(v);
    }

    for(int i=0; i != p_size; i++){
        processPatch(tesselation, i);
    }

    return model;
}

void processPatch(int tess, int n) {
    float inc = 1.0 / tess, u, v, u2, v2;

    for (int i = 0; i < tess; i++) {
        for (int j = 0; j < tess; j++) {
            u = i * inc;
            v = j * inc;
            u2 = (i + 1) * inc;
            v2 = (j + 1) * inc;

            Vertex *p1 = bezierCalculate(u,v,n);
            Vertex *p2 = bezierCalculate(u,v2,n);
            Vertex *p3 = bezierCalculate(u2,v,n);
            Vertex *p4 = bezierCalculate(u2,v2,n);

            model->pushVertex(p1);
            model->pushVertex(p3);
            model->pushVertex(p4);

            model->pushVertex(p1);
            model->pushVertex(p4);
            model->pushVertex(p2);
        }
    }
}

Vertex* bezierCalculate(float u, float v, int n_patch){
    float bz_p[4][3], res[4][3];
    int j = 0, k = 0;
    n_patch = (16*n_patch);

    for(int i = 0; i < 16; i++){
        bz_p[j][0] = controls.at(patches[n_patch+i])->getX();
        bz_p[j][1] = controls.at(patches[n_patch+i])->getY();
        bz_p[j][2] = controls.at(patches[n_patch+i])->getZ();

        j++;

        if( j % 4 == 0){
            Vertex *v = vertexCalculate(u,bz_p[0],bz_p[1],bz_p[2],bz_p[3]);

            res[k][0] = v->getX();
            res[k][1] = v->getY();
            res[k][2] = v->getZ();

            k++;
            j=0;
        }
    }

    return vertexCalculate(v,res[0],res[1],res[2],res[3]);
}

Vertex* vertexCalculate(float t, float *p1, float *p2, float *p3, float *p4){
    float coords[3];
    float ite = 1.0 -t;

    float bz1= ite * ite * ite;
    float bz2 = 3 * t * ite * ite;
    float bz3 = 3 * t * t * ite;
    float bz4 = t * t * t;

    coords[0] = bz1*p1[0] + bz2*p2[0] + bz3*p3[0] + bz4*p4[0];
    coords[1] = bz1*p1[1] + bz2*p2[1] + bz3*p3[1] + bz4*p4[1];
    coords[2] = bz1*p1[2] + bz2*p2[2] + bz3*p3[2] + bz4*p4[2];

    return (new Vertex(coords[0],coords[1],coords[2]));
}