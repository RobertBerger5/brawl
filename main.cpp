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

#include "Critter.h"
#include "Platform.h"
#include "Player.h"

using namespace std;

const float GRAVITY= -.1;
const int PLATS=10;

bool mouseIsDragging = false;
int WIDTH = 700;
int HEIGHT = 500;
int xOffset=100;
int yOffset=100;

char programName[] = "¡Brawl!";

int ptr[4];//start x, start y, stop x, stop y
map<char,bool> keys;

Player player;
Platform base(10,HEIGHT-5,WIDTH-20,100);

//TODO: static vars for how many there are...
Platform* platforms[PLATS];//10 including the floor

// Print out text at coordinates x and y, char * passed gets printed out

void drawText(int x, int y, char * text){
  int l,i;
  l=strlen(text);
  glRasterPos2i( x, y); // location of text
  for( i=0; i < l; i++) 
    {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]); // font of text
    }
  // can change color of text but might look ugly
  
  glMatrixMode(GL_MODELVIEW); // code to prevent constant refresh (flickering)
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glEnable(GL_TEXTURE_2D);
  
  //glutSwapBuffers();
  glutPostRedisplay();
}

void drawBox(int x,int y,int w,int h,bool fill){
  if(fill){
      glBegin(GL_POLYGON);
  }else{
    glBegin(GL_LINE_LOOP);
  }
  glLineWidth(20);
  glVertex2f(x, y);
  glVertex2f(x + w,y);
  glVertex2f(x + w,y + h);
  glVertex2f(x,y + h);
  glEnd();
}

void keyboard(unsigned char c, int x, int y){
  keys[c]=true;
  switch(c) {
  case 'w':
  case 'W':
    for(int i=0;i<PLATS;i++){
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

void reshape(int w, int h){//probably shouldn't...
  glViewport(0,0, (GLsizei) w, (GLsizei) h);
  WIDTH = w; HEIGHT = h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);
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
  bool touch=false;
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

void init(void){
  // change color of window
  glClearColor(50.0, 50.0, 50.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // set up the coordinate system:  number of pixels along x and y
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);
  
  cout << "Welcome to " << programName << endl;
}


void init_gl_window(){
  char *argv[] = {programName};
  int argc = sizeof(argv) / sizeof(argv[0]);
  
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
  glutInitWindowSize(WIDTH,HEIGHT);
  glutInitWindowPosition(100,100);
  glutCreateWindow(programName);
  init();
  
  glutDisplayFunc(drawWindow);
  glutIdleFunc(update);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  
  glutMainLoop();
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
