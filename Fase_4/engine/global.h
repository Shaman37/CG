#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "group.h"
#include <vector>

using namespace std;

Group* scene = new Group();

Group* asteroid = new Group();
int n_asteroid = 0;
vector<Asteroid*> p_asteroids;
vector<float> ast_scale;

Group* ship = new Group();
float speed = 0.5;

float pos_x=30;
float pos_y = 5;
float pos_z= 10;

float h_angle = -10;
float v_angle = 45;

bool foward = false;
bool backward = false;
bool strafe_left = false;
bool strafe_right = false;
bool turn_left = false;
bool turn_right = false;
bool roll = false;

float tilt = 0;
int current_roll = 0;

Group* bullet = new Group();
vector<Bullet*> bullets;
bool gun = false;
bool fire = false;
float pew = 1;
float gun_side = 0.3;

bool orbits = false;
vector<Translation*> orbit;

bool fpcam = false;

float distanceBetween3D(float x1, float y1, float z1, float x2, float y2, float z2){
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

#endif
