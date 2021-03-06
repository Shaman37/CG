#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <GL/glut.h>

#include "tinyxml2.h"
#include "../src/vertex.h"

using namespace tinyxml2;
using namespace std;
using std::vector;

float camX = 10, camY = 5, camZ = 10; // Camera var's
float r_x = 1,r_y = 1; // Rotation var's
float t_x,t_y,t_z; // Translation var's
float axes_x, axes_y, axes_z; // Axis length var's
float R2 = 1, B2 = 1, G2 = 1; // Triangle color
float R3 = 1, B3 = 1, G3 = 1; // Triangle secondary color

vector<Vertex*> models; // Global struct
int draw_mode = GL_LINE;

// Prints our Engine's Guide to Success!
void printHelp(){
    cout << "#____________________________________ENGINE [GUIDE__]________________________________ #" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "| Usage: ./engine {XML FILE}                                                                                   |" << endl;
    cout << "|                 [-h]                                                                                                              |" << endl;
    cout << "|                                                                                                                                     |" << endl;
    cout << "|   FILE:                                                                                                                        |" << endl;
    cout << "|   Specify a path to an XML file in which the information about         |" << endl;
    cout << "|   the models you wish to create are specified                                          |" << endl;
    cout << "|                                                                                                                                     |" << endl;
    cout << "|   MOVE:                                                                                                                        |" << endl;
    cout << "|   w: Move UP (Translates upward, through the Y axis                                  |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   s: Move DOWN (Translates downward, through the Y axis                         |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   a: Move LEFT (Translates leftward, through the X axis (negative)    |" << endl;
    cout << "|       and through the Z axis (positive)                                                             |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   d: Move RIGHT (Translates rightward through the X axis (positive)|" << endl;
    cout << "|   and through the Z axis (negative)                                                                 |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   1 : Increase X axis length                                                                                |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   2 : Increase Y axis length                                                                                |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   3 : Increase Z axis length                                                                                |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   + : Zoom In                                                                                                                |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   - : Zoom Out                                                                                                              |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   COLOR:                                                                                                                       |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   r : Red and White Color                                                                                       |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   g :  Green and White Color                                                                                  |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   b : Blue and White Color                                                                                      |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   FORMAT:                                                                                                                     |" << endl;
    cout << "|   p: Change the figure format into points                                                     |" << endl;
    cout << "|                                                                                                                                       |" << endl;
    cout << "|   l: Change the figure format into lines                                                       |" << endl;
    cout << "|                                                                                                                                       |" << endl;
    cout << "|   o: Fill up the figure                                                                                           |" << endl;
    cout << "#__________________________________________________________________________________________#" << endl;
}

