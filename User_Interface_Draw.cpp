#include <iostream>
#include <fstream>
#include "Image.h"
#include "Selection.h"
#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "ColorPoint2.h"
#include "Button.h"
#include <string.h>
#include <ctype.h>
#include "subButton.h"
#include <stdio.h>
using namespace std;

char ** imgdatawrite;

enum ModeType{MouseDraw, Paint_Can, Selector_Mode,Move_Image, Resize_Image,Eraser_Mode};
// Shortcuts to image editing function
ModeType currentMode=MouseDraw;
bool mouseIsDragging = false;
int WIDTH = 1200;
int HEIGHT = 800;
int xOffset=100;
int yOffset=100;

char programName[] = "Image_Editor";
int menuitem=0;
int drawthis=1;
bool changing=false;

char * fileloc=0; // FILE DIRECTORY IS STORED HERE
char * saveloc;
int ptrx, ptry, ptrx2, ptry2;

bool PaintDraw;
int modechange=0;

int ** dataWrite;

int sliderR[]={WIDTH-255,0};
int sliderG[]={WIDTH-255,15};
int sliderB[]={WIDTH-255,30};
int r=223;
int g=10;
int b=255;

const int layerNum=10;
Image* img;
int iL=0;//image layer
Image** layers=new Image*[layerNum];

Image funstuff(WIDTH-100,HEIGHT-100); // test...
Image mouseIcon(20,20);
Image sprayIcon(20,20);
Image lineIcon(20,20);
Image moveIcon(20,20);
Image resizeIcon(20,20);
Image eraserIcon(20,20);
Image logo;
const int logoD=150;
int drawstart;//looks like this should be a bool?

// Button has the constructor x, y, width, height
int Button::numButtons = 0;
Button File(0,0,45,25,"File");
subButton Open(File,"Open",1);
subButton Save(File,"Save",2);
subButton Saveas(File,"Save as",3);
subButton Close(File,"Close",4);
subButton Quit(File, "Quit", 5);

Button Edit(45,0,50,25,"Edit");
subButton Copy(Edit,"Copy",1);
subButton Paste(Edit,"Paste",2);
subButton SelectAll(Edit,"Select All", 3);

Button Options(95,0,90,25,"Options");
subButton Help(Options,"Help",1);

Button ImageButton(185,0,70,25,"Image");
subButton Stitch(ImageButton,"Stitch",1);
subButton Rotate(ImageButton,"Rotate",2);
subButton Flip(ImageButton,"Flip X",3);
subButton RGB(ImageButton,"Flip Y",4);

Button Layers(255,0,80,25,"Layers");
subButton LayerDown(Layers,"Move A Layer Down", 1);


Button Effects(335,0,80,25,"Effects");
subButton Invert(Effects, "Invert", 1);
subButton Grey(Effects, "Grey", 2);
subButton Brightness(Effects,"Brightness", 3);

// Print out text at coordinates x and y, char * passed gets printed out

