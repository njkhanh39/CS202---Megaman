#ifndef ATTACKENEMIES_H
#define ATTACKENEMIES_H

#include "shooterenemies.h"

class AttackEnemy1 : public MovingEnemy { //that spiky wheel
public:
	AttackEnemy1(TextureManager* textureManager, float x, float y, Direction direction):
	MovingEnemy(textureManager, x, y, 28.5, 28.5, 200, direction, Vector2f(x,y), 500) {

		this->velocityX = 70;
		this->collisiondamage = 30;

		InnitAnimation();


		//this dude needs to be scaled
		this->sprite.setScale(0.75, 0.75);
	}

	 void Update(Character* character, float delt) override{
		 this->Entity::UpdateEntity(delt);
		this->UpdateEnemyBehaviour(character, delt);
		 this->AttackCharacter(character, delt);
	 }

	 void Render(RenderWindow* l_window) override {
		// l_window->draw(frame);
		 l_window->draw(sprite);
	 }



	//We dont touch this unless the enemy has a gun :)

	void HandleProjectileCollision(Obstacle* obs, Entity* en) override {}

	void HandleProjectileCollision(Entity* en) override {}

	void HandleProjectileCollision(Obstacle* obs) override {}

private:

	void AttackCharacter(Character* character, float delt) {
		//meh, dude just moves around and hits stuffs
	}

	void UpdateEnemyBehaviour(Character* character, float delt) {
		this->MovingEnemy::UpdateEnemyBehaviour(character, delt); //reuse
	}

	void InnitAnimation() {
		textureManager->BorrowTexture("Animation\\Map1\\AttackEnemy1\\spikewheel_idle.png", this->texture_idle_left);
		textureManager->BorrowTexture("Animation\\Map1\\AttackEnemy1\\spikewheel_idle.png", this->texture_idle_right);

		movingAnimation->AddAnimation("MovingLeft", "Animation\\Map1\\AttackEnemy1\\spikewheel_move.png",
			100.f, 0, 0, 2, 0, 38, 38);
		movingAnimation->AddAnimation("MovingRight", "Animation\\Map1\\AttackEnemy1\\spikewheel_move.png",
			100.f, 0, 0, 2, 0, 38, 38);
	}
};

#endif 