// Responsible for our Window Reshaping
void changeSize(int w, int h) {

    if (h == 0)
        h = 1;

    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Responsible for our XYZ Axis Drawing
void drawAxis(){

    // Draw mode of Axis's Arrow Heads (FILLED)
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    // Arrow Head of X Axis (RED)
    glRotatef(90.f,0,1,0); // Rotation of 90º Degrees, relative to the Y Axis... Now our Z axis, assumes the initial position of the X axis.
    glTranslatef(0,0,5+axes_x); // Translation of "5+axes_x", relative to the Z Axis
    glColor3f(1.0f,0.0f,0.0f); // Color of our X Arrow Head
    glutSolidCone(0.1,0.3,5,5); // Arrow Head is drawn, as a solid cone. Base on Z=0, Height on Z=0.3, Radius of 0.3 {5 slices and 5 stacks}.
    
    // This cone stays in the previously  translated position. In this case "5+axes_x"
    glTranslatef(0,0,-5-axes_x); // Translates to the initial position - origin
    glRotatef(-90.f,0,1,0); // Rotates to the initial position - origin

    // Arrow Head of Y Axis (GREEN)
    glRotatef(-90.f,1,0,0); // Rotation of -90º Degrees, relative to the X Axis... Now our Z axis, assumes the initial position of the Y axis.
    glTranslatef(0,0,5+axes_y); // Translation of "5+axes_y", relative to the Z Axis
    glColor3f(0.0f,1.0f,0.0f); // Color of our Y Arrow Head
    glutSolidCone(0.1,0.3,5,5); // Arrow Head is drawn, as a solid cone. Base on Z=0, Height on Z=0.3, Radius of 0.3 {5 slices and 5 stacks}.
    
    // This cone stays in the previously  translated position. In this case "5+axes_y"
    glTranslatef(0,0,-5-axes_y); // Translates to the initial position - origin
    glRotatef(90.f,1,0,0); // Rotates to the initial position - origin

    // Arrow Head of Z Axis (BLUE)
    glTranslatef(0,0,5+axes_z); // Translation of "5+axes_z", relative to the Z Axis
    glColor3f(0.0f,0.0f,1.0f); // Color of our Z Arrow Head
    glutSolidCone(0.1,0.3,5,5); // Arrow Head is drawn, as a solid cone. Base on Z=0, Height on Z=0.3, Radius of 0.3 {5 slices and 5 stacks}.
    
    // This cone stays in the previously  translated position. In this case "5+axes_z"
    glTranslatef(0,0,-5-axes_z); // Translates to the initial position - origin

    glBegin(GL_LINES);

    // Line of X Axis (RED) - From (0,0,0) to (5+axes_x,0,0)
    glColor3f(1.0,0,0);
    glVertex3f(0,0,0);
    glVertex3f(5 + axes_x,0,0);

    // Line of Y Axis (GREEN) - From (0,0,0) to (0,5+axes_y,0)
    glColor3f(0,1.0,0);
    glVertex3f(0,0,0);
    glVertex3f(0,5 + axes_y,0);

    // Line of Z Axis (BLUE) - From (0,0,0) to (0,0,5+axes_z)
    glColor3f(0,0,1.0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,5 + axes_z);

    glEnd();

    // Letter X coloring (RED) and positioning (5+axes_x+0.5,0,0)
    glColor3f(1.0,0,0);
    glRasterPos3f((5 + axes_x + 0.5),0,0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'X');

    // Letter Y coloring (GREEN) and positioning (0,5+axes_y+0.5,0)
    glColor3f(0,1.0,0);
    glRasterPos3f(0,(5 + axes_y + 0.5),0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'Y');

    // Letter Z coloring (BLUE) and positioning (0,0,5+axes_z+0.5)
    glColor3f(0,0,1.0);
    glRasterPos3f(0,0,(5 + axes_z + 0.5));
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'Z');
}

void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(camX, camY, camZ, // Position setting of our "eye" aka Camera
              0.0, 0.0, 0.0, // Sets the position we are looking at
              0.0f, 1.0f, 0.0f); // Sets the direction of the Up Vector (the axis which is upwards)


    glTranslatef(t_x, t_y, t_z); // Translates the object's positioning, as well as the XYZ axis
    glRotatef(r_y,0,1,0); // Rotates relatively to the Y axis (-1º or +1º)
    glRotatef(r_x,1,0,0); // Rotates relatively to the Z axis (-1º or +1º)

    drawAxis(); // Draws Axis

    glPolygonMode(GL_FRONT_AND_BACK,draw_mode); // Changes how our shapes are represented (drawn). Lines, Points or Filled
    glBegin(GL_TRIANGLES);

    // Goes through all our models's vertexes and draws them
    for(int i = 0; (i < models.size()) && ((models[i]) || (models[i])); i+=6){
        glColor3f(R2,G2,B2);
        glVertex3f(models[i]->getX(),models[i]->getY(),models[i]->getZ());
        glVertex3f(models[i+1]->getX(),models[i+1]->getY(),models[i+1]->getZ());
        glVertex3f(models[i+2]->getX(),models[i+2]->getY(),models[i+2]->getZ());
        glColor3f(R3,G3,B3);
        glVertex3f(models[i+3]->getX(),models[i+3]->getY(),models[i+3]->getZ());
        glVertex3f(models[i+4]->getX(),models[i+4]->getY(),models[i+4]->getZ());
        glVertex3f(models[i+5]->getX(),models[i+5]->getY(),models[i+5]->getZ());

    }

    glEnd();

    glutSwapBuffers();
}

void keyboardSpecial(int key_code, int x , int y){

    switch(key_code){
        case GLUT_KEY_UP: r_x-=2.0; // Rotates X, +1º
            break;
        case GLUT_KEY_DOWN:	r_x+=2.0; // Rotates X, -1º
            break;
        case GLUT_KEY_LEFT: r_y-=2.0; // Rotates Y, +1º
            break;
        case GLUT_KEY_RIGHT: r_y+=2.0; // Rotates Y, -1º
            break;
    }

    glutPostRedisplay();
}

