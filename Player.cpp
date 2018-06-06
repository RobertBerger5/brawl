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
  case 'q'://TODO: don't have it so damn close to wasd
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

void Player::update(Platform** plats,int platNum){
  //yV-=GRAVITY;//TODO: make constants.h or whatever with gravity, platNum, etc
  
  for(int i=0;i<platNum;i++){
    if(plats[i]->inBounds(*this)){
      collide(*plats[i]);//TODO: should probably just be one rig...
    }
  }

  yV+=.1;//gravity
  
  if(keys['w']==true){
    
  }
  if(keys['s']==true){
    
  }
  if(keys['a']==true){
    if(xV > -1* maxSpeed)
      xV-=accel;
  }
  if(keys['d']==true){
    if(xV < maxSpeed)
      xV+=accel;
  }
  
  if(keys['k']==true)
    cout<<xV<<endl;

  x+=xV;
  y+=yV;
}
