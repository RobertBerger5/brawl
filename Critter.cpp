#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include <iostream>

#include "Critter.h"

using namespace std;

Critter::Critter(){
  x=y=0;
  xV=yV=0;
  maxSpeed=5;
  accel=.1;
  width=height=25;
}

Critter::Critter(int x1,int y1,int w,int h){
  x=x1;
  y=y1;
  xV=yV=0;
  maxSpeed=10;
  accel=.2;
  width=w;
  height=h;
}
Critter::Critter(const Critter& src){
  x=src.x;
  y=src.y;
  width=src.width;
  height=src.height;
}

void Critter::update(){
  x+=xV;
  y+=yV;
}

void Critter::draw(){
  glBegin(GL_POLYGON);
  glVertex2f(x,y);
  glVertex2f(x+width,y);
  glVertex2f(x+width,y+height);
  glVertex2f(x,y+height);
  glEnd();
}
