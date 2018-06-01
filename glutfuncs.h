#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

int WIDTH=700;
int HEIGHT=500;

char programName[]="¡Brawl!";

void drawWindow();
void update();
void keyboard(unsigned char c,int x,int y);
void keyboardUp(unsigned char c,int x,int y);
void mouse(int button,int state,int x,int y);
void mouse_motion(int x,int y);

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

//draw box, can either be filled or just an outline
void drawBox(int x,int y,int w,int h,bool fill){
  if(fill){
    //set width?
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

//window is resized/reshaped
void reshape(int w, int h){//probably shouldn't...
  glViewport(0,0, (GLsizei) w, (GLsizei) h);
  WIDTH = w; HEIGHT = h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);
}

//initialize everything
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

//initialize window
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
