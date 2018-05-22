#include "transforms.h"
#include<GL/glut.h>

void catmullRomPoint(float t, float *p, std::vector<Vertex*> points);
void catmullCalculate(float t, int* index, float* p, std::vector<Vertex*> points);

Transform::Transform(){
}

Transform::Transform(float x, float y, float z){
    x_var = x;
    y_var = y;
    z_var = z;
}

float Transform::getX(){
    return x_var;
}

float Transform::getY(){
    return y_var;
}

float Transform::getZ(){
    return z_var;
}

/* TRANSLATION DEFINITION */

Translation::Translation(float x, float y, float z,float t) : Transform(x,y,z){
    time = t;
}

void catmullRomPoint(float t, float *p, std::vector<Vertex*> points){
    int size = points.size();

    float rt = t*size;
    int i = floor(rt);
    rt = rt - i;

    int index[4];
    index[0] = (i  + size - 1) % size;
    index[1] = (index[0]  + 1) % size;
    index[2] = (index[1]  + 1) % size;
    index[3] = (index[2]  + 1) % size;

    catmullCalculate(rt,index,p,points);
}

void catmullCalculate(float t, int* index, float* p, std::vector<Vertex*> points){
    float aux[4];
    float t_2 = t*t;
    float t_3 = t*t*t;

    float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
                                  { 1.0f, -2.5f, 2.0f, -0.5f },
                                  { -0.5f, 0.0f, 0.5f, 0.0f },
                                  { 0.0f, 1.0f, 0.0f, 0.0f }};

    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 0.0;

    aux[0] = t_3*m[0][0] + t_2*m[1][0] + t*m[2][0] + m[3][0];
    aux[1] = t_3*m[0][1] + t_2*m[1][1] + t*m[2][1] + m[3][1];
    aux[2] = t_3*m[0][2] + t_2*m[1][2] + t*m[2][2] + m[3][2];
    aux[3] = t_3*m[0][3] + t_2*m[1][3] + t*m[2][3] + m[3][3];


    int i_0 = index[0];
    int i_1 = index[1];
    int i_2 = index[2];
    int i_3 = index[3];
    Vertex* v0 = points[i_0];
    Vertex* v1 = points[i_1];
    Vertex* v2 = points[i_2];
    Vertex* v3 = points[i_3];

    p[0] = aux[0] * v0->getX() + aux[1] * v1->getX() + aux[2] * v2->getX() + aux[3] * v3->getX();
    p[1] = aux[0] * v0->getY() + aux[1] * v1->getY() + aux[2] * v2->getY() + aux[3] * v3->getY();
    p[2] = aux[0] * v0->getZ() + aux[1] * v1->getZ() + aux[2] * v2->getZ() + aux[3] * v3->getZ();
}

void Translation::generateCurve(){
    float p[3];

    if(catmull_points.size() != 0) {
        for (float gt = 0; gt < 1; gt += 0.01) {
            catmullRomPoint(gt, p, catmull_points);
            curve_points.push_back(new Vertex(p[0], p[1], p[2]));
        }
    }
    else return;
}

int Translation::getCurveSize(){
    return catmull_points.size();
}

void Translation::drawCurve() {
    if (curve_points.size() != 0) {

        glBegin(GL_LINE_LOOP);

        for (Vertex *v: curve_points) {
            glVertex3f(v->getX(), v->getY(), v->getZ());
        }

        glEnd();
    }
}

void Translation::addPoint(Vertex* v){
    catmull_points.push_back(v);
}

void Translation::perform() {
    float coord[3] = {getX(),getY(),getZ()};

    if(time>0){
        float elapsed = glutGet(GLUT_ELAPSED_TIME) % (int) (time * 1000);
        float rt = elapsed/(time*1000);

        catmullRomPoint(rt,coord,catmull_points);
    }

    glTranslatef(coord[0], coord[1], coord[2]);
}

Translation::~Translation(void){

}

/* ROTATION DEFINITION */

Rotation::Rotation(float t, float a, float x, float y, float z) : Transform(x,y,z){
    angle = a;
    time = t;
}

void Rotation::perform(){
    float curr_ang = angle;

    if(time > 0){
        float elapsed = glutGet(GLUT_ELAPSED_TIME) % (int) (time * 1000);
        curr_ang = (elapsed *360) / (time * 1000);
    }

    glRotatef(curr_ang,getX(),getY(),getZ());
}

Rotation::~Rotation(void){

}

/* SCALE DEFINITION */

Scale::Scale(float x, float y, float z) : Transform(x,y,z){

}

void Scale::perform(){
    glScalef(getX(),getY(),getZ());
}

Scale::~Scale(void){
}

/* COLOUR DEFINITION */

Colour::Colour(float r, float g, float b):Transform(r,g,b){

}

void Colour::perform(){
    float r = getX()/255;
    float g = getY()/255;
    float b = getZ()/255;
    glColor3f(r,g,b);
}

Colour::~Colour(void){

}