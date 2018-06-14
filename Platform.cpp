#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Platform.h"
#include "Critter.h"

Platform::Platform(int x1,int y1,int w,int h){
  x=x1;
  y=y1;
  width=w;
  height=h;
  r=.5;
  g=.8;
  b=1;
}

Platform::Platform(const Platform& src){
  x=src.x;
  y=src.y;
  width=src.width;
  height=src.height;
}

bool Platform::inBounds(int x1,int y1){
  return (
	  x1>=x &&
	  x1<=x+width &&
	  y1>=y &&
	  y1<=y+height
	  );
}
//maybe have an inbounds for rectangle?
bool Platform::totallyInBounds(const Critter& crit){
  return(
	 crit.x+crit.width >= x &&
	 crit.x <= x+width &&
	 crit.y+crit.height >= y &&
	 crit.y <= y+height
	 );
}
bool Platform::inBounds(const Critter& crit){
  return(inBounds(crit.x,crit.y) ||
	 inBounds(crit.x+crit.width,crit.y) ||
	 inBounds(crit.x+crit.width,crit.y+crit.height) ||
	 inBounds(crit.x,crit.y+crit.height)
	 );
}

void Platform::draw(){
  glColor3f(r,g,b);
  glBegin(GL_POLYGON);
  glVertex2f(x,y);
  glVertex2f(x+width,y);
  glVertex2f(x+width,y+height);
  glVertex2f(x,y+height);
  glEnd();
}
