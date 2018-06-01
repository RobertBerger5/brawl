#ifndef _CRITTER_
#define _CRITTER_

class Critter{
 public:
  int x,y;
  float xV,yV;
  int maxSpeed;
  float accel;
  int width,height;
  
  //Image img;//TODO: is there a more efficient glut way to do this?
  //TODO: enum for which animation to do
  
  Critter();
  Critter(int x1,int y1,int w,int h);//TODO: add image somehow
  Critter(const Critter& src);
  //TODO: destructor when pointers get in here
  
  int getX(){return x;};
  int getY(){return y;};
  /*int getXV(){return xV;};
  int getYV(){return yV;};
  int getXA(){return xA;};
  int getYA(){return yA;};*/
  int getW(){return width;};
  int getH(){return height;};
  
  virtual void update();//TODO: make virtual
  void draw();
};

#endif
