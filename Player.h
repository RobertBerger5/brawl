#ifndef _PLAYER_
#define _PLAYER_

#include "Critter.h"

class Player : public Critter{
public:
  bool ducking;//TODO: implement so the player doesn't become tiny
  Player();
};

#endif
