#include <GL/glew.h>
#include <GL/glut.h>
#include "group.h"

using std::vector;

Group::Group(){
}

vector<Transform*> Group::getTransforms(){
    return transforms;
}

vector<Model*> Group::getModels(){
    return models;
}

vector<Group*> Group::getChilds(){
    return children;
}

vector<Light*> Group::getLights(){
    return lights;
}

void Group::pushTransform(Transform* t){
    transforms.push_back(t);
}

void Group::pushModel(Model*  m){
    models.push_back(m);
}

void Group::pushChild(Group* c){
    children.push_back(c);
}

void Group::pushLight(Light* l){
    lights.push_back(l);
}

Group::~Group(void) {
}

void Model::pushMaterials(float d[4], float s[4], float e[4], float a[4] , float shine){
    ambient[0] = a[0];
    ambient[1] = a[1];
    ambient[2] = a[2];
    ambient[3] = a[3];

    diffuse[0] = d[0];
    diffuse[1] = d[1];
    diffuse[2] = d[2];
    diffuse[3] = d[3];

    specular[0] = s[0];
    specular[1] = s[1];
    specular[2] = s[2];
    specular[3] = s[3];

    emission[0] = e[0];
    emission[1] = e[1];
    emission[2] = e[2];
    emission[3] = e[3];

    shininess = shine;
}

void Model::setUpVBO(vector<Vertex*> vert, vector<Vertex*> norm, vector<Vertex*> tex){
    buff_size[0] = vert.size();
    buff_size[1] = norm.size();
    buff_size[2] = tex.size();

    float* vertex_array = (float*) malloc(sizeof(float) * (vert.size()) * 3);
    float* normal_array = (float*) malloc(sizeof(float) * (norm.size()) * 3);
    float* texture_array = (float*) malloc(sizeof(float) * (tex.size()) * 2);

    int index = 0;
    for(Vertex* v: vert){
        vertex_array[index] = v->getX();
        vertex_array[index+1] = v->getY();
        vertex_array[index+2] = v->getZ();
        index+=3;
    }

    index = 0;
    for(Vertex* n: norm){
        normal_array[index] = n->getX();
        normal_array[index+1] = n->getY();
        normal_array[index+2] = n->getZ();
        index+=3;
    }

    index = 0;
    for(Vertex* t: tex){
        texture_array[index] = t->getX();
        texture_array[index+1] = t->getY();
        index+=2;
    }

    glGenBuffers(3, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buff_size[0] * 3, vertex_array, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buff_size[1] * 3, normal_array, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buff_size[2] * 2, texture_array, GL_STATIC_DRAW);

    free(vertex_array);
    free(normal_array);
    free(texture_array);
}

void Model::setUpTexture(std::string t_file){
    unsigned int t,tw,th;
    unsigned char *texData;

    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) t_file.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::renderModel(){
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if(buff_size[1]){
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    if(buff_size[2]){
        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    glDrawArrays(GL_TRIANGLES, 0, buff_size[0] * 3);
    glBindTexture(GL_TEXTURE_2D, 0);

}

Model::~Model(void){
}

Bullet::Bullet(float x, float y, float z, float i_h_angle, float i_v_angle){
    posX = x;
    posY = y;
    posZ = z;
    init_h_angle = i_h_angle;
    init_v_angle = i_v_angle;
    life = 150;
}

void Bullet::move(){
    float xrot, yrot;
    yrot = (init_v_angle / 180.0f * M_PI);
    xrot = (init_h_angle / 180.0f * M_PI);

    posX += sin(yrot);
    posZ -= cos(yrot);
    posY -= sin(xrot);

    life--;
}

Asteroid::Asteroid(float r, float y, float a, float s){
    posX = cos(a)*r;
    posY = y;
    posZ = sin(a)*r;
    angle = a;
    speed = s;
    radius = r;
    hit = false;
}

void Asteroid::rotate(){
    angle += speed;
    if(angle > 360) angle -= 360;

    posX = cos(angle) * radius;
    posZ = sin(angle) * radius;
}