#include "character.h"


Character::Character(TextureManager* textureManager, float x, float y): Entity(textureManager, x,y) {


	setSize(framesize);

	//move sprite accordingly to frame
	fixSpriteToFrame();


	//Weapon

	blaster = new XBuster(textureManager);


	//Health

	this->health = 30;

	//--------Load and add animations--------------

	LoadAndAddAnimations();


	//------SCALING ORIGINAL IMAGE---------

	//scale image, but dont forget the hitbox!
	this->sprite.setScale(scaleFactor, scaleFactor);
	this->blaster->ScaleProjectileAnimation(scaleFactor, scaleFactor);
}

Character::~Character() {

	//others
	std::cout << "Destructor of Character.\n";
	delete blaster;
}

//virtual drawing

void Character::Render(RenderWindow* l_window) {
	l_window->draw(frame);
	l_window->draw(sprite);
	blaster->RenderProjectiles(l_window);
}




//actions

void Character::HandleEventInput(Event& evt, Time& elapsed) {
	if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::E) {
		Shoot();
	}
	else if (evt.type == Event::KeyReleased && evt.key.code == Keyboard::E) {
		std::cout << "Release!\n";

		ChargeShoot();
	}
	else isChargeShooting = false;
	
}

void Character::HandleMovements(Time& elapsed) {
	float f_elapsed = elapsed.asSeconds();


	//moving
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

	//-------Shooting needs to be in "HandlingEvent"

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
	//	Shoot();//isShooting is handled in the function
	//}
	//else isShooting = false;

	// (right,left,jump) flags are modified by World::HandleCollision() in game loop
}

void Character::Shoot() {
	if (!isShooting) {
		isChargeShooting = false;
		blaster->Shoot(direction);
		blaster->Charge();
		isShooting = true;
	}
}


void Character::ChargeShoot() {
	isShooting = false;
	blaster->UnCharge();
	blaster->ChargeShoot(direction);
	isChargeShooting = true;
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

void Character::Update(float delt) {

	//movements
	this->Entity::UpdateMovements(delt);
	//update char's projectiles
	this->UpdateCharacterProjectiles(delt);

}

//--------------------PRIVATES-----------------------

void Character::LoadAndAddAnimations() {

	this->textureManager->BorrowTexture("Animation\\X\\X_IdleLeft.png", texture_idle_left);
	this->textureManager->BorrowTexture("Animation\\X\\X_IdleRight.png", texture_idle_right);
	this->textureManager->BorrowTexture("Animation\\X\\X_JumpLeft.png", texture_jump_left);
	this->textureManager->BorrowTexture("Animation\\X\\X_JumpRight.png", texture_jump_right);

	this->textureManager->BorrowTexture("Animation\\X\\X_ShootLeft.png", texture_shoot_left);
	this->textureManager->BorrowTexture("Animation\\X\\X_ShootJumpLeft.png", texture_shoot_jump_left);
	this->textureManager->BorrowTexture("Animation\\X\\X_ShootRight.png", texture_shoot_right);
	this->textureManager->BorrowTexture("Animation\\X\\X_ShootJumpRight.png", texture_shoot_jump_right);
	//animations


	movingAnimation->AddAnimation("MovingRight", "Animation\\X\\X_MovementRight.png", 100.f, 0, 0, 6, 0, 137, 142);
	movingAnimation->AddAnimation("MovingLeft", "Animation\\X\\X_MovementLeft.png", 100.f, 0, 0, 6, 0, 137, 142);
	movingAnimation->AddAnimation("MovingShootRight", "Animation\\X\\X_MovementShootRight.png", 100.f, 0, 0, 5, 0, 137, 142);
	movingAnimation->AddAnimation("MovingShootLeft", "Animation\\X\\X_MovementShootLeft.png", 100.f, 0, 0, 5, 0, 137, 142);
}


//-------UPDATING-------

void Character::UpdateCharacterProjectiles(float delt) {

	Vector2f pos = getPosition();
	Vector2f size = getFrameSize();

	//update char's projectiles by updating his gun's projectiles
	blaster->UpdateMovingProjectiles(delt, { pos.x + size.x, pos.y + size.y / 2 });
}


//------Helpers----

void Character::setPosition(Vector2f vec){
	frame.setPosition(vec);
	auto x = vec.x;
	auto y = vec.y;
	sprite.setPosition({ x - dilation,y });
}

void Character::fixSpriteToFrame() {
	auto vec = getUpLeftPosition();
	auto x = vec.x;
	auto y = vec.y;
	sprite.setPosition({ x - dilation,y });
}