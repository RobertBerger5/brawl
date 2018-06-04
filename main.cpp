#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <string.h>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <time.h>

#include "glutfuncs.h"
#include "Critter.h"
#include "Platform.h"
#include "Player.h"

using namespace std;

const float GRAVITY= -.1;
const int PLATS=10;

bool mouseIsDragging = false;
int xOffset=100;
int yOffset=100;


int ptr[4];//start x, start y, stop x, stop y
map<char,bool> keys;

Player player;
Platform base(10,HEIGHT-5,WIDTH-20,100);

//TODO: static vars for how many there are...
Platform* platforms[PLATS];//10 including the floor


void keyboard(unsigned char c, int x, int y){
  keys[c]=true;
  switch(c) {
  case 'w':
  case 'W':
    for(int i=0;i<PLATS;i++){
      //TODO: maybe just make it so if the y is EXACTLY right, then let em jump
      if(platforms[i]->inBounds(player)){
	player.yV=-4;
	player.y--;
      }
    }
    break;
  case 's':
  case 'S':
    if(!player.ducking){
      player.height/=2;
      player.y+=player.height;
      player.ducking=true;
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
  glutPostRedisplay();
}
void keyboardUp(unsigned char c, int x, int y){
  keys[c]=false;
  switch(c){
  case 'w':
  case 'W':
    break;
  case 's':
  case 'S':
    if(player.ducking){
      player.height*=2;
      player.y-=player.height/2;
      player.ducking=false;
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

void mouse(int button, int state, int x, int y){
  if (GLUT_LEFT_BUTTON == button) {
    if (GLUT_DOWN == state){
      mouseIsDragging=true;
      ptr[0]=ptr[2]=x;
      ptr[1]=ptr[3]=y;
    }else if(GLUT_UP==state){
      mouseIsDragging=false;
      //what to do after mouse dragging motion...
    }
  }else if(GLUT_RIGHT_BUTTON == button) {
    if(GLUT_DOWN == state){
    }else if(GLUT_UP==state){
    }
  }
  
  glutPostRedisplay();
}

void mouse_motion(int x,int y){
  if(mouseIsDragging){
    ptr[2]=x;
    ptr[3]=y;
  }
  glutPostRedisplay();
}

void drawWindow(){
  glClear(GL_COLOR_BUFFER_BIT);// clear buffer
  
  glColor3f(0,0,0);
  drawBox(0,0,WIDTH,HEIGHT,true);//background
  
  glColor3f(.5,.5,.5);
  for(int i=0;i<PLATS;i++){
    platforms[i]->draw();
  }
  
  glColor3f(1,0,0);
  player.draw();
  
  drawBox(ptr[0],ptr[1],ptr[2]-ptr[0],ptr[3]-ptr[1],false);
  
  // end draw
  glutSwapBuffers();
}

void update(){//TODO: control accel and speed with pythag theorem?
  /*bool touch=false;
  for(int i=0;i<PLATS;i++){
    if(platforms[i]->fallOnto(player)){
      player.y=platforms[i]->y-player.height;
      player.yV=0;
      touch=true;
      break;
    }
  }
  if(!touch){
    player.yV-=GRAVITY;
  }*/

  player.yV-=GRAVITY;
  
  for(int i=0;i<PLATS;i++){
    if(platforms[i]->inBounds(player)){
      //cout<<"uh oh spaghettios "<<endl;
      player.collide(*platforms[i]);
    }
  }
  
  if(keys['w']==true){
    
  }
  if(keys['s']==true){
    
  }
  if(keys['a']==true){
    if(player.xV > -1* player.maxSpeed)
      player.xV-=player.accel;
  }
  if(keys['d']==true){
    if(player.xV < player.maxSpeed)
      player.xV+=player.accel;
  }
  
  if(keys['k']==true)
    cout<<player.xV<<endl;
  
  player.update();
  glutPostRedisplay();
}

void init_area(){
  srand(time(NULL));
  platforms[0]=&base;
  //TODO: make it generate in a blockier way
  //calculate 500x300 screen to be like 50x30ish "blocks"
  //makes less shitty lookin shit
  for(int i=1;i<PLATS;i++){
    int w=rand()%100+50;
    int h=10;
    platforms[i]=new Platform(rand()%(WIDTH-w),rand()%(HEIGHT-h),w,h);
  }
}

int main(int argc, char *argv[]){
  for(int i=0;i<argc;i++){
    cout<<argv[i]<<endl;
  }
  
  init_area();
  
  init_gl_window();
}
