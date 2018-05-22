#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "tinyxml2.h"
#include "group.h"
#include "parser.h"

using namespace std;

float pos_x=30;
float pos_y = 5;
float pos_z= 10;

float h_angle;
float v_angle;
float speed = 0.1;

bool foward = false;
bool backward = false;
bool strafe_left = false;
bool strafe_right = false;
bool turn_left = false;
bool turn_right = false;
bool roll = false;

float tilt = 0;

int current_roll = 0;

bool mouseCaptured = true;
bool warping = false;
float centerX;
float centerY;

int window;
int timebase = 0, frame = 0;
float fps;

Group* scene = new Group();  // Global struct
Group* ship = new Group();

bool orbits = true;
vector<Translation*> orbit;

int draw_mode = GL_TRIANGLES;

void cameraUpdate();

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
    cout << "|   w: Move Foward                                                                                                       |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   s: Move Back                                                                                                            |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   a: Strafe Left                                                                                                        |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   d: Strafe Right                                                                                                     |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   q: Turn Left (Third Person Only)                                                                    |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   e: Turn Right (Third Person Only)                                                                  |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   + : Increase Camera Speed                                                                                 |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   - : Decrease Camera Speed                                                                                  |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   FORMAT:                                                                                                                     |" << endl;
    cout << "|   p: Change the figure format into points                                                     |" << endl;
    cout << "|                                                                                                                                       |" << endl;
    cout << "|   l: Change the figure format into lines                                                       |" << endl;
    cout << "|                                                                                                                                       |" << endl;
    cout << "|   m: Fill up the figure                                                                                           |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   WINDOW:                                                                                                                    |" << endl;
    cout << "|   f: Change to Fullscreen Mode                                                                          |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   ESC: Close Window                                                                                                |" << endl;
    cout << "#__________________________________________________________________________________________#" << endl;
}


void displayFPS(){
    int time;
    char title[20];

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame * 1000.0/(time - timebase);
        timebase = time;
        frame = 0;
        sprintf(title,"Engine  |  %.2f FPS",fps);
        glutSetWindowTitle(title);
    }
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
    for(Transform *t: t_list)
        t->perform();

    vector<Model*> m_list = g->getModels();

    for(Model *m: m_list) m->renderModel();

    vector<Group*> c_list = g->getChilds();
    for(Group *c: c_list) render(c);

    glPopMatrix();
}

void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glPolygonMode(GL_FRONT_AND_BACK,draw_mode);

    glTranslatef(0.0f, -0.6f, -3);
    glTranslatef(0,centerY/1000 - 0.7,-centerX/225);
    cameraUpdate();

    render(scene);


    if(orbits) {
        for (int i = 0; i < orbit.size(); i++)
            orbit[i]->drawCurve();
    }

    displayFPS();
    glutSwapBuffers();
}

void turnLeft(){
    v_angle -= speed*M_PI;
    if (v_angle < 360) v_angle += 360;
}

void turnRight(){
    v_angle += speed*M_PI;
    if (v_angle >360) v_angle -= 360;
}

void barrelRoll(){
    if(tilt == 25){
        if(current_roll < 360) {glRotatef(current_roll,0,0,1); current_roll += 3;}
        else {current_roll = 0; roll = false;}
    }
    else{
        if(current_roll < 360) {glRotatef(-current_roll,0,0,1); current_roll += 3;}
        else {current_roll = 0; roll = false;}
    }
}

void move_foward(){
    float xrot, yrot;
    yrot = (v_angle / 180.0f * M_PI);
    xrot = (h_angle / 180.0f * M_PI);
    pos_x += (sin(yrot))*speed;
    pos_z -= (cos(yrot))*speed;
    pos_y -= (sin(xrot))*speed;
}

void move_back(){
    float xrot, yrot;
    yrot = (v_angle / 180.0f * M_PI);
    xrot = (h_angle / 180.0f * M_PI);
    pos_x -= (sin(yrot))*speed;
    pos_z += (cos(yrot))*speed;
    pos_y += (sin(xrot))*speed;
}

void move_left(){
    float yrot;
    yrot = (v_angle / 180.0f * M_PI);
    pos_x -= (cos(yrot)) * speed;
    pos_z -= (sin(yrot)) * speed;
}

