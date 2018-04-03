#include "transforms.h"

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

Translation::Translation(float x, float y, float z) : Transform(x,y,z){
}

void Translation::perform() {
    glTranslatef(getX(), getY(), getZ());
}

Translation::~Translation(void){
}

/* ROTATION DEFINITION */

Rotation::Rotation(float a, float x, float y, float z) : Transform(x,y,z){
    angle = a;
}

void Rotation::perform(){
    glRotatef(angle,getX(),getY(),getZ());
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