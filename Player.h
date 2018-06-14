#ifndef _PLAYER_
#define _PLAYER_

#include <map>
#include "Critter.h"

using namespace std;

class Player : public Critter{
public:
  bool ducking;
  map<char,bool> keys;
  Player();
  void keyboard(unsigned char c);
  void keyboardUp(unsigned char c);
  void update(Platform** plats,Critter** crits);
  int collide(Platform& plat);
};

#endif
