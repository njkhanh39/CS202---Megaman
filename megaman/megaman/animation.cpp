#include "animation.h"

AnimationComponent::AnimationComponent(Sprite& sprite)
: sprite(sprite)
{
	
}

AnimationComponent::~AnimationComponent() {
	for (auto& it : animations) {
		delete it.second;
	}
}

void AnimationComponent::AddAnimation(const std::string key, Texture& texture_sheet, float animation_timer, int start_frame_x, int start_frame_y,
	int frames_x, int frames_y, int _width, int _height) {

	animations[key] = new Animation(sprite, texture_sheet, animation_timer, start_frame_x, start_frame_y,
		frames_x, frames_y, _width, _height);
}

void AnimationComponent::Play(const std::string key, const float& dt) {
	animations[key]->Play(dt);
}

void AnimationComponent::Reset(const std::string key) {
	animations[key]->Reset();
}

void AnimationComponent::Reset() {
	for (auto& x : animations) {
		x.second->Reset();
	}
}