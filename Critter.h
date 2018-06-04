#ifndef _CRITTER_
#define _CRITTER_

#include "Platform.h"

class Platform;

class Critter{
 public:
  int x,y;
  float xV,yV;
  int maxSpeed;
  float accel;
  int width,height;
  int health;
  
  //Image img;//TODO: is there a more efficient glut way to do this?
  //TODO: enum for which animation to do
  
  Critter();
  Critter(int x1,int y1,int w,int h);//TODO: add image somehow
  Critter(const Critter& src);
  //make a destructor if pointers are added
  
  virtual void update();//TODO: make virtual
  void draw();
  
  void collide(const Platform& plat);
};

#endif