void PrintText(int x, int y, char * text){
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

void drawIcons(){
  glBegin(GL_LINE_LOOP); // box for pointer functions
  glColor3f(0.0,0.0,0.0);
  glLineWidth(60);
  glVertex2f(WIDTH-55, 100);
  glVertex2f(WIDTH-25,  100);
  glVertex2f(WIDTH-25,  255);
  glVertex2f(WIDTH-55, 255);
  glEnd();
  
  // glColor4f(1.0,0.0,0.0,.2);
  // glRecti(WIDTH-90,105,WIDTH-60, 125);
  

  if(currentMode==MouseDraw && modechange==1){
    mouseIcon.opacity(10);
  }
  else if(currentMode!=MouseDraw&&modechange==1){
    //mouseIcon.opacity(999);
    mouseIcon.read_file("icons/mouse.png");
  }
  
   if(currentMode==Paint_Can && modechange==1){
    sprayIcon.opacity(10);
    
  }
  else if(currentMode!=Paint_Can&&modechange==1){

   sprayIcon.read_file("icons/spray.png");

  //sprayIcon.opacity(999);
  }
  if(currentMode==Selector_Mode && modechange==1){
    lineIcon.opacity(10);
  }
  
  else if(currentMode!=Selector_Mode && modechange==1){
    //lineIcon.opacity(999);
    lineIcon.read_file("icons/selection.png");

  }
  if(Move_Image==currentMode && modechange==1){
    moveIcon.opacity(10);

  }
  else if(Move_Image!=currentMode && modechange==1){
    moveIcon.read_file("icons/move.png");

  //moveIcon.opacity(999);
  }
  if(Resize_Image==currentMode && modechange==1){
    resizeIcon.opacity(10);
  }
  else if(Resize_Image!=currentMode && modechange==1){
    resizeIcon.read_file("icons/resize.png");

    // resizeIcon.opacity(999);
  }
  if(Eraser_Mode==currentMode && modechange==1){
    eraserIcon.opacity(10);
  }
  else if(Eraser_Mode!=currentMode && modechange==1){
    eraserIcon.read_file("icons/eraser.png");
    //eraserIcon.opacity(999);
  }
  mouseIcon.draw(20,20);
  moveIcon.draw(20,20);
  sprayIcon.draw(20,20);
  lineIcon.draw(20, 20);
  resizeIcon.draw(20,20);
  eraserIcon.draw(20,20);
  logo.draw(logo.xImg,logo.yImg,logoD,logoD);
  modechange++;
}

void drawMenu(){
  
  
  if(menuitem==1){
    glBegin(GL_LINE_LOOP); // start drawing lines
    glColor3f(0.0,0.0,0.0); // color
    glLineWidth(40); // line width
    glVertex2f(File.position.x+1, File.position.y+25); // draws between all points in order that the points are declared and draws lines between them
    glVertex2f(File.position.x+40 + File.width, File.position.y+25);
    glVertex2f(File.position.x+40 + File.width,Quit.position.y + 10);
    glVertex2f(File.position.x+1,Quit.position.y + 10);
    // lets make all widths of submenus uniform so that it doesn't look so clunky
    // maybe add lines between selections?
    
    // PrintText( x position, y position, label of button)
    glEnd(); // note: height of characters is about 25 pixels
    PrintText(Open.position.x+5, Open.position.y,Open.label);
    PrintText(Save.position.x+5, Save.position.y,Save.label);
    PrintText(Saveas.position.x+5,Saveas.position.y,Saveas.label);
    PrintText(Close.position.x+5,Close.position.y,Close.label);
    PrintText(Quit.position.x+5,Quit.position.y,Quit.label);
    // print text
    
  }else if(menuitem==2){
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0,0.0,0.0);
    glLineWidth(40);
    glVertex2f(Edit.position.x, Edit.position.y+25); 
    glVertex2f(Edit.position.x+40 + Edit.width+15, Edit.position.y+25);
    glVertex2f(Edit.position.x+40 + Edit.width+15,SelectAll.position.y+10);
    glVertex2f(Edit.position.x+1,SelectAll.position.y+10);
    glEnd();
    PrintText(Copy.position.x+5, Copy.position.y,Copy.label);
    PrintText(Paste.position.x+5, Paste.position.y,Paste.label);
    PrintText(SelectAll.position.x+5,SelectAll.position.y,SelectAll.label);
  }else if(menuitem==3){
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0,0.0,0.0);
    glLineWidth(40);
    glVertex2f(Help.position.x, Options.position.y+25); 
    glVertex2f(Help.position.x -30 + Help.width, Options.position.y+25);
    glVertex2f(Options.position.x -30 + Options.width,Help.position.y+10);
    glVertex2f(Options.position.x,Help.position.y+10);
    glEnd();
    
    PrintText(Help.position.x+5, Help.position.y,Help.label);
  }else if(menuitem==4){
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0,0.0,0.0); 
    glLineWidth(40); 
    glVertex2f(ImageButton.position.x, ImageButton.position.y+25); 
    glVertex2f(ImageButton.position.x+10 + ImageButton.width, ImageButton.position.y+25);
    glVertex2f(ImageButton.position.x +10+ ImageButton.width,RGB.position.y+10);
    glVertex2f(ImageButton.position.x,RGB.position.y+10 );
    
    glEnd();
    PrintText(Stitch.position.x+5, Stitch.position.y,Stitch.label);
    PrintText(Rotate.position.x+5, Rotate.position.y,Rotate.label);
    PrintText(Flip.position.x+5, Flip.position.y,Flip.label);
    PrintText(RGB.position.x+5, RGB.position.y,RGB.label);
  }else if(menuitem==5){
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0,0.0,0.0); 
    glLineWidth(40); 
    glVertex2f(Layers.position.x, Layers.position.y+25); 
    glVertex2f(Layers.position.x+135 + Layers.width, Layers.position.y+25);
    glVertex2f(Layers.position.x +135+ Layers.width,RGB.position.y+10);
    glVertex2f(Layers.position.x,RGB.position.y+10 );
    
    glEnd();
    PrintText(LayerDown.position.x+5, LayerDown.position.y,LayerDown.label);
    
  }
  else if(menuitem==6){
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0,0.0,0.0); 
    glLineWidth(40); 
    glVertex2f(Effects.position.x, Effects.position.y+25); 
    glVertex2f(Effects.position.x+40 + Effects.width, Effects.position.y+25);
    glVertex2f(Effects.position.x +40+ Effects.width,RGB.position.y+10);
    glVertex2f(Effects.position.x,RGB.position.y+10 );
    
    glEnd();
    PrintText(Invert.position.x+5, Invert.position.y,Invert.label);
    PrintText(Grey.position.x+5, Grey.position.y,Grey.label);
    PrintText(Brightness.position.x+5, Brightness.position.y,Brightness.label);
        


  }
  
  /*if(drawthis==1){//not used anymore
    for(int i=4;i<10000;i=i+4){
      glBegin(GL_LINE_LOOP);
      glColor4f(1.0,0.0,0.0,1.0);
      glLineWidth(40);
      // glVertex2f(ptrx, ptry);
      // glVertex2f(ptrx2,ptry2);
      glVertex2f(drawline[i],drawline[i+1]);
      glVertex2f(drawline[i+2],drawline[i+3]);
      glEnd();
    }
  }*/
}


