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
bool Platform::inBounds(const Critter& crit){//no tolerance for hitboxes?
  return(
	 crit.x+crit.width >= x &&
	 crit.x <= x+width &&
	 crit.y+crit.height >= y &&
	 crit.y <= y+height
	 );
}

/*bool Platform::fallOnto(const Critter& crit){
  //want the critter to be falling (yV>0)
  //also want the top of the critter to be above the box
  //lower area needs to be inside tho
  return(
	 crit.yV>0 && //falling
	 crit.y+crit.height/2<y && //middle of crit is above plat
	 crit.x <= x+width  &&  crit.x+crit.width >=x && //critter in plat
	 crit.y+crit.height >= y  &&  crit.y+crit.height <= y+height //bottom in
	);
}
*/

void Platform::draw(){
  glBegin(GL_POLYGON);
  glVertex2f(x,y);
  glVertex2f(x+width,y);
  glVertex2f(x+width,y+height);
  glVertex2f(x,y+height);
  glEnd();
}
