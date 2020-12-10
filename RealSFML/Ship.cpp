#include "Ship.h"
Ship::Ship(Texture* texture, Vector2u imageCount, float switchTime, float speed) :
	animation(texture, imageCount, switchTime)
{
	this->speed = speed;
	this->info_ally.maxHealth = 1200;
	this->info_ally.health = this->info_ally.maxHealth;
	this->info_ally.maxmana = 620;
	this->info_ally.mana = this->info_ally.maxmana;
	this->info_ally.atkSpd = 300;
	this->info_ally.dmg = 100.0f;
	row = 1;
	body.setSize(Vector2f(30.0f, 40.0f)),
		body.setPosition(Vector2f(100.0f, 100.0f));
	body.setOrigin(body.getSize() / 2.0f);
	body.setTexture(texture);
	
}
void Ship::Update(float time)
{
	if (this->info_ally.expBar >= this->info_ally.levelGate)
	{
		this->info_ally.level++;
		this->info_ally.expBar -= this->info_ally.levelGate;
		this->info_ally.levelGate *= 1.35;
	}
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x -= speed,row=0;
	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x += speed,row=2;
	if (Keyboard::isKeyPressed(Keyboard::Up))
		velocity.y -= speed;
	if (Keyboard::isKeyPressed(Keyboard::Down))
		velocity.y += speed;
	animation.Updateyx(row, time);
	body.setTextureRect(animation.xyRect);
	body.move(velocity * time);
	row = 1;
}

void Ship::Draw(RenderWindow& window)
{
	window.draw(body);
}