void DrawBox(Button &but){ // see above for reference
  //background for box
  glColor3f(1.0,1.0,1.0);
    glBegin(GL_POLYGON);
    glVertex2f(but.position.x, but.position.y);
    glVertex2f(but.position.x + but.width,but.position.y);
    glVertex2f(but.position.x + but.width,but.position.y + but.height);
    glVertex2f(but.position.x,but.position.y + but.height);
    glEnd();
  //outline for box
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
  glLineWidth(20);
  glVertex2f(but.position.x, but.position.y);
  glVertex2f(but.position.x + but.width,but.position.y);
  glVertex2f(but.position.x + but.width,but.position.y + but.height);
  glVertex2f(but.position.x,but.position.y + but.height);
  glEnd();
  //text within box
  PrintText(but.position.x+5, but.position.y+20,but.label);
}

void DrawBox(int x,int y,int w,int h,bool s){//s stands for selected/special
  glBegin(GL_LINE_LOOP);
  glColor3f(0.0,0.0,0.0);//draw black
  if(s)
    glColor3f(1.0,0.0,0.0);//unless it's the selected layer
  glLineWidth(20);
  glVertex2f(x, y);
  glVertex2f(x + w,y);
  glVertex2f(x + w,y + h);
  glVertex2f(x,y + h);
  glEnd();
}
void DrawBox(int x,int y,int w,int h){
  glBegin(GL_POLYGON);
  glVertex2f(x, y);
  glVertex2f(x + w,y);
  glVertex2f(x + w,y + h);
  glVertex2f(x,y + h);
  glEnd();
}

