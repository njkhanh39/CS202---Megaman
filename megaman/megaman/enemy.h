#pragma once
#include "entity.h"

using namespace sf;

class Enemy: public Entity {
private:
	bool canMove;
	float movingRange;
public:
	Enemy(float x, float y, bool _canMove, float _movingRange): Entity(x,y) {
		canMove = _canMove;
		movingRange = _movingRange;
	}


};