void keyboardControls(unsigned char key, int x, int y){
    switch (key) {

        case 'a': // Translates to the Left
            t_x -= 2;
            t_z += 2;
            break;

        case 'd': // Translates to the Right
            t_x += 2;
            t_z -= 2;
            break;

        case 'w': // Translates Upward
            t_y += 2;
            break;

        case 's': // Translates Downward
            t_y -= 2;
            break;

        case 'e': // -0.05 RED Color
            R2-=0.05;
            break;

        case 'r': // Changes Shape's Color to RED
            R2 = 1; R3=1;
            B2 = 0; B3 = 1;
            G2 = 0; G3 = 1;
            break;

        case 't':
            R2+=0.05; // +0.05 RED Color
            break;

        case 'f':
            G2-=0.05; // -0.05 GREEN Color
            break;

        case 'g': // Changes Shape's Color to GREEN
            R2 = 0;
            G2 = 1;
            B2 = 0;
            break;

        case 'h': // +0.05 GREEN Color
            G2+=0.05;
            break;

        case 'v': // -0.05 BLUE Color
            B2-=0.05;
            break;

        case 'b': // Changes Shape's Color to BLUE
            R2 = 0;
            G2 = 0;
            B2 = 1;
            break;

        case 'n': // +0.05 BLUE Color
            B2+=0.05;
            break;

        case 'p': // Sets our Models to be represented as Points
            draw_mode = GL_POINT;
            break;

        case 'l': // Sets our Models to be represented as Lines
            draw_mode = GL_LINE;
            break;

        case 'm': // Sets our Models to be filled
            draw_mode = GL_FILL;
            break;

        case '-': // Zoom Out Camera
            gluLookAt(camX += 0.5, camY += 0.5, camZ += 0.5, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
            break;

        case '+': // Zoom In Camera
            gluLookAt(camX -= 0.5, camY -= 0.5, camZ -= 0.5, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
            break;

        case '1': // Increases X axis length
            axes_x +=1;
            break;

        case '2': // Increases Y axis length
            axes_y +=1;
            break;

        case '3': // Increases Z axis length
            axes_z +=1;
            break;

        case 'c': // Resets all changes to the default settings
            camX = 10; camY = 10; camZ = 10;
            r_x = 1; r_y = 1;
            t_x = 0; t_y = 0; t_z = 0;
            R2=1,G2=1,B2=1;R3=1,G3=1,B3=1;
            axes_x = 0;
            axes_y = 0;
            axes_z = 0;
            draw_mode = GL_LINE;
    }

    glutPostRedisplay(); // Projects  the changes made at the moment a key is pressed
}

// Reads the ".3d" files, inside the given XML file and inserts all Vertexes in our Global Struct "vector<Vertex*> models"
void readFile(string m_file) {
    string line;
    ifstream file(m_file);

    if (file.fail()) {
        throw std::ios_base::failure(string("Couldn't find file: ") + m_file);
    }

    else {
        while (getline(file, line)) {
            Vertex *v = new Vertex(line);
            models.push_back(v);
        }
    }
    file.close();
}

// Parses through the XML File, while getting the "model" attribute informations
void readXML(string  f_path){
    XMLDocument xmlDoc;
    XMLElement *element;

    if (!(xmlDoc.LoadFile(f_path.c_str()))) {

        element = xmlDoc.FirstChildElement(); //ROOT ELEMENT (<scene>)
        for (element = element->FirstChildElement(); element; element = element->NextSiblingElement()) { // Iterates between Elements
            string ficheiro = element->Attribute("file"); // Gets file information, on each Model Attribute
            readFile(ficheiro); // Gets model's vertexes
        }
    }
    else {
        cout << "XML File " + f_path + " could not be found" << endl;
    }
}


int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("Engine - @CG2018");

     if(argc < 2){
        cout << "Invalid input. Use -h if you need some help." << endl;
        return 0;
    }

    else if (!strcmp(argv[1],"-h")){
        printHelp();
        return 0;
    }

    else readXML(argv[1]); // Read XML File

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutSpecialFunc(keyboardSpecial);
    glutKeyboardFunc(keyboardControls);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}