void DrawSliders(){
  glColor3f(0.5,0.5,0.5);
  DrawBox(sliderR[0],sliderR[1],255,40);//background box
  glColor3f(1.0,0.0,0.0);
  DrawBox(sliderR[0],sliderR[1],(int)r,10);
  glColor3f(0.0,1.0,0.0);
  DrawBox(sliderG[0],sliderG[1],(int)g,10);
  glColor3f(0.0,0.0,1.0);
  DrawBox(sliderB[0],sliderB[1],(int)b,10);
  glColor3f(r/255.0,g/255.0,b/255.0);
  DrawBox(sliderR[0]-40,sliderR[1],40,40);
}
bool onSliderR(int x,int y){
  return
    x>=sliderR[0] &&
    x<sliderR[0]+255 &&
      y>=sliderR[1] &&
    y<sliderR[1]+10;
}
bool onSliderG(int x,int y){
  return
    x>=sliderG[0] &&
    x<sliderG[0]+255 &&
      y>=sliderG[1] &&
    y<sliderG[1]+10;
}
bool onSliderB(int x,int y){
  return
    x>=sliderB[0] &&
    x<sliderB[0]+255 &&
      y>=sliderB[1] &&
    y<sliderB[1]+10;
}

void moveLayer(bool up){//move the selected layer one up or down
  Image* temp;
  if(up && iL!=layerNum-1){
    temp=layers[iL];
    layers[iL]=layers[iL+1];
    layers[iL+1]=temp;
    iL++;
  }else if(iL!=0){
    temp=layers[iL];
    layers[iL]=layers[iL-1];
    layers[iL-1]=temp;
    iL--;
  }
}

void drawLayers(){
  Image* pic;
  for(int i=layerNum-1;i>=0;i--){
    pic=layers[i];
    pic->draw(10,100+50*(layerNum-i),50,50);
    DrawBox(10,100+50*(layerNum-i),50,50,0);
  }
  DrawBox(10,100+50*(layerNum-iL),50,50,1);
}

bool insideImageButton(int x, int y){
  
  double x0 = min(img->xImg, img->xImg + img->width);
  double x1 = max(img->xImg, img->xImg + img->width);
  double y0 = min(img->yImg, img->yImg + img->height);
  double y1 = max(img->yImg, img->yImg + img->height);
  if(x >= x0 && x <= x1 && y >= y0 && y <= y1)
    return true;
  else
    return false;
}


