#ifndef _PLAYER_
#define _PLAYER_

#include <map>
#include "Critter.h"

using namespace std;

class Player : public Critter{
public:
  bool ducking;//TODO: implement so the player doesn't become tiny
  map<char,bool> keys;
  Player();
  void keyboard(unsigned char c);
  void keyboardUp(unsigned char c);
  void update(Platform** plats, int platNum);
};

#endif
