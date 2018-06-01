#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include <iostream>

#include "Critter.h"
#include "Player.h"

using namespace std;

Player::Player(){
  x=y=50;
  xV=yV=0;
  maxSpeed=5;
  accel=.2;
  width=height=25;
  ducking=false;
}
