#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <GL/glut.h>

#include "tinyxml2.h"
#include "group.h"
#include "parser.h"


using namespace std;
using std::vector;

float dir_x;
float dir_y;
float dir_z;

float h_angle;
float v_angle;

float pos_x;
float pos_y;
float pos_z;

float speed = 0.1;
float rotSpeed = 0.001;

int window;

Group* scene = new Group();  // Global struct
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
    cout << "|   + : Zoom In                                                                                                                |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   - : Zoom Out                                                                                                              |" << endl;
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
void changeSize(int w, int h){

    if (h == 0)
        h = 1;

    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void render(Group* g){
    glPushMatrix();

    vector<Transform*> t_list = g->getTransforms();
    for(Transform *t: t_list) t->perform();

    vector<Shape*> m_list = g->getModels();
    for(Shape *s: m_list){
        vector<Vertex*> v_list = s->getVertexes();

        glPolygonMode(GL_FRONT_AND_BACK,draw_mode); // Changes how our shapes are represented (drawn). Lines, Points or Filled
        glBegin(GL_TRIANGLES);

        for(int i = 0; i<s->getSize();i++){
            glVertex3f(v_list[i]->getX(),v_list[i]->getY(),v_list[i]->getZ());
        }

        glEnd();
    }

    vector<Group*> c_list = g->getChilds();
    for(Group *c: c_list) render(c);

    glPopMatrix();
}

void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(	pos_x, pos_y, pos_z,
                  pos_x+dir_x, pos_y+dir_y,  pos_z+dir_z,
                  0.0f, 1.0f,  0.0f);

    glTranslatef(30,50,20);
    render(scene);
    glutSwapBuffers();
}
bool mouseCaptured = true;

void keyboardControls(unsigned char key, int x, int y){
    switch (key) {

        case 'w': // Translates Upward
            pos_x += dir_x * speed;
			pos_y += dir_y * speed;
            pos_z += dir_z * speed;
            break;

        case 's': // Translates Downward
            pos_x -= dir_x * speed;
            pos_y -= dir_y * speed;
            pos_z -= dir_z * speed;
            break;

        case 'a': // Translates to the Left
pos_x += dir_z * speed;
pos_z -= dir_x * speed;
            break;

        case 'd': // Translates to the Right
pos_x -= dir_z * speed;
pos_z += dir_x * speed;
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
        case 'f':
            glutFullScreen();
            break;

        case '+':
            speed+=0.05;
            break;

        case '-':
            speed-=0.05;
            break;
        case 27:
            glutDestroyWindow(window);
            exit(0);
    }

    glutPostRedisplay(); // Projects  the changes made at the moment a key is pressed
}

bool warping = false;
void mouseMove(int x, int y)
{
    if(warping)
    {
        warping = false;
        return;
    }
    int dx = x - 100;
    int dy = y - 100;
    h_angle = h_angle+dx*rotSpeed;
    v_angle = v_angle+dy*rotSpeed;
    dir_x=sin(v_angle)*sin(h_angle);
    dir_y=-cos(v_angle);
    dir_z=-sin(v_angle)*cos(h_angle);

    if(mouseCaptured)
    {
        warping = true;
        glutWarpPointer(100,100);
    }
}

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1540,890);
    window = glutCreateWindow("Engine - @CG2018");

     if(argc < 2){
        cout << "Invalid input. Use -h if you need some help." << endl;
        return 0;
    }

    else if (!strcmp(argv[1],"-h")){
        printHelp();
        return 0;
    }

    else readXML(argv[1],scene); // Read XML File

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(keyboardControls);
    glutPassiveMotionFunc(mouseMove);

    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(100,100);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}