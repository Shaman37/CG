#include "../src/vertex.h"
#include "../src/shape.h"
#include "models.h"

#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;
using std::vector;
using std::ofstream;

void generatePlane(char* s, char* f_path);
void generateBox(char*width, char*height, char*length, char*div, char* f_path);
void generateCone(char* radius, char* height, char* slices, char* stacks, char* f_path);
void generateSphere(char*radius, char* slices, char* stacks, char* f_path);
void generateCylinder(char* radius, char* height, char* slices, char* stacks, char* f_path);
void writeFile(Shape* s, string f_path);
void printGuide();

int main(int argc, char** argv){
    if (argc == 1) printGuide();
        // argv[1] = plane -- argv[2] = size -- argv[3] = file path
    else if (!(strcmp(argv[1],"plane")) && (argc == 4))
        generatePlane(argv[2],argv[3]);

        // argv[1] = box -- argv[2] = X -- argv[3] = Y -- argv[4] = Z -- argv[5] = Div -- argv[6] = file path
    else if(!(strcmp(argv[1],"box")) && (argc == 7))
        generateBox(argv[2],argv[3],argv[4],argv[5],argv[6]);

        // argv[1] = cone -- argv[2] = radius -- argv[3] = height -- argv[4] = Slices -- argv[5] = Stacks -- argv[6] = file path
    else if(!(strcmp(argv[1],"cone")) && (argc == 7))
        generateCone(argv[2],argv[3],argv[4],argv[5],argv[6]);

        // argv[1] = sphere -- argv[2] = radius -- argv[3] = Slices -- argv[4] = Stacks -- argv[5] = file path
    else if(!(strcmp(argv[1],"sphere")) && (argc == 6))
        generateSphere(argv[2],argv[3],argv[4],argv[5]);

        // argv[1] = cylinder -- argv[2] = radius -- argv[3] = height -- argv[4] = Slices -- argv[5] = Stacks -- argv[6] = file path
    else if(!(strcmp(argv[1],"cylinder")) && (argc == 7))
        generateCylinder(argv[2],argv[3],argv[4],argv[5],argv[6]);

    else if(!(strcmp(argv[1],"-help")) || !(strcmp(argv[1],"-h")))
        printGuide();

    return 0;
}

// Generates a Plane of size "s", to be saved on a file named "f_path"
void generatePlane(char* s, char* f_path){
    float size = atof(s);
    ofstream file;

    Shape* p = createPlane(size);
    writeFile(p,f_path);
}

// Generates a Box with the specified size and face divisions, to be saved on a file named "f_path"
void generateBox(char* width, char* height, char* length, char* div, char* f_path){
    float x,y,z;
    int n;

    x = atof(width);
    y = atof(height);
    z = atof(length);
    n = atoi(div);

    ofstream file;

    Shape* b = createBox(x,y,z,n);
    writeFile(b,f_path);
}

// Generates a Cone with the specified base and height, to be saved on a file named "f_path"
void generateCone(char* radius, char* height, char* slices, char* stacks, char* f_path){
    float r,h;
    int sl, st;

    r = atof(radius);
    h = atof(height);
    sl = atoi(slices);
    st = atoi(stacks);

    if((sl < 9) || (st == 0)){
        std::cout << "The options you introduced to create a cone aren't viable... "<< std::endl;
        std::cout << ">> Recomendations: Try using at least 15 slices and 1 stack for a smoother model" << std::endl;
    }
    else {
        ofstream file;

        Shape *c = createCone(r, h, sl, st);
        writeFile(c, f_path);
    }
}

// Generates a Sphere with the specified radius, to be saved on a file named "f_path"
void generateSphere(char*radius, char* slices, char* stacks, char* f_path) {
    float r;
    int sl, st;

    r = atof(radius);
    sl = atoi(slices);
    st = atoi(stacks);

    if ((sl < 15) || (st < 10)) {
        std::cout << "\n" << std::endl;
        std::cout << "The options you introduced to create a sphere  aren't viable... " << std::endl;
        std::cout << ">> Recomendations: Try using at least 15 slices and 10 stacks for a smoother model" << std::endl;
    }
    else {
        ofstream file;

        Shape *sp = createSphere(r, sl, st);
        writeFile(sp, f_path);
    }
}

// Generates a Cylinder with the specified radius and height, to be saved on a file named "f_path"
void generateCylinder(char* radius, char* height, char* slices, char* stacks, char* f_path) {
    float r, h;
    int sl, st;

    r = atof(radius);
    h = atof(height);
    sl = atoi(slices);
    st = atoi(stacks);

    if ((sl < 15) || (st == 0)) {
        std::cout << "\n" << std::endl;
        std::cout << "The options you introduced to create a cylinder  aren't viable... " << std::endl;
        std::cout << ">> Recomendations: Try using at least 15 slices and 1 stack for a smoother model" << std::endl;
    }
    else {
        ofstream file;

        Shape *cl = createCylinder(r, h, sl, st);
        writeFile(cl, f_path);
    }
}

// Writes all our Shape's Vertexes on the desired file ("f_path")
void writeFile(Shape* s, string f_path){
    char buff[1024];
    int i;
    int size = s->getSize();
    Vertex* v;

    system("mkdir -p ../files3d/ ");
    string path = "../files3d/" + f_path;
    ofstream file (path,std::ofstream::out);

    for(i=0;i<size;i++){
        s->getVertex(i,&v);
        sprintf(buff, "%f %f %f\n", v->getX(), v->getY(), v->getZ());
        file << buff;
    }

    file.close();
}

// Prints the Generator's Guide to success!

void printGuide() {
    std::cout << "              ---- Generator [GUIDE] ----        " << std::endl;
    std::cout << "Guidelines: generate <shape> [options] <file>" << std::endl;
    std::cout << "-.. Shapes and Options: ..- " << std::endl;
    std::cout << "\t-> plane <size>" << std::endl;
    std::cout << "\t-> box <width> <height> <length> <divisions>" << std::endl;
    std::cout << "\t-> sphere <radius> <slices> <stacks>" << std::endl;
    std::cout << "\t-> cone <radius> <height> <slices> <stacks>" << std::endl;
    std::cout << "\t-> -(For an inverted cone, use a negative height)-" << std::endl;
    std::cout << "\t-> cylinder <radius> <height> <slices> <stacks>" << std::endl;
}