#ifndef _BIRD_
#define _BIRD_

#include <map>
//#include "constants.h"
#include "Critter.h"

using namespace std;

class Bird : public Critter{
 public:
  double angle;//where the bird is looking
  Critter* target;//target to pursue
  int cw;
  float turn;
  
  
  Bird();
  Bird(int x1,int y1,double ang);

  void draw();
  
  void update(Platform** plats,Critter** crits);
  int collide(Platform& plat);
  
  double getAngle(float x,float y);
  bool inSight(Critter& crit);
};

#endif
