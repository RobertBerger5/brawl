#ifndef _PLATFORM_
#define _PLATFORM_

#include "Critter.h"

class Critter;


class Platform{
 public:
  int x,y;
  int width,height;
  
  Platform(int x1,int y1,int w,int h);
  Platform(const Platform& src);
  
  bool inBounds(int x1,int y1);
  bool inBounds(const Critter& crit);
  //bool fallOnto(const Critter& crit);
  
  
  void draw(); 
};

#endif
