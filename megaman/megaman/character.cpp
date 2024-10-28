#include "character.h"


Character::Character(float x, float y): Entity(x,y) {


	setSize({ 80.f, 145.f });

	//move sprite accordingly to frame
	fixSpriteToFrame();


	//Weapon

	blaster = new Shooter();


	//--------Load--------------


	texture_idle_left->loadFromFile("Animation\\X_IdleLeft.png");
	texture_idle_right->loadFromFile("Animation\\X_IdleRight.png");
	texture_jump_left->loadFromFile("Animation\\X_JumpLeft.png");
	texture_jump_right->loadFromFile("Animation\\X_JumpRight.png");

	texture_shoot_left->loadFromFile("Animation\\X_ShootLeft.png");
	texture_shoot_right->loadFromFile("Animation\\X_ShootRight.png");
	texture_shoot_jump_left->loadFromFile("Animation\\X_ShootJumpLeft.png");
	texture_shoot_jump_right->loadFromFile("Animation\\X_ShootJumpRight.png");


	//animations

	texture_movement_right->loadFromFile("Animation\\X_MovementRight.png");
	texture_movement_left->loadFromFile("Animation\\X_MovementLeft.png");
	texture_movement_shoot_right->loadFromFile("Animation\\X_MovementShootRight.png");
	texture_movement_shoot_left->loadFromFile("Animation\\X_MovementShootLeft.png");



	movingAnimation->AddAnimation("MovingRight", *texture_movement_right, 100.f, 0, 0, 6, 0, 137, 142);
	movingAnimation->AddAnimation("MovingLeft", *texture_movement_left,100.f, 0, 0, 6, 0, 137, 142);
	movingAnimation->AddAnimation("MovingShootRight", *texture_movement_shoot_right, 100.f, 0, 0, 5, 0, 137, 142);
	movingAnimation->AddAnimation("MovingShootLeft", *texture_movement_shoot_left, 100.f, 0, 0, 5, 0, 137, 142);
}

Character::~Character() {

	//others

	delete blaster;
}

//virtual drawing

void Character::Render(RenderWindow* l_window) {
	l_window->draw(frame);
	l_window->draw(sprite);
	blaster->RenderProjectiles(l_window);
}




//actions

void Character::HandleInput(Time& elapsed) {
	float f_elapsed = elapsed.asSeconds();


	//moving, shooting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (right) {
			MoveRight(f_elapsed);
			isRight = true;
		}// f_elapsed is typically the frame time
		else isRight = false;
	}
	else isRight = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (left) {
			MoveLeft(f_elapsed);
			isLeft = true;
		}
		else isLeft = false;
	}
	else isLeft = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		Jump(f_elapsed);
		//isJumping is handled outside
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		Shoot();//isShooting is handled in the function
	}
	else isShooting = false;

	// (right,left,jump) flags are modified by World::HandleCollision() in game loop
}

void Character::Shoot() {
	if (!isShooting) {
		blaster->Shoot(direction);
		isShooting = true;
	}
}

//-----collision checks-----

void Character::HandleProjectileCollision(Obstacle* obs) {
	blaster->HandleProjectileCollision(obs);
}

void Character::HandleProjectileCollision(Obstacle* obs, Entity* en) {
	blaster->HandleProjectileCollision(obs, en);
}

void Character::HandleProjectileCollision(Entity* en) {
	blaster->HandleProjectileCollision(en);
}



bool Character::canKeepFalling(Obstacle* obs) {
	float lmy = getUpMostY(), rmy = getDownMostY();
	float umy = obs->getUpMostY(), vmy = obs->getDownMostY();

	if (rmy >= umy && (rmy - umy < 1.f || rmy-umy < 3.f)) {
		float l = getLeftMostX(), r = getRightMostX();
		float u = obs->getLeftMostX(), v = obs->getRightMostX();

		if (!(r < u || l > v)) {

			//make the position even
			
			frame.setPosition({ getLeftMostX(), umy - rmy + lmy });
			fixSpriteToFrame();
			
			return false;
		}

		return true;
	}
	return true;
}


//-------UPDATING-------

void Character::UpdateCharacterProjectiles(float delt) {

	Vector2f pos = getPosition();
	Vector2f size = getFrameSize();

	blaster->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y + size.y / 2 });
}


void Character::fixSpriteToFrame() {
	auto vec = getUpLeftPosition();
	auto x = vec.x;
	auto y = vec.y;
	sprite.setPosition({ x - 20.f,y });
}