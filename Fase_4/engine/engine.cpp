#include <string>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include "global.h"
#include "tinyxml2.h"
#include "parser.h"

bool mouseCaptured = true;
bool warping = false;

int window;
float centerX;
float centerY;

int timebase = 0, frame = 0;
float fps;
float score = 0;
int draw_mode = GL_FILL;

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
    gluPerspective(45.0f, ratio, 2.0f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}

void render(Group* g){
    glPushMatrix();

    vector<Light*> l_list = g->getLights();
    for(Light *l: l_list)
        l->renderLight();

    vector<Transform*> t_list = g->getTransforms();
    for(Transform *t: t_list)
        t->perform();

    vector<Model*> m_list = g->getModels();
    for(Model *m: m_list) m->renderModel();

    vector<Group*> c_list = g->getChilds();
    for(Group *c: c_list) render(c);

    glPopMatrix();
}

void generateAsteroidPositions(){

    for (; n_asteroid < 500; n_asteroid++) {
        float x = 0;
        float z = 0;
        float y = 0;
        float a = 0;
        float scl = 0;
        float s = 0;
        float r = 0;

        while (distanceBetween3D(0,0,0, x,y,z) < 1.5) {

                float angle = float(rand() % 360);
                r = rand() % ((77 - 69) + 1) + 69;
                y = rand() % 4 + (-2);

                a = rand() % 360;
                s = rand() % ((10 - 2) + 1) + 2;
                s = s/95000;

                scl = rand() % ((1000 - 500) +1) + 500;
                scl = scl/500;

                x = cos(angle) * r;
                z = sin(angle) * r;
        }

        ast_scale.push_back(scl);

        p_asteroids.push_back(new Asteroid(r,y,a,s));
    }
}

void updateAsteroids(){
    if(p_asteroids.size() <= 0) generateAsteroidPositions();

    for(int i = 0; i < p_asteroids.size(); i++) {

        for(int j = 0; j <  bullets.size(); j++) {

            if ((distanceBetween3D(bullets[j]->getPosX(), bullets[j]->getPosY(), bullets[j]->getPosZ(),
                                                           p_asteroids[i]->getX(), p_asteroids[i]->getY()+ 2/ast_scale[i], p_asteroids[i]->getZ() )) < 0.5*ast_scale[i]) {
                p_asteroids[i]->flagHit();
                bullets[j]->setLife(0);
                score++;
            }
        }

        if (p_asteroids[i]->checkHit()) {
            delete p_asteroids[i];
            p_asteroids.erase(p_asteroids.begin() + i);
            ast_scale.erase(ast_scale.begin() + i);
        }

        else {
            glPushMatrix();

            glTranslatef(p_asteroids[i]->getX(), p_asteroids[i]->getY(), p_asteroids[i]->getZ());

            glColor3f(1, 1, 1);
            glScalef(ast_scale[i] , ast_scale[i], ast_scale[i] );
            render(asteroid);

            glPopMatrix();

            p_asteroids[i]->rotate();
        }
    }
}

/* ********* */
/* MOVEMENT */
/* ********* */

void turnLeft(){
    v_angle -= M_PI/8;
    if (v_angle <= 0) v_angle += 360;
}

void turnRight(){
    v_angle += M_PI/8;
    if (v_angle >= 360) v_angle -= 360;
}