bool handleButtonPress(int x, int y){
  /* for(int i=1;i>11;i++){
    if(layers[i]->insideImage(x,y)==true)
      iL=i;
}
  */ 

  if(mouseIcon.insideImage(x,y)){
    currentMode=MouseDraw;
    modechange=1;
  }
  if(moveIcon.insideImage(x,y)){
    currentMode=Move_Image;
    modechange=1;
  }
  if(sprayIcon.insideImage(x,y)){
    currentMode=Paint_Can;
    modechange=1;
  }
  if(lineIcon.insideImage(x,y)){
    currentMode=Selector_Mode;
    modechange=1;
  }
  if(resizeIcon.insideImage(x,y)){
    currentMode=Resize_Image;
    modechange=1;
  }
  if(eraserIcon.insideImage(x,y)){
    currentMode=Eraser_Mode;
    modechange=1;
  }
  
  if(modechange!=1){
    if(File.containsPoint(x, y)){
      menuitem=1;
    }else if(Open.containsMenu(x, y)&&menuitem==1){
      if(fileloc)
	delete [] fileloc;
      // user clicked on open file dialog
      system("gnome-terminal -x sh -c 'zenity --file-selection > temp.txt'");//display dialogue
      changing=true;
      string filestr;
      while(filestr[0]=='\0'){
	ifstream f("temp.txt");
	f >> filestr;//store whatever file they chose into string named filestr
      }
      int i=0;
      while(filestr[i]!='\0')
	i++;
      
      fileloc = new char [i];
      for(int loop=0; loop<i;loop++)
	fileloc[loop]=filestr[loop];//wait why didn't we just put it into fileloc and just not involve filestr at all...?
      fileloc[i]='\0';
      
      menuitem=1;
      drawstart=1;
      remove("temp.txt");
    }else if(Saveas.containsMenu(x,y)&&menuitem==1){//TODO: there's nothing for Save.containsMenu??
      system("gnome-terminal -x sh -c 'zenity --file-selection --save > save.txt'");
      string savestr;
      while(savestr[0]=='\0'){
	ifstream f("save.txt");
	f >> savestr;
      }
      int i=0;
      while(savestr[i]!='\0')
	i++;
      
      saveloc = new char [i];
      for(int loop=0; loop<i;loop++){
	saveloc[loop]=savestr[loop];
	fileloc[loop]=saveloc[loop];
      }    
      saveloc[i]='\0';
      
      remove("save.txt");
      
      Image saver(*layers[0]);//otherwise it adds everything to the base layer every time you save
      for(int i=1;i<layerNum;i++){
	saver.addImage(*layers[i],layers[i]->xImg-xOffset,layers[i]->yImg-yOffset);
      }
      saver.write_file(saveloc);
      cout<<"SAVED TO "<<saveloc<<endl;
      
    }else if(Close.containsMenu(x,y) && menuitem==1){
      drawstart=0;
      fileloc=0;
      img->width=0;
    }else if(Quit.containsMenu(x,y)&&menuitem==1){
      int handle = glutGetWindow();
      glutDestroyWindow(handle);
    }else if(Edit.containsPoint(x,y)){
      menuitem=2;
    }else if(Options.containsPoint(x,y)){
      menuitem=3;
    }else if(ImageButton.containsPoint(x,y)){
      menuitem=4;
    }else if(Rotate.containsMenu(x,y) && menuitem==4){
      img->rotate();
    }else if(Flip.containsMenu(x,y) && menuitem==4){
      img->flipX(); 
    }else if(Layers.containsPoint(x,y)){
      menuitem=5;
    }else if(Effects.containsPoint(x,y)){
      menuitem=6;
    }
    else if(Brightness.containsMenu(x,y) && menuitem==6){
      layers[iL]->brightness(50);
    }
    else if(Grey.containsMenu(x,y) && menuitem==6){
      layers[iL]->grey(50);
    }
    else if(Invert.containsMenu(x,y) && menuitem==6){
      layers[iL]->invert();
    }
    else if(currentMode==Selector_Mode){
      ptrx=ptrx2=x;
      ptry=ptry2=y;
    }else if(currentMode==Paint_Can && insideImageButton(x,y)){
      //moved to the mouse motion function
    }else if(currentMode==Eraser_Mode && insideImageButton(x,y)){
      //moved to the mouse motion function
    }else{
      menuitem=0; // if the user is not inside the file bar then disappear
      //didn't click on anything, return 0
      return 0;
    }
  }
  return 1;
}

void changeImg(){
  img->read_file(fileloc);
  img->xImg=xOffset;
  img->yImg=yOffset;
  //if it's way bigger than the window, resize it
  while(img->width>WIDTH || img->height>HEIGHT){
    img->resize(img->width*0.8,img->height*0.8);
  }
  cout<<"current layer's image changed"<<endl;
}

/*void changeLayer(int n){//could also just do no arguments and go on to the next layer
  if(n<0 || n >=layerNum){
    cout<<"error: invalid layer"<<endl;
    return;
  }
  img=layers[n];
  iL=n;
  cout<<"selecting layer number "<<n<<endl;
}*/
void changeLayer(){
  iL++;
  if(iL>=layerNum)
    iL=0;
  img=layers[iL];
  cout<<"selecting layer number "<<iL<<endl;
}

