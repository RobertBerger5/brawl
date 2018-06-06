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
//map<char,bool> keys;

Player player;
Platform base(10,HEIGHT-5,WIDTH-20,100);

//TODO: static vars for how many there are...
Platform** platforms=new Platform*[10];//10 including the floor


void keyboard(unsigned char c, int x, int y){
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
  for(int i=0;i<PLATS;i++){
    platforms[i]->draw();
  }
  
  glColor3f(1,0,0);
  player.draw();
  
  drawBox(ptr[0],ptr[1],ptr[2]-ptr[0],ptr[3]-ptr[1],false);
  
  // end draw
  glutSwapBuffers();
}

void update(){
  player.update(platforms,PLATS);
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
