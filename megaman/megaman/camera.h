#pragma once
#include "character.h"

enum MAP1CONST {
	LEFTLIMIT = 0, RIGHTLIMIT = 7080, UPLIMIT = 0, DOWNLIMIT = 400, BOSS_REGION_LEFT = 6730, BOSS_REGION_RIGHT = 7080
};

enum MAP2CONST {
	LEFTLIMIT2 = 0, RIGHTLIMIT2 = 8174, UPLIMIT2 = 0, DOWNLIMIT2 = 767, BOSS_REGION_LEFT2 = 7754, BOSS_REGION_RIGHT2 = 8111
};

class Camera {//follows the character
private:
	sf::View* view; //borrowed

	const int leftlimit; 
	const int rightlimit; 

	const int uplimit; //view.top must >= uplimit
	const int downlimit; //view.top + height must <= downlimit 


	//--------MAP1---------//
	const int boss_region_left = 6730;
	const int boss_region_right = 7080;


	std::vector<sf::FloatRect> restricted;
public:
	Camera(int left, int top, int width, int height, int left_limit, int right_limit, int up_limit, int down_limit,
		int boss_left, int boss_right, sf::View* stateview) :
		view(stateview), leftlimit(left_limit), rightlimit(right_limit), uplimit(up_limit), downlimit(down_limit), boss_region_left(boss_left),
		boss_region_right(boss_right){

		view->setSize(sf::Vector2f(width, height));
		view->setCenter(sf::Vector2f( left + width / 2, top + height / 2 ));
	}

	Camera(int left, int top, int width, int height, int left_limit, int right_limit, int up_limit, int down_limit,
		int boss_left, int boss_right, sf::View* stateview, std::vector<FloatRect>& tmp) :
		view(stateview), leftlimit(left_limit), rightlimit(right_limit), uplimit(up_limit), downlimit(down_limit), boss_region_left(boss_left),
		boss_region_right(boss_right) {

		view->setSize(sf::Vector2f(width, height));
		view->setCenter(sf::Vector2f(left + width / 2, top + height / 2));




		for (auto& x : tmp) restricted.push_back(x);
	}

	~Camera(){}


	void Update(Character* character, const float& dt) {
		if (character->IsDead()) return;

		//int top = view->getCenter().y - view->getSize().y / 2;
		int height = view->getSize().y;
		int width = view->getSize().x;

		sf::Vector2f center = character->getCenterPosition();
		

		//------------BOSS REGION....
		if (character->getLeftMostX() >= boss_region_left) {
			//--------SLOWLY UPDATE UNTIL VIEW.LEFT == REGION_LEFT

			center = view->getCenter();

			auto leftview = view->getCenter().x - width / 2;
			
			if (leftview < boss_region_left) {
				center.x += 50 * dt;
			}
			else {
				center.x = boss_region_left + width / 2; //desired
			}

		}
		else {

			//try to fix center... We dont even use upperbound and lowerbound do we ?

			if (center.y - height / 2 < uplimit) {
				//adjust y
				center.y = uplimit + height / 2;
			}

			if (center.y + height / 2 > downlimit) {
				//adjust y
				center.y = downlimit - height / 2;
			}

			if (center.x - width / 2 < leftlimit) {
				//adjust x
				center.x = leftlimit + width / 2;
			}

			if (center.x + width / 2 > rightlimit) {
				//adjust x
				center.x = rightlimit - width / 2;
			}

			for (auto& rect : restricted) {
				FixView(width, height, center, rect);
			}
		}

		view->setCenter(center);


		

		//check for view rect dragging downwards

		
		//if (character->getCenterPosition().y + height / 2 > downlimit) {
		//	view->setCenter(Vector2f(character->getCenterPosition().x, downlimit - height / 2));
		//}
		//else if (character->getCenterPosition().y - height / 2 < uplimit) {
		//	view->setCenter(character->getCenterPosition().x, uplimit + height / 2);
		//}

		//if (character->getUpMostY() <= lowerbound && character->getDownMostY() >= upperbound) { //normal, only update x-coor
		//	view->setCenter(Vector2f( character->getCenterPosition().x, height/2 ));
		//}
		//else if(top + height <= downlimit && top >= uplimit) {
		//	// update y coor with center to char

		//	view->setCenter(character->getCenterPosition());
		//}
		//else if(top + height > downlimit) { // the view rect cannot get any lower
		//	view->setCenter(Vector2f( character->getCenterPosition().x , downlimit-height/2));
		//}
		//else view->setCenter(character->getCenterPosition().x, uplimit + height / 2);

		//top = view->getCenter().y - view->getSize().y / 2;
		//height = view->getSize().y;

		//if (top + height > downlimit) {
		//	view->setCenter(Vector2f(character->getCenterPosition().x, downlimit - height / 2));
		//}
		//else if (top < uplimit) {
		//	view->setCenter(character->getCenterPosition().x, uplimit + height / 2);
		//}

	}

	sf::Vector2f GetViewCenter() {
		return view->getCenter();
	}

	sf::Vector2f GetViewSize() {
		return view->getSize();
	}

	sf::Vector2f GetTopLeftCoordinates() {
		int left = view->getCenter().x - view->getSize().x / 2;
		int top = view->getCenter().y - view->getSize().y / 2;

		return (sf::Vector2f(left, top));
	}

	void GetViewInfo() {
		auto center = GetViewCenter();
		auto size = GetViewSize();
		std::cout << "Top left coords: ";
		std::cout << center.x - size.x / 2 << "," << center.y - size.y / 2 << '\n';
		std::cout << "Center coords: ";
		std::cout << center.x  << "," << center.y << '\n';
	}


	//helpers

	void FixView(int width, int height, sf::Vector2f& center, FloatRect rect) {
		FloatRect tmp = FloatRect(center.x - width / 2, center.y - height / 2, width, height);

		if (tmp.intersects(rect)) {
			if (tmp.top <= rect.top) {
				center.y = rect.top - height / 2;
			}
			else center.y = rect.top + rect.height + height / 2;
		}
	}
};