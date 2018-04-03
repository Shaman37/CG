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

float pos_x=30;
float pos_y = 5;
float pos_z= 10;

float dir_x = 0;
float dir_y = 0;
float dir_z = 0;

float h_angle;
float v_angle;

float tilt = 0;

float speed = 0.2;
float rotSpeed = 0.00175;

bool mouseCaptured = true;
bool warping = false;

int centerX;
int centerY;

bool foward = false;
bool backward = false;
bool strafe_left = false;
bool strafe_right = false;
bool turn_left = false;
bool turn_right = false;

bool fps_cam = false;

int window;

Group* scene = new Group();  // Global struct
int draw_mode = GL_LINE;

void camera();

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
    cout << "|   + : Increase Camera Speed                                                                                 |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   - : Decrease Camera Speed                                                                                  |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   FORMAT:                                                                                                                     |" << endl;
    cout << "|   p: Change the figure format into points                                                     |" << endl;
    cout << "|                                                                                                                                       |" << endl;
    cout << "|   l: Change the figure format into lines                                                       |" << endl;
    cout << "|                                                                                                                                       |" << endl;
    cout << "|   o: Fill up the figure                                                                                           |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   WINDOW:                                                                                                                    |" << endl;
    cout << "|   f: Change to Fullscreen Mode                                                                          |" << endl;
    cout << "|                                                                                                                                      |" << endl;
    cout << "|   ESC: Close Window                                                                                                |" << endl;
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

    camera();

    render(scene);
    glutSwapBuffers();
}

/* *****************  */
/* KEYBORD CONTROLS */
/* *****************  */

/* ---------------------- */

/* ************ */
/* ON KEY PRESS */
/* ************ */

/* ---------------------- */

void keyboardPress(unsigned char key, int x, int y) {
    switch (key) {

        case 'w': // Translates Upward
            foward = true;
            break;

        case 's': // Translates Downward
            backward = true;
            break;

        case 'a': // Translates to the Left
            strafe_left = true;
            tilt += 50;
            if(tilt > 25) tilt = 25;
            break;

        case 'd': // Translates to the Right
            strafe_right = true;
            tilt -= 50;
            if(tilt < -25) tilt = -25;
            break;

        case 'q':
            turn_left = true;
            tilt +=50;
            if(tilt > 25) tilt = 25;
            break;

        case 'e':
            turn_right = true;
            tilt -=50;
            if(tilt > -25) tilt = -25;
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
            speed += 0.05;
            if(speed > 0.75) speed = 0.75;
            break;

        case '-':
            speed -= 0.05;
            if (speed < 0.0) speed = 0.0;
            break;

        case '1':
            fps_cam = true;
            break;

        case '3':
            fps_cam = false;
            break;

        case 27:
                glutDestroyWindow(window);
                exit(0);
    }
}

/* ---------------------- */

/* **************** */
/* ON KEY REALEASE */
/* **************** */

/* ---------------------- */

void keyboardRelease(unsigned char key, int x, int y){
    switch (key) {

        case 'w': // Translates Upward
            foward = false;
            break;

        case 's': // Translates Downward
            backward = false;
            break;

        case 'a': // Translates to the Left
            strafe_left = false;
            tilt = 0;
            break;

        case 'd': // Translates to the Right
            strafe_right = false;
            tilt = 0;
            break;

        case 'q':
            turn_left = false;
            break;

        case 'e':
            turn_right = false;
            break;
    }
}

void turnLeft(){
    v_angle -= 1;
    if (v_angle <360) v_angle -= 360;
}

void turnRight(){
    v_angle += 1;
    if (v_angle >360) v_angle -= 360;
}

/* ************ */
/* FPS CAM CALC */
/* ************ */

void move_foward_f(){
    pos_x += dir_x * speed;
    pos_y += dir_y * speed;
    pos_z += dir_z * speed;
}

void move_back_f(){
    pos_x -= dir_x * speed;
    pos_y -= dir_y * speed;
    pos_z -= dir_z * speed;
}

void move_left_f(){
    pos_x += dir_z * speed;
    pos_z -= dir_x * speed;
}

void move_right_f(){
    pos_x -= dir_z * speed;
    pos_z += dir_x * speed;
}

/* ************ */
/* TRD CAM CALC*/
/* ************ */

void move_foward_t(){
    float xrot, yrot;
    yrot = (v_angle / 180.0f * M_PI);
    xrot = (h_angle / 180.0f * M_PI);
    pos_x += (sin(yrot))*speed;
    pos_z -= (cos(yrot))*speed;
    pos_y -= (sin(xrot))*speed;
}

