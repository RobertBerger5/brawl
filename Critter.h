#ifndef _CRITTER_
#define _CRITTER_

#include "Platform.h"

class Platform;

class Critter{
  /*
    Subclasses to implement:
      Player: the player, can move around pretty fast, and can duck. Has some sort of attack...?
      Bird: can fly around and probably ignore platforms, have a line of sight of an angle, and fly big circles if they don't see anyone (try to avoid leaving the map tho...
      Bear: slow but fucks you UP
      (something fast that has shit attack?)
        badger?
	domesticated dog like a jack russel or somethin? but I don't wanna kill any...
   */
 public:
  float x,y;
  float xV,yV;
  int maxSpeed;
  float accel;
  int width,height;
  int health;
  int damage;
  float r,g,b;
  
  //Image img;//TODO: is there a more efficient glut way to do this?
  //TODO: enum for which animation to do
  
  Critter();
  Critter(int x1,int y1,int w,int h);//TODO: add image somehow
  Critter(const Critter& src);
  //make a destructor if pointers are added
  
  virtual void draw();
  
  virtual void update(Platform** plats,Critter** crits);
  virtual int collide(Platform& plat);//returns int incase we need data about it?
};

#endif
