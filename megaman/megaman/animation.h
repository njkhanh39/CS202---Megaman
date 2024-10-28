#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
#include <map>
#include <string>

using namespace sf;

class AnimationComponent {
private:
	class Animation {
	public:
		Sprite& sprite;
		Texture& textureSheet;
		float animationTimer;
		float timer;
		int width, height;
		IntRect currentRect, startRect,endRect;
		int curRectIndex;

		Animation(Sprite& _sprite, Texture& _textureSheet, float animation_timer, int start_frame_x, int start_frame_y,
			int frames_x, int frames_y, int _width, int _height) 
			: sprite(_sprite), textureSheet(_textureSheet), animationTimer(animation_timer), width(_width), height(_height)
		{	
			curRectIndex = 0;
			timer = 0;
			
			startRect = IntRect(start_frame_x*width, start_frame_y*height, _width, _height);
			currentRect = startRect;
			endRect = IntRect(frames_x*width, frames_y*height, _width, _height);

			sprite.setTexture(textureSheet, true);
			sprite.setTextureRect(startRect);
		}

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

				//note this, since sprite can changes textureSheet explicitly!
				sprite.setTexture(textureSheet, true);
				sprite.setTextureRect(currentRect);
			}
		}

		void Reset() {
			timer = 0.f;
			currentRect = startRect;

			sprite.setTexture(textureSheet, true);
			sprite.setTextureRect(startRect);
		}

		//getters

		//0-indexing
		int GetFrameIndex() {
			return curRectIndex;
		}
	};

	Sprite& sprite;
	std::map<std::string, Animation*> animations;

public:
	//pick which animation to update in here
	AnimationComponent(Sprite& sprite);	
	virtual ~AnimationComponent();

	//functions

	void AddAnimation(const std::string key, Texture& texture_sheet, 
		float animation_timer, int start_frame_x, int start_frame_y,
		int frames_x, int frames_y, int _width, int _height);

	void Play(const std::string key, const float& dt);

	void Reset(const std::string key);

	void Reset();
};