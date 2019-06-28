#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include <iostream>

#include "constants.h"
#include "Critter.h"
#include "Player.h"

using namespace std;

Player::Player(){
  x=y=250;
  xV=yV=0;
  maxSpeed=5;
  accel=.2;
  width=height=PLAYERSIZE;
  ducking=false;
  r=1;
  g=b=0;
}

void Player::keyboard(unsigned char c){
  keys[c]=true;
  switch(c) {
  case 'w':
  case 'W':
    if(yV==0){//technically now possible to double jump
      yV=-4;
      y--;
    }
    break;
  case 's':
  case 'S':
    if(!ducking){
      height/=2;
      y+=height;
      ducking=true;
    }
    break;
  case 'a':
  case 'A':
    break;
  case 'd':
  case 'D':
    break;
    case 'q'://TODO: don't have it so damn close to wasd?
    cout<<"exiting..."<<endl;
    glutDestroyWindow(glutGetWindow());
    exit(0);
    break;
  default:
    break;
  }
}

void Player::keyboardUp(unsigned char c){
  keys[c]=false;
  switch(c){
  case 'w':
  case 'W':
    break;
  case 's':
  case 'S':
    if(ducking){
      height*=2;
      y-=height/2;
      ducking=false;
    }
    break;
  case 'a':
  case 'A':
    break;
  case 'd':
  case 'D':
    break;
  default:
    break;
  }
}

void Player::update(Platform** plats,Critter** crits){
  bool fall=true;
  for(int i=0;i<PLATS;i++){
    if(plats[i]->inBounds(*this)){
      if(collide(*plats[i]))
	fall=false;
    }
  }
  
  if(fall)
    yV+=.1;//gravity
  
  if(keys['a']==true && xV > -1* maxSpeed && !ducking)
    xV-=accel;
  
  if(keys['d']==true && !ducking && xV < maxSpeed)
    xV+=accel;
  
  Critter::update(plats,crits);//TODO: put the falling shit in this somehow...
}

int Player::collide(Platform& plat){
  //TODO: there's some weirdness here...
  //also I could have them bounce off or lose health...
  if(x-xV+ width <= plat.x  &&  x+width > plat.x){//left->inside
    xV=0;
    x=plat.x- width;
    keys['d']=false;
    //stop x motion, place em to the left
    //cerr<<"left ";
  }else if( x- xV >= plat.x+plat.width &&  x < plat.x+plat.width){//right->inside
    xV=0;
    x=plat.x+plat.width;
    keys['a']=false;
    //stop x motion, place em to the right
    //cerr<<"right ";
  }
  
  if( y- yV+ height <= plat.y &&  y+ height >= plat.y){//above->inside
    yV=0;
    y=plat.y- height;
    //stop y motion, place em above
    return 1;//means don't continue falling...?
  }else if( y- yV >= plat.y+plat.height &&  y < plat.y+plat.height){//below->inside
    yV=0;
    y=plat.y+plat.height;
    //stop y motion, place em below
    //cerr<<"below ";
  }
  return 0;
}
