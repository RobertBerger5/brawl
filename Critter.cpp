#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include <iostream>

#include "Critter.h"
#include "Platform.h"

using namespace std;

Critter::Critter(){
  x=y=0;
  xV=yV=0;
  maxSpeed=5;
  accel=.1;
  width=height=25;
  health=100;
}

Critter::Critter(int x1,int y1,int w,int h){
  x=x1;
  y=y1;
  xV=yV=0;
  maxSpeed=10;
  accel=.2;
  width=w;
  height=h;
  health=100;
}
Critter::Critter(const Critter& src){
  x=src.x;
  y=src.y;
  xV=src.xV;
  yV=src.yV;
  accel=src.accel;
  width=src.width;
  height=src.height;
  health=src.health;
}

void Critter::update(Platform** plats,int platNum){
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

void Critter::collide(Platform& plat){
  //TODO: there's some weirdness here...
  //also I could have them bounce off or lose health...
  if(x-xV+ width < plat.x  &&  x+width > plat.x){//left->inside
    xV=0;
    x=plat.x- width;
    //stop x motion, place em to the left
  }else if( x- xV > plat.x+plat.width &&  x < plat.x+plat.width){//right->inside
    xV=0;
    x=plat.x+plat.width;
    //stop x motion, place em to the right
  }
  
  if( y- yV+ height < plat.y &&  y+ height > plat.y){//above->inside
    yV=-.1;
    y=plat.y- height;
    //stop y motion, place em above
  }else if( y- yV > plat.y+plat.height &&  y < plat.y+plat.height){//below->inside
    yV=0;
    y=plat.y+plat.height;
    //stop y motion, place em below
  }
}

