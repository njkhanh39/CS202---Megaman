#pragma once
#include "character.h"

enum MAP1CONST {
	UPBOUND = 0, LOWBOUND = 220, UPLIMIT = 0, DOWNLIMIT = 400
};

class Camera {//follows the character
private:
	sf::View* view; //borrowed

	const int upperbound; //when character y lvl <= upperbound, camera moves up
	const int lowerbound; //when character y lvl >= lowerbound, camera moves down

	const int uplimit; //view.top must >= uplimit
	const int downlimit; //view.top + height must <= downlimit 
public:
	Camera(int left,  int top, int width, int height,int u, int l, int ul, int dl, sf::View* stateview) :
		view(stateview),
		upperbound(u), lowerbound(l), uplimit(ul), downlimit(dl){

		view->setSize(Vector2f(width, height));
		view->setCenter(Vector2f( left + width / 2, top + height / 2 ));
	}

	~Camera(){}


	void UpdateFollowCharacter(Character* character) {
		if (character->IsDead()) return;

		int top = view->getCenter().y - view->getSize().y / 2;
		int height = view->getSize().y;

		//check for view rect dragging downwards

		if (character->getCenterPosition().y + height / 2 > downlimit) {
			view->setCenter(Vector2f(character->getCenterPosition().x, downlimit - height / 2));
		}
		else if (character->getCenterPosition().y - height / 2 < uplimit) {
			view->setCenter(character->getCenterPosition().x, uplimit + height / 2);
		}
		if (character->getUpMostY() <= lowerbound && character->getDownMostY() >= upperbound) { //normal, only update x-coor
			view->setCenter(Vector2f( character->getCenterPosition().x, height/2 ));
		}
		else if(top + height <= downlimit && top >= uplimit) {
			// update y coor with center to char

			view->setCenter(character->getCenterPosition());
		}
		else if(top + height > downlimit) { // the view rect cannot get any lower
			view->setCenter(Vector2f( character->getCenterPosition().x , downlimit-height/2));
		}
		else view->setCenter(character->getCenterPosition().x, uplimit + height / 2);

		top = view->getCenter().y - view->getSize().y / 2;
		height = view->getSize().y;

		if (top + height > downlimit) {
			view->setCenter(Vector2f(character->getCenterPosition().x, downlimit - height / 2));
		}
		else if (top < uplimit) {
			view->setCenter(character->getCenterPosition().x, uplimit + height / 2);
		}

		//top = view->getCenter().y - view->getSize().y / 2;

		////check for view rect dragging upwards

		//if (character->getDownMostY() >= upperbound) {//normal, only update x-coor
		//	view->setCenter(Vector2f( character->getCenterPosition().x, height / 2 ));
		//}
		//else if (top >= uplimit) {
		//	//center with char

		//	view->setCenter(character->getCenterPosition());
		//}
		//else {// the view rect cannot get any higher
		//	
		//	view->setCenter(character->getCenterPosition().x, uplimit + height / 2);
		//}
	}
};