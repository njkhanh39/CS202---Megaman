#include "character.h"


Character::Character(TextureManager* textureManager, float x, float y): Entity(textureManager, x,y) {


	setSize(framesize);

	//move sprite accordingly to frame


	//Weapon

	blaster = new XBuster(textureManager);


	//Health

	this->health = 300;
	this->invisibleMaxTimer = 100.f; //the bigger, the longer character remains invisible
	this->velocityX = 120;
	this->gravity = 500;


	//dilation

	this->dilation = { 5.f, 0.f };
	setPosition({ x,y });

	//--------Load and add animations--------------

	LoadAndAddAnimations();


	//------SCALING ORIGINAL IMAGE---------

	//scale image, but dont forget the hitbox!
	this->sprite.setScale(scaleFactor, scaleFactor); //already set frame size ~ scaled animation
	this->blaster->ScaleProjectileAnimation(scaleFactor, 0.85); //note that projectile's frame size needs = scaled animation
}

Character::~Character() {

	//others
	std::cout << "Destructor of Character.\n";
	delete blaster;
}

//virtual drawing

void Character::Render(RenderWindow* l_window) {
	//l_window->draw(frame);
	l_window->draw(sprite);
	blaster->RenderProjectiles(l_window);
}




//actions

void Character::HandleEventInput(Event& evt, Time& elapsed) {
	if (this->invisible) return; //when invisible, cannot do anything

	if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::Enter) {
		Shoot(elapsed.asSeconds());
	}
	else if (evt.type == Event::KeyReleased && evt.key.code == Keyboard::Enter) {
		std::cout << "Release!\n";

		ChargeShoot();
	}
	else isChargeShooting = false;
	
}

void Character::HandleMovements(Time& elapsed) {
	float f_elapsed = elapsed.asSeconds();

	if (this->invisible) return; //when invisible, cannot do anything
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

void Character::Shoot(float delt) {
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


void Character::Update(float delt) {

	//movements
	this->Character::UpdateEntity(delt);
	//update char's projectiles
	this->UpdateCharacterProjectiles(delt);

}

//---------IMPORTANT OVERRIDE--------
void Character::UpdateEntity(float delt)  {
	//Death
	if (this->IsDead()) return;

	//invisibility 

	if (this->invisible) { //true = is being hurt

		//---Immortal---//
		if (this->temphealth == -1) {
			this->temphealth = this->health;
			this->health = IMMORTAL;
		}


		this->invisibleTimer += 50 * delt;

		if (invisibleTimer >= invisibleMaxTimer) { // we are done with invisibility!
			this->invisible = false;
			//----BACK TO NORMAL---//

			this->invisibleTimer = 0;
			this->health = this->temphealth;
			this->temphealth = -1;
		}

	
	}


	//---Movement----

	if (isJumping || fall) {
		//(make character fall)


		if (isGrabbing) {
			velocityY += slowGravity * delt;
		}
		else {
			velocityY += gravity * delt;
		}

		//when reaches a limit, fall & isJumping will be turned to false
		//by our canKeepFalling function


		//---------INVISIBLE---------//
		if (this->invisible) velocityY = 0;

		//move our 2 attributes

		sprite.move(0, velocityY * delt);
		frame.setPosition({ frame.getPosition().x, frame.getPosition().y + velocityY * delt });
	}
	else velocityY = 0.0f; //not falling & not jumping = 0 velocityY ?? :D

	//-----ANIMATION UPDATE---------

	//from first if to last if, it follows the animation priority

	if (invisible) { //play the hurt animation
		movingAnimation->Play("HurtAnimation", delt / 2); //make animation slower by dividing
	}
	else if (isJumping) {
		if (isShooting) {
			if (direction == Direction::Right && texture_shoot_jump_right) sprite.setTexture(*texture_shoot_jump_right, true);
			if (direction == Direction::Left && texture_shoot_jump_left) sprite.setTexture(*texture_shoot_jump_left, true);
		}
		else {
			if (direction == Direction::Right && texture_jump_right) sprite.setTexture(*texture_jump_right, true);
			if (direction == Direction::Left && texture_jump_left) sprite.setTexture(*texture_jump_left, true);
		}
	}
	else if (isRight) {
		if (isShooting) movingAnimation->Play("MovingShootRight", delt);
		else movingAnimation->Play("MovingRight", delt);
	}
	else if (isLeft) {
		if (isShooting) movingAnimation->Play("MovingShootLeft", delt);
		else movingAnimation->Play("MovingLeft", delt);
	}
	else {
		//Character not jumping, not running

		//this one helpssss, animations can colide (play many at the same time)
		movingAnimation->Reset();

		//-----PURE SPRITE SET, NOT ANIMATION-----//

		if (isShooting) {
			if (direction == Direction::Left && texture_shoot_left) sprite.setTexture(*texture_shoot_left);
			else if (texture_shoot_right) sprite.setTexture(*texture_shoot_right);
		}
		else if (direction == Direction::Right && texture_idle_right) {
			sprite.setTexture(*texture_idle_right, true);
		}
		else if (texture_idle_left) {
			sprite.setTexture(*texture_idle_left, true);
		}
		else {
			//plays idle
			this->idleAnimation->Play("Idle", delt);
		}
	}
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

	movingAnimation->AddAnimation("HurtAnimation", "Animation\\X\\X_Hurt.png", 100.f, 0, 0, 1, 0, 130, 152);
	movingAnimation->AddAnimation("MovingRight", "Animation\\X\\X_MovementRight.png", 100.f, 0, 0, 6, 0, 137, 142);
	movingAnimation->AddAnimation("MovingLeft", "Animation\\X\\X_MovementLeft.png", 100.f, 0, 0, 6, 0, 137, 142);
	movingAnimation->AddAnimation("MovingShootRight", "Animation\\X\\X_MovementShootRight.png", 100.f, 0, 0, 5, 0, 137, 142);
	movingAnimation->AddAnimation("MovingShootLeft", "Animation\\X\\X_MovementShootLeft.png", 100.f, 0, 0, 5, 0, 137, 142);

	//idle...
}


//-------UPDATING-------

void Character::UpdateCharacterProjectiles(float delt) {

	Vector2f pos = getPosition();
	Vector2f size = getFrameSize();

	//update char's projectiles by updating his gun's projectiles
	blaster->UpdateMovingProjectiles( delt, getCenterPosition() + Vector2f(0.f,-10.f));
}


//------Helpers----