#include "animation.h"

AnimationComponent::AnimationComponent(Sprite* sprite)
: sprite(sprite)
{
	
}

AnimationComponent::~AnimationComponent() {
	std::cout << "Destructor of Animation Component\n";
	for (auto& it : animations) {
		if(it.second) delete it.second;
	}
}

void AnimationComponent::AddAnimation(const std::string key, const std::string& sheetFile, float animation_timer, int start_frame_x, int start_frame_y,
	int frames_x, int frames_y, int _width, int _height) {

	animations[key] = new Animation(sprite, sheetFile, animation_timer, start_frame_x, start_frame_y,
		frames_x, frames_y, _width, _height);

	animations[key]->textureSheet = new Texture();
	animations[key]->textureSheet->loadFromFile(sheetFile);
}

void AnimationComponent::Play(const std::string key, const float& dt) {
	if (!animations[key]) return;
	animations[key]->Play(dt);
}

void AnimationComponent::Reset(const std::string key) {
	if (!animations[key]) return;
	animations[key]->Reset();
}

void AnimationComponent::Reset() {
	for (auto& x : animations) {
		if(x.second) x.second->Reset();
	}
}