void barrelRoll(){
    if(tilt == 25){
        if(current_roll < 360) {glRotatef(current_roll,0,0,1); current_roll += 2;}
        else {current_roll = 0; roll = false;}
    }
    else{
        if(current_roll < 360) {glRotatef(-current_roll,0,0,1); current_roll += 2;}
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

void gunFire(){
    if (fire) {
        if (pew <= 0) {
            gun = !gun;
            gun_side = -gun_side;
            bullets.push_back(new Bullet(pos_x + gun_side, pos_y, pos_z, h_angle, v_angle));
            pew = 3.5;
        }
    }
    else{
        pew = 1;
    }
}

    void renderBullets(){
    glPushMatrix();
    glTranslatef(0,-1.5,-5);

    for (int i = 0; i < bullets.size(); i++){

        if (bullets[i]->getLife() == 0){
            delete bullets[i];
            bullets.erase(bullets.begin()+i);
        }

        else{

            glPushMatrix();
            glRotatef(h_angle,1,0,0);
            glRotatef(v_angle,0,1,0);
            glTranslatef(-pos_x,-pos_y,-pos_z);

            glPushMatrix();
            glTranslatef(bullets[i]->getPosX(), bullets[i]->getPosY(), bullets[i]->getPosZ());
            render(bullet);
            glPopMatrix();

            glPopMatrix();
            bullets[i]->move();
        }
    }
    glPopMatrix();
}

/*
void renderCrosshair() {
    glPushMatrix();
    centerX = glutGet(GLUT_WINDOW_WIDTH) / 2.0f ;
    centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2.0f;
    glOrtho(0, centerX, centerY, 0, -1, 1);


    float crosshair[22] = {centerX, centerY-1,
                           centerX-1, centerY,
                           centerX+1, centerY,
                           centerX, centerY+1,
                           centerX, centerY-1,

                           centerX-0.5,centerY+1,
                           centerX-2, centerY,
                           centerX-0.5,centerY-2,

                           centerX+0.5,centerY+2,
                           centerX+2,centerY,
                           centerX+0.5,centerY-2,
    };

    glVertexPointer(2,GL_FLOAT,0,crosshair);
    glDrawArrays(GL_LINES,0,2);
    glPopMatrix();
}
*/

void renderPlayer() {
    if (!fpcam) {
        glPushMatrix();
        glTranslatef(0, -1.5, -5);
//        renderCrosshair();

        glRotatef(tilt, 0, 0, 1);
        glRotatef(-h_angle / 2, 1, 0, 0);

        if (roll) {
            glPushMatrix();
            barrelRoll();
            render(ship);
            glPopMatrix();
        } else render(ship);

    }else{
            glTranslatef(0, 1.5, 5);
        }
}

void cameraUpdate(){

    if (foward) move_foward();
    if (backward) move_back();
    if (strafe_left) move_left();
    if (strafe_right) move_right();
    if(turn_left) turnLeft();
    if(turn_right) turnRight();
    if(fire) gunFire(); else pew = 1;

    renderBullets();
    renderPlayer();

    glPopMatrix();

    glRotatef(h_angle,1,0,0);
    glRotatef(v_angle,0,1,0);
    glTranslatef(-pos_x,-pos_y,-pos_z);
}

void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glPolygonMode(GL_FRONT_AND_BACK,draw_mode);

    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    cameraUpdate();

    updateAsteroids();

    glEnable(GL_LIGHTING);
    render(scene);
    glDisable(GL_LIGHTING);

    if(orbits) {
        for (int i = 0; i < orbit.size(); i++)
            orbit[i]->drawCurve();
    }

    displayFPS();
    glutSwapBuffers();
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
            if(speed > 1.5) {speed = 1.5;}
            break;

        case '-':
            speed -= 0.05;
            if (speed < 0.0) {speed = 0.0;}
            break;

        case 'r':
        case 'R':
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

        case 32:
            fire = true;
                pew--;
            break;

        case 'c':
        case 'C':
            fpcam = !fpcam;
            break;

        case 27:
            glutDestroyWindow(window);
            exit(0);
    }

    glutPostRedisplay();
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

        case 32:
            fire = false;
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
        v_angle += diffx*0.1;

        if (h_angle> 60.0f) h_angle= 60.0f;
        if (h_angle< -60.0f) h_angle= -60.0f;

    if(mouseCaptured){
        warping = true;
        glutWarpPointer(centerX,centerY);
    }
}

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1080,720);
    window = glutCreateWindow("Engine - @CG2018");
    glewInit();
    ilInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHT0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);
    glCullFace(GL_BACK);

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
    readXML("../files3d/asteroid.xml",asteroid,&orbit);
    readXML("../files3d/bullet.xml",bullet,&orbit);

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