void move_right(){
    float yrot;
    yrot = (v_angle / 180.0f * M_PI);
    pos_x += (cos(yrot)) * speed;
    pos_z += (sin(yrot)) * speed;
}

/* *****************  */
/* KEYBORD CONTROLS */
/* *****************  */

/* ************ */
/* ON KEY PRESS */
/* ************ */

/* ---------------------- */

void keyPress(unsigned char key, int x, int y) {
    switch (key){

        case 'w':
        case 'W':
            foward = true;
            break;

        case 's':
        case 'S':
            backward = true;
            break;

        case 'a':
        case 'A':
            strafe_left = true;
            tilt = 25;
            break;

        case 'd':
        case 'D':
            strafe_right = true;
            tilt = -25;
            break;

        case 'q':
        case 'Q':
            turn_left = true;
            tilt = 25;
            break;

        case 'e':
        case 'E':
            turn_right = true;
            tilt = -25;
            break;

        case 'f':
        case 'F':
            glutFullScreen();
            break;

        case '+':
            speed += 0.05;
            if(speed > 1.5) speed = 1.5;
            break;

        case '-':
            speed -= 0.05;
            if (speed < 0.0) speed = 0.0;
            break;

        case 32:
            roll = true;
            break;

        case 'o':
        case 'O':
            orbits = !orbits;
            break;

        case 'l':
        case 'L':
            draw_mode = GL_LINE;
            break;

        case 'p':
        case 'P':
            draw_mode = GL_FILL;
            break;

            case 'm':
            case 'M':
            draw_mode = GL_POINT;
        break;

        case 27:
            glutDestroyWindow(window);
            exit(0);
    }
}


/* **************** */
/* ON KEY REALEASE */
/* **************** */

/* ---------------------- */

void keyRelease(unsigned char key, int x, int y){
    switch (key){

        case 'w':
        case 'W':
            foward = false;
            break;

        case 's':
        case 'S':
            backward = false;
            break;

        case 'a':
        case 'A':
            strafe_left = false;
            tilt = 0;
            break;

        case 'd':
        case 'D':
            strafe_right = false;
            tilt = 0;
            break;

        case 'q':
        case 'Q':
            turn_left = false;
            tilt = 0;
            break;

        case 'e':
        case 'E':
            turn_right = false;
            tilt = 0;
            break;
    }
}

/* *************** */
/* MOUSE MOVEMENT */
/* *************** */

void mouseMove( int x, int y ){

    centerX = glutGet(GLUT_WINDOW_WIDTH) / 2.0f ;
    centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;

    if(warping)
    {
        warping = false;
        return;
    }

    float diffx = x - centerX;
    float diffy = y - centerY;

        h_angle += diffy*0.1;
        v_angle+= diffx*0.1;

        if (h_angle> 40.0f) h_angle= 40.0f;
        if (h_angle< -40.0f) h_angle= -40.0f;

    if(mouseCaptured){
        warping = true;
        glutWarpPointer(centerX,centerY);
    }
}

void cameraUpdate(){;

    if (foward) move_foward();
    if (backward) move_back();
    if (strafe_left) move_left();
    if (strafe_right) move_right();
    if(turn_left) turnLeft();
    if(turn_right) turnRight();

    glPushMatrix();
    glRotatef(tilt,0,0,1);
    glRotatef(-h_angle/2,1,0,0);
    if(roll) barrelRoll();

    render(ship);

    glPopMatrix();

    glRotatef(h_angle,1,0,0);
    glRotatef(v_angle,0,1,0);
    glTranslatef(-pos_x,-pos_y,-pos_z);


}

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1080,720);
    window = glutCreateWindow("Engine - @CG2018");
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnableClientState(GL_VERTEX_ARRAY);

     if(argc < 2){
        cout << "Invalid input. Use -h if you need some help." << endl;
        return 0;
    }
    else if (!strcmp(argv[1],"-h")) {
         printHelp();
         return 0;
     }
     else readXML(argv[1], scene, &orbit); // Read XML File

    readXML("../files3d/space.xml",ship,&orbit);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyRelease);
    glutPassiveMotionFunc(mouseMove);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();

    return 1;
}