void move_back_t(){
    float xrot, yrot;
    yrot = (v_angle / 180.0f * M_PI);
    xrot = (h_angle / 180.0f * M_PI);
    pos_x -= (sin(yrot))*speed;
    pos_z += (cos(yrot))*speed;
    pos_y += (sin(xrot))*speed;
}

void move_left_t(){
    float yrot;
    yrot = (v_angle / 180.0f * M_PI);
    pos_x -= (cos(yrot)) * speed;
    pos_z -= (sin(yrot)) * speed;
}

void move_right_t(){
    float yrot;
    yrot = (v_angle / 180.0f * M_PI);
    pos_x += (cos(yrot)) * speed;
    pos_z += (sin(yrot)) * speed;
}

/* *************** */
/* MOUSE MOVEMENT */
/* *************** */

void mouseMove( int x, int y ) {

    centerX = glutGet(GLUT_WINDOW_WIDTH) / 2 ;
    centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2 ;

    if(warping)
    {
        warping = false;
        return;
    }
    int diffx = x - centerX;
    int diffy = y - centerY;

    if(fps_cam){
        h_angle += diffx * rotSpeed;
        v_angle +=  diffy * rotSpeed;

        dir_x = sin(v_angle) * sin(h_angle);
        dir_y = -cos(v_angle);
        dir_z = -sin(v_angle) * cos(h_angle);
    }
    else{
        h_angle += diffy*0.4;
        v_angle += diffx*0.4;

        if (h_angle > 30.0f) h_angle = 30.0f;
        if (h_angle < -30.0f) h_angle = -30.0f;
        if(v_angle > 360.0f) v_angle -= 360.0f;

    }

    if(mouseCaptured){
        warping = true;
        glutWarpPointer(centerX,centerY);
    }
}

/* ***** */
/* SHIP  */
/* ***** */

void drawShip(){
    glPushMatrix();
    glScalef(0.2,0.2,0.2);
    glRotatef(tilt,0,0,1);
    glRotatef(-h_angle,1,0,0);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //1
    glColor3f(0.0f,1.0f,0.0f);
    glScalef(0.3, 0.3, 0.3);
    glutSolidCone(1,1,10,10);
    glColor3f(0.0f,1.0f,0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //2
    glPushMatrix();
    glTranslated(0,0,0);
    glScalef(5.0,0.15,0);
    glutSolidCube(2);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //3
    glColor3f(0.25f,0.25f,0.25f);
    glPushMatrix();
    glScalef(0.35,0.35,0.35);
    glTranslated(-3,-1,4);
    glutWireSphere(2,50,50);
    glPopMatrix();

    //5
    glColor3f(0.25f,0.25f,0.25f);
    glPushMatrix();
    glScalef(0.19,0.19,0.19);
    glTranslated(-25,-1,4);
    glutWireSphere(2,50,50);
    glPopMatrix();

    //4
    glColor3f(0.25f,0.25f,0.25f);
    glPushMatrix();
    glScalef(0.35,0.35,0.35);
    glTranslated(3,-1,4);
    glutWireSphere(2,50,50);
    glPopMatrix();

    //6
    glColor3f(0.25f,0.25f,0.25f);
    glPushMatrix();
    glScalef(0.15,0.15,0.15);
    glTranslated(-27,-2,4);
    glutWireSphere(2,50,50);
    glPopMatrix();

    //7
    glColor3f(0.25f,0.25f,0.25f);
    glPushMatrix();
    glScalef(0.19,0.19,0.19);
    glTranslated(25,-1,4);
    glutWireSphere(2,50,50);
    glPopMatrix();

    //8
    glColor3f(0.25f,0.25f,0.25f);
    glPushMatrix();
    glScalef(0.15,0.15,0.15);
    glTranslated(27, -2, 4);
    glutWireSphere(2,50,50);
    glPopMatrix();


    //9
    glColor3f(1.0f,0.0f,0.0f);
    glPushMatrix();
    glScalef(0.55,0.55,0.55);
    glTranslated(1.79,-0.15,5);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //10
    glColor3f(1.0f,0.6f,0.0f);
    glPushMatrix();
    glScalef(0.42,0.42,0.42);
    glTranslated(2.26,0.14,8.5);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //11
    glColor3f(1.0f,1.0f,0.0f);
    glPushMatrix();
    glScalef(0.32,0.32,0.32);
    glTranslated(2.84,0.6,13.5);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //12
    glColor3f(0.87f,0.87f,0.87f);
    glPushMatrix();
    glScalef(0.22,0.22,0.22);
    glTranslated(4.0,1.5,23.5);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //13
    glColor3f(1.0f,0.0f,0.0f);
    glPushMatrix();
    glScalef(0.55,0.55,0.55);
    glTranslated(-1.79,-0.15,5);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //14
    glColor3f(1.0f,0.6f,0.0f);
    glPushMatrix();
    glScalef(0.42,0.42,0.42);
    glTranslated(-2.26,0.14,8.5);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //15
    glColor3f(1.0f,1.0f,0.0f);
    glPushMatrix();
    glScalef(0.32,0.32,0.32);
    glTranslated(-2.84,0.6,13.5);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //16
    glColor3f(0.87f,0.87f,0.87f);
    glPushMatrix();
    glScalef(0.22,0.22,0.22);
    glTranslated(-4.0,1.5,23.5);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //17
    glColor3f(1.0f,0.0f,0.0f);
    glPushMatrix();
    glScalef(0.3,0.3,0.3);
    glTranslated(-14.73,0.17,7);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //18
    glColor3f(1.0f,0.6f,0.0f);
    glPushMatrix();
    glScalef(0.25,0.25,0.25);
    glTranslated(-17.3,0.46,10);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //19
    glColor3f(1.0f,1.0f,0.0f);
    glPushMatrix();
    glScalef(0.2,0.2,0.2);
    glTranslated(-21.5,0.63,13);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //20
    glColor3f(0.87f,0.87f,0.87f);
    glPushMatrix();
    glScalef(0.15,0.15,0.15);
    glTranslated(-28.5,0.79,18);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //21
    glColor3f(1.0f,0.0f,0.0f);
    glPushMatrix();
    glScalef(0.23,0.23,0.23);
    glTranslated(-16.7,-0.53,7);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //22
    glColor3f(1.0f,0.6f,0.0f);
    glPushMatrix();
    glScalef(0.17,0.17,0.17);
    glTranslated(-22.1,-0.3,12);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //23
    glColor3f(1.0f,1.0f,0.0f);
    glPushMatrix();
    glScalef(0.13,0.13,0.13);
    glTranslated(-28.5,-0.01,18);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //24
    glColor3f(1.0f,0.0f,0.0f);
    glPushMatrix();
    glScalef(0.23,0.23,0.23);
    glTranslated(16.7,-0.53,7);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //25
    glColor3f(1.0f,0.6f,0.0f);
    glPushMatrix();
    glScalef(0.17,0.17,0.17);
    glTranslated(22.1,-0.3,12);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //26
    glColor3f(1.0f,1.0f,0.0f);
    glPushMatrix();
    glScalef(0.13,0.13,0.13);
    glTranslated(28.5,-0.01,18);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //27
    glColor3f(1.0f,0.0f,0.0f);
    glPushMatrix();
    glScalef(0.3,0.3,0.3);
    glTranslated(14.73,0.17,7);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //28
    glColor3f(1.0f,0.6f,0.0f);
    glPushMatrix();
    glScalef(0.25,0.25,0.25);
    glTranslated(17.3,0.46,10);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //29
    glColor3f(1.0f,1.0f,0.0f);
    glPushMatrix();
    glScalef(0.2,0.2,0.2);
    glTranslated(21.5,0.63,13);
    glutWireSphere(1,50,50);
    glPopMatrix();

    //30
    glColor3f(0.87f,0.87f,0.87f);
    glPushMatrix();
    glScalef(0.15,0.15,0.15);
    glTranslated(28.5,0.79,18);
    glutWireSphere(1,50,50);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPopMatrix();
}

void camera(){
    centerX = glutGet(GLUT_WINDOW_WIDTH) / 2 ;
    centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2 ;

    if(fps_cam) {
        if (foward) move_foward_f();
        if (backward) move_back_f();
        if (strafe_left) move_left_f();
        if (strafe_right) move_right_f();
        if(turn_left) turnLeft();
        if(turn_right) turnRight();

        gluLookAt(pos_x, pos_y, pos_z,
                  pos_x + dir_x, pos_y + dir_y, pos_z + dir_z,
                  0.0f, 1.0f, 0.0f);
    }

    else{
        if (foward) move_foward_t();
        if (backward) move_back_t();
        if (strafe_left) move_left_t();
        if (strafe_right) move_right_t();
        if(turn_left) turnLeft();
        if(turn_right) turnRight();

        glTranslatef(0,centerY/1000 - 0.7,-centerX/225);
        drawShip();

        glRotatef(h_angle,1,0,0);
        glRotatef(v_angle,0,1,0);
        glTranslatef(-pos_x,-pos_y,-pos_z);
    }
 }

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1080,720);
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

    glutKeyboardFunc(keyboardPress);
    glutKeyboardUpFunc(keyboardRelease);
    glutPassiveMotionFunc(mouseMove);
    glutSetCursor(GLUT_CURSOR_NONE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}