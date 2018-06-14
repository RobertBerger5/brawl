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

#include "constants.h"
#include "glutfuncs.h"
#include "Critter.h"
#include "Platform.h"
#include "Player.h"
#include "Bird.h"

using namespace std;


bool mouseIsDragging = false;
int xOffset=100;
int yOffset=100;


int ptr[4];//start x, start y, stop x, stop y
//map<char,bool> keys;

Player player;
Platform base(-100,HEIGHT-PSIZE,WIDTH+200,100);
Platform walL(-100+PSIZE,0,100,HEIGHT);
Platform walR(WIDTH-PSIZE,0,100,HEIGHT);
Platform top(-100,-100+PSIZE,WIDTH+200,100);
Platform** platforms;

Critter** critters;


void keyboard(unsigned char c, int x, int y){
  //TODO: the quit button should word even without "players"
  player.keyboard(c);
}
void keyboardUp(unsigned char c, int x, int y){
  player.keyboardUp(c);
}

void drawWindow(){
  glClear(GL_COLOR_BUFFER_BIT);// clear buffer
  
  glColor3f(0,0,0);
  drawBox(0,0,WIDTH,HEIGHT,true);//background

  glColor3f(.5,.5,.5);
  glBegin(GL_LINES);
  glVertex2f(0,HEIGHT/2);
  glVertex2f(WIDTH,HEIGHT/2);
  glVertex2f(WIDTH/2,0);
  glVertex2f(WIDTH/2,HEIGHT);
  glEnd();
  
  //glColor3f(.5,.9,1);
  for(int i=0;i<PLATS;i++){
    platforms[i]->draw();
  }
  
  //glColor3f(0,1,0);
  for(int i=0;i<CRITTERS;i++){//skip the player I guess?
    critters[i]->draw();
  }
  
  drawBox(ptr[0],ptr[1],ptr[2]-ptr[0],ptr[3]-ptr[1],false);
  
  // end draw
  glutSwapBuffers();
}

void update(){
  for(int i=0;i<CRITTERS;i++){
    critters[i]->update(platforms,critters);
  }
  glutPostRedisplay();
}

void init_area(){
  srand(time(NULL));
  
  //TODO: static vars for how many there are so they can count themselves
  platforms=new Platform*[PLATS];//includes 4 walls
  platforms[0]=&base;
  platforms[1]=&walL;
  platforms[2]=&walR;
  platforms[3]=&top;

  //random
  for(int i=4;i<PLATS;i++){//TODO: have some check for how close it is to others...
    int w=PSIZE*(rand()%PSIZE)+PSIZE*2;
    int x=PSIZE*(rand()%(WIDTH/PSIZE))-w/2;
    int y=PSIZE*(rand()%(HEIGHT/PSIZE));
    platforms[i]=new Platform(x,y,w,PSIZE);
  }/**/
  
  //grid
  /*for(int x=0;x<10;x++){
    for(int y=0;y<10;y++){
      platforms[(x*10)+y]=new Platform((int)(WIDTH/10*x),(int)(HEIGHT/10*y),PSIZE,PSIZE);
    }
    }/**/
  
  critters=new Critter*[CRITTERS];
  critters[0]=&player;
  for(int i=1;i<CRITTERS;i++){//TODO: fix
    int x=(rand()%(WIDTH-50))+25;
    int y=(rand()%(HEIGHT-50))+25;
    double ang=rand()/(double)100000000;//fuccc
    critters[i]=new Bird(x,y,ang);
  }
}

int main(int argc, char *argv[]){
  for(int i=0;i<argc;i++){
    cout<<argv[i]<<endl;
  }
  
  init_area();
  
  init_gl_window();
}