void drawWindow(){
  glClear(GL_COLOR_BUFFER_BIT);// clear buffer
  
  //draw stuff
  drawLayers();
  //draw all images onto the canvas
  for(int i=0;i<layerNum;i++){
    layers[i]->draw(layers[i]->width,layers[i]->height);
  }
  //draw box around current selection
  DrawBox(ptrx,ptry,ptrx2-ptrx,ptry2-ptry,1);
  if(currentMode==Resize_Image)
    DrawBox(xOffset,yOffset,img->width,img->height,1);
  //draw box around what'll be saved if the user hits "save" right now
  DrawBox(xOffset,yOffset,layers[0]->width,layers[0]->height,0);
  DrawSliders();
  
  DrawBox(File);
  DrawBox(Edit);
  DrawBox(Options);
  DrawBox(ImageButton);
  DrawBox(Layers);
  DrawBox(Effects);
  drawMenu();
  drawIcons();
  if(fileloc && changing){
    changeImg();
    changing=false;
  }
  
  // end draw
  glutSwapBuffers();
}


void keyboard(unsigned char c, int x, int y){
  //int win = glutGetWindow();
  switch(c) {
    
    /* keyboard shortcuts to editing functions
       ex: case 'p': currentMode=PAINT_CAN; break;
    */
  case 'e':
  case 'E':
    modechange=1;
    currentMode=Eraser_Mode;
    cout << "Eraser Mode" << endl;
    break;
  case '+':
  case '=':
    cout<<"layer swapped up"<<endl;
    moveLayer(1);
    break;
  case '-':
  case '_':
    cout<<"layer swapped down"<<endl;
    moveLayer(0);
    break;
  case 'n':
  case 'N':
    cout<<"layer changed"<<endl;
    changeLayer();//changes layer
    break;
  case 'h':
  case 'H':
    modechange=1;
    currentMode=MouseDraw;
    cout<<"No mode"<<endl;
    break;

  case 'p':
  case 'P':
    modechange=1;
    currentMode=Paint_Can;
    cout << "Paint Can Mode" << endl;
    break;
  case 's':
  case 'S':
    modechange=1;
    currentMode=Selector_Mode;
    cout << "Selection Mode" << endl;
  break;
  case 'm':
  case 'M':
    modechange=1;
    currentMode=Move_Image;
    cout << "Move Image Mode" << endl;
    break;
  case 'r':
  case 'R':
    modechange=1;
    currentMode=Resize_Image;
    cout << "Resize Image Mode" << endl;
    break;
  case 'c':
  case 'C':
    //collapses the layer ontop of this onto it
    img->addImage(*layers[iL+1],layers[iL+1]->xImg-xOffset,layers[iL+1]->yImg-yOffset);
    layers[iL+1]->~Image();
    layers[iL+1]=new Image;
    break;
  case '0':
    img->fillBackground(r,g,b);
    break;
  case '1':
    img->invert();
    break;
  case '2':
    img->grey(75);
    break;
  case '3':
    img->brightness(50);
    break;
  case '4':
    img->brightness(150);
    break;
  case '5':
    img->opacity(75);
    break;
  case '6':
    img->opacity(100);
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void reshape(int w, int h){
  glViewport(0,0, (GLsizei) w, (GLsizei) h);
  WIDTH = w; HEIGHT = h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);
  mouseIcon.xImg=WIDTH-50;
  sprayIcon.xImg=WIDTH-50;
  lineIcon.xImg=WIDTH-50;
  moveIcon.xImg=WIDTH-50;
  resizeIcon.xImg=WIDTH-50;
  eraserIcon.xImg=WIDTH-50;

  logo.xImg=WIDTH-logoD-10;
  logo.yImg=HEIGHT-logoD-10;

  sliderR[0]=WIDTH-255;
  sliderG[0]=WIDTH-255;
  sliderB[0]=WIDTH-255;
}

void mouse(int button, int state, int x, int y){
  if (GLUT_LEFT_BUTTON == button) {
    
    if (GLUT_DOWN == state){
      //user clicked the left button down
      if(!handleButtonPress(x,y)){//if the user clicks on the menu, don't do anything else
	mouseIsDragging=true;
	if(currentMode==Resize_Image){
	  img->resize(img->xImg-xOffset*2+x,img->yImg-yOffset*2+y);
	}
      }
    }else if(GLUT_UP==state && currentMode==Selector_Mode){
      int x1=min(ptrx,ptrx2);
      int x2=max(ptrx,ptrx2);
      int y1=min(ptry,ptry2);
      int y2=max(ptry,ptry2);
      Selection s(x1-xOffset,y1-yOffset,x2-x1,y2-y1);
      layers[iL+1]-> ~Image();//manually delete the layer above
      layers[iL+1]=new Image(*img,s);
      iL++;
      img=layers[iL];
      img->xImg=ptrx;
      img->yImg=ptry;
      ptrx=ptry=ptrx2=ptry2=0;
    }
  }else if(GLUT_RIGHT_BUTTON == button) {
    if(GLUT_DOWN == state){
      mouseIsDragging=false; // usually right click and not drag - right click
    }
  }
  
  
  glutPostRedisplay();
}


void mouse_motion(int x,int y){
  // the mouse button is pressed, and the mouse is moving...
  //test if on sliders
  if(onSliderR(x,y)){
    r=x-sliderR[0];
  }else if(onSliderG(x,y)){
    g=x-sliderG[0];
  }else if(onSliderB(x,y)){
    b=x-sliderB[0];
  }
  //do things depending on current mode
  if(currentMode==Selector_Mode){
    ptrx2=x;
    ptry2=y;
  }else if(currentMode==Paint_Can){
    img->paint(10,x- img->xImg, y- img->yImg, 10, r, g, b);
  }else if(currentMode==Eraser_Mode){
    img->erase(x- img->xImg,y- img->yImg,10);
  }else if(currentMode==Move_Image && iL!=0){
    img->xImg=x-img->width/2;
    img->yImg=y-img->width/2;
  }
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
  
  // welcome message
  cout << "Welcome to " << programName << "." << endl;
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
  
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  
  //  glutSetCursor(GLUT_CURSOR_SPRAY);
  
  glutMainLoop();
}

int main(int argc, char *argv[]){
  for(int i=0;i<argc;i++){
    cout<<argv[i]<<endl;
  }
  if(argc>layerNum)
    cout<<"error: too many command line arguments given, only using the first "<<layerNum<<endl;
  for(int i=0;i<layerNum;i++){
    layers[i]=new Image;
    if(i<argc-1){
      layers[i]->read_file(argv[i+1]);
      //if it's way bigger than the window, resize it
      while(layers[i]->width>WIDTH || layers[i]->height>HEIGHT){
	layers[i]->resize(layers[i]->width*0.8,layers[i]->height*0.8);
      }
    }
    layers[i]->xImg=xOffset;
    layers[i]->yImg=yOffset;
  };
  
  img=layers[iL];
  
  mouseIcon.read_file("icons/mouse.png");
  mouseIcon.xImg=WIDTH-50;
  mouseIcon.yImg=105;
  sprayIcon.read_file("icons/spray.png");
  sprayIcon.xImg=WIDTH-50;
  sprayIcon.yImg=130;
  lineIcon.read_file("icons/line.png");
  lineIcon.xImg=WIDTH-50;
  lineIcon.yImg=155;
  moveIcon.read_file("icons/move.png");
  moveIcon.xImg=WIDTH-50;
  moveIcon.yImg=180;
  resizeIcon.read_file("icons/resize.png");
  resizeIcon.xImg=WIDTH-50;
  resizeIcon.yImg=205;
  eraserIcon.read_file("icons/eraser.png");
  eraserIcon.xImg=WIDTH-50;
  eraserIcon.yImg=230;
  logo.read_file("icons/logo.png");
  //logo.resize(50,50);
  logo.xImg=WIDTH-logoD-10;
  logo.yImg=HEIGHT-logoD-10;
  cout<<"made icons"<<endl;
  init_gl_window();
}
