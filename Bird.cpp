#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "Critter.h"
#include "Bird.h"

using namespace std;

Bird::Bird(){
  angle=PI/2;
}

Bird::Bird(int x1,int y1,double ang) : Critter(x1,y1,BIRDSIZE,BIRDSIZE){
  angle=ang;
  r=g=0;
  b=1;
  accel=(rand()%100)/(float)100+1;
  target=0;
  cw=(rand()%2)*2-1;//either 1 or -1 (which way it'll turn in idle)
};


void Bird::draw(){
  //draw directions
  if(target)
    glColor3f(1,0,0);
  else
    glColor3f(0,1,0);
  glBegin(GL_LINES);
  glVertex2f(x+width/2,y+height/2);
  glVertex2f(x+width/2+cos(angle)*BIRDSIZE*3,
	     y+height/2+sin(angle)*BIRDSIZE*3);
  
  //sight lines
  /*glVertex2f(x+width/2,y+height/2);
  glVertex2f(x+width/2+cos(angle+BIRDVIEWANGLE)*1000,
	     y+height/2+sin(angle+BIRDVIEWANGLE)*1000);
  
  glVertex2f(x+width/2,y+height/2);
  glVertex2f(x+width/2+cos(angle-BIRDVIEWANGLE)*1000,
        y+height/2+sin(angle-BIRDVIEWANGLE)*1000);*/
  glEnd();
  
  //actual bird
  Critter::draw();
  glEnd();
}

//ignoring platforms, fly around in a circle until a critter is within sight->chase
void Bird::update(Platform** plats,Critter** crits){
  //cout<<"update: ";
  if(target){//if chasing a target
    //cout<<"chasing, ";
    if(inSight(*target)){//if the target is in sight
      //cout<<"still in sight."<<endl;
      //TODO: adjust the angle to face the target

      
      if(getAngle(target->x+target->width/2,target->y+target->height/2) < angle)
	angle-=.02;
      else
	angle+=.02;
      
      if(angle>2*PI)
	angle=0;
      else if(angle<0)
	angle=2*PI;
    }else{//they got away, go back to "search mode" or whatever
      //cout<<"lost em."<<endl;
      target=0;
      cw=(rand()%2)*2-1;
    }
  }else{
    //cout<<"looking for target...";
    angle+=.01*cw;//fly in a circle (clockwise). should I give each a boolean for this?
    if(angle>2*PI)
      angle=0;
    else if(angle<0)
      angle=2*PI;
    
    for(int i=0;i<CRITTERS;i++){//look for new prey
      if(inSight(*crits[i]) && crits[i]!=this){
	target=crits[i];
	//cout<<"target found!";
      }
    }
    //cout<<endl;
  }
  
  //good enough right
  x+=cos(angle)*accel;
  y+=sin(angle)*accel;
}

int Bird::collide(Platform& plat){
  cout<<"fak ";
  return 0;
}

double Bird::getAngle(float x1,float y1){
  double yDiff=(y1-y);
  double xDiff=(x1-x);
  
  double theta;
  if(xDiff==0){
    //this should probably be slightly fancier...but it seems to work fine.
    if(yDiff<0)
      return angle>PI;
    else
      return angle<PI;
    //xDiff=-.01;
  }
  else
    theta=atan( yDiff / xDiff );
  
  if(xDiff<0)
    theta+=PI;
  
  while(theta<0)
    theta+=(2*PI);
  while(theta>2*PI)
    theta-=2*PI;

  return theta;
};

//of course, this doesn't take width and height into account...
//it'd be REALLY fancy if they couldn't see through platforms...so much harder...
bool Bird::inSight(Critter& crit){
  /*double yDiff=(crit.y+crit.height/2-y);
  double xDiff=(crit.x+crit.width/2-x);
  
  double theta;
  if(xDiff==0){
    //this should probably be slightly fancier...but it seems to work fine.
    if(yDiff<0)
      return angle>PI;
    else
      return angle<PI;
    //xDiff=-.01;
  }
  else
    theta=atan( yDiff / xDiff );
  
  if(xDiff<0)
    theta+=PI;
  
  while(theta<0)
    theta+=(2*PI);
  while(theta>2*PI)
  theta-=2*PI;*/

  double theta=getAngle(crit.x+crit.width/2,crit.y+crit.height/2);
  
  //cout<<theta/PI<<"pi rad"<<endl;
  
  
  return(
	  (
	    theta < (angle+BIRDVIEWANGLE) &&
	    theta > (angle-BIRDVIEWANGLE)
	  ) || (
	    theta < (angle+BIRDVIEWANGLE)-2*PI &&
	    theta > (angle-BIRDVIEWANGLE)-2*PI
	  ) || (
	    theta-2*PI < (angle+BIRDVIEWANGLE) &&
	    theta-2*PI > (angle-BIRDVIEWANGLE)
	  )
        );
}
