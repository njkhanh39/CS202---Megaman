#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
#include <map>
#include <string>
#include "Manager/TextureManager.h"

using namespace sf;

class AnimationComponent {
private:
	class Animation {
	public:
		Sprite* sprite; //points to entity's sprite
		Texture* textureSheet; //we borrow 
		float animationTimer;
		float timer;
		int width, height;
		IntRect currentRect, startRect,endRect;
		int curRectIndex;

		TextureManager* textureManager;

		Animation(TextureManager* textureManager, Sprite* _sprite, const std::string& sheetFile, float animation_timer, int start_frame_x, int start_frame_y,
			int frames_x, int frames_y, int _width, int _height) 
			: textureManager(textureManager), sprite(_sprite), animationTimer(animation_timer), width(_width), height(_height)
		{	

			/*textureSheet = new Texture();
			textureSheet->loadFromFile(sheetFile);*/
			
			//we borrow texture

			this->textureManager->BorrowTexture(sheetFile, textureSheet);

			curRectIndex = 0;
			timer = 0;
			
			startRect = IntRect(start_frame_x*width, start_frame_y*height, _width, _height);
			currentRect = startRect;
			endRect = IntRect(frames_x*width, frames_y*height, _width, _height);

			sprite->setTexture(*textureSheet, true);
			sprite->setTextureRect(startRect);
		}


		//----**THIS ONE DOES SHALLOW COPY ON SPRITE AND BORROWED TEXTURE, DEEP COPY ON OTHERS**------//

		Animation(const Animation& other): animationTimer(other.animationTimer), timer(other.timer), width(other.width),
		height(other.height), currentRect(other.currentRect), startRect(other.startRect), endRect(other.endRect),
		curRectIndex(other.curRectIndex), textureSheet(other.textureSheet), textureManager(other.textureManager){

			//sprite is initially shallowly copied
			sprite = other.sprite;


			
		}

		/*friend void swap(Animation& obj1, Animation& obj2) {
			std::swap(obj1.sprite, obj2.sprite);
			std::swap(obj1.textureSheet, obj2.textureSheet);
			std::swap(obj1.animationTimer, obj2.animationTimer);
			std::swap(obj1.timer, obj2.timer);
			std::swap(obj1.width, obj2.width);
			std::swap(obj1.height, obj2.height);
			std::swap(obj1.currentRect, obj2.currentRect);
			std::swap(obj1.startRect, obj2.startRect);
			std::swap(obj1.endRect, obj2.endRect);
			std::swap(obj1.curRectIndex, obj2.curRectIndex);
		}

		Animation& operator=(Animation obj) {
			swap(*this, obj);
			return *this;
		}*/

		//Functions

		void Play(const float& dt) {//dt = delta time
			timer += 600.f * dt;
			if (timer >= animationTimer) {

				//reset timer
				timer = 0.f;

				//animate
				if (currentRect != endRect) {
					curRectIndex++;
					currentRect.left += width;
				}
				else { //reset
					curRectIndex = 0;
					currentRect.left = startRect.left;
				}

				//note this, since sprite can change textureSheet explicitly!
				sprite->setTexture(*textureSheet, true);
				sprite->setTextureRect(currentRect);
			}
		}

		~Animation() {
			//std::cout << "Destructor of Animation Component\n";
			
			//delete textureSheet;  //borrowed, so do not delete!

			//dont delete sprite, it will be deleted in entity
		}

		void Reset() {
			timer = 0.f;
			currentRect = startRect;

			sprite->setTexture(*textureSheet, true);
			sprite->setTextureRect(startRect);
		}

		//getters

		//0-indexing
		int GetFrameIndex() {
			return curRectIndex;
		}
	};

	TextureManager* textureManager; //dont delete
	Sprite* sprite;
	std::map<std::string, Animation*> animations;
public:

	
	//pick which animation to update in here
	AnimationComponent(TextureManager* textureManager, Sprite* sprite);


	//----------**Does Shallow copy on sprite, deep copy on others**----------//

	AnimationComponent(const AnimationComponent& other) {
		sprite = other.sprite;
		this->textureManager = other.textureManager;
		for (auto& a : other.animations) {
			std::string key = a.first;

			//invoke copy constructor
			animations[key] = new Animation(*a.second);
		}
	}

	virtual ~AnimationComponent();

	//functions

	void AddAnimation(const std::string key, const std::string& sheetFile, 
		float animation_timer, int start_frame_x, int start_frame_y,
		int frames_x, int frames_y, int _width, int _height);


	void Play(const std::string key, const float& dt);

	void Reset(const std::string key);

	void Reset();

	//helpers

	void SetAllAnimationsSprite(Sprite* ptr) {
		sprite = ptr;
		for (auto& a : animations) {
			a.second->sprite = ptr;
		}
	}

	bool DoesExist(const std::string& key) {
		if (animations[key] != nullptr) {
			return true;
		}
	}
};