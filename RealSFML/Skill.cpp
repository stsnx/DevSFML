#include "Skill.h"

Skill::Skill(Texture* texture, Vector2u imageCount, float switchTime, Vector2f position,float speed, Vector2f size)
	: animation(texture, imageCount, switchTime)
{
	this->dmg = 150;
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	body.setTexture(texture);
	body.setPosition(position);
	this->row = 0;
	this->speed = speed;
	this->health = 800;
}

void Skill::Draw(RenderWindow& window)
{
	window.draw(body);
}

void Skill::Update(float time)
{
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x -= speed;
	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x += speed;
	if (Keyboard::isKeyPressed(Keyboard::Up))
		velocity.y -= speed;
	if (Keyboard::isKeyPressed(Keyboard::Down))
		velocity.y += speed;
	animation.Updatexy(row, time);
	body.setTextureRect(animation.xyRect);
	body.move(velocity * time);
}
