#include "Boss.h"
Boss::Boss(Texture* texture, Vector2u imageCount, float switchTime, Vector2f position) :
	animation(texture, imageCount, switchTime)
{
	row = 0;
	this->info_boss.maxHealth=4000;
	this->info_boss.health = this->info_boss.maxHealth;
	this->dmg = 800;
	this->speed = 50;
	float Health = this->info_boss.health;
	float Maxhealth = this->info_boss.maxHealth;
	body.setSize(Vector2f(187.0f, 262.0f));
	healthBar.setSize(Vector2f(300.0f, 8.0f));
	healthBar.setPosition(Vector2f(position.x - 150.0f, position.y + 24.5f));
	healthBar.setFillColor(Color::White);
	float current = 300 * Health / Maxhealth;
	currentHealth.setSize(Vector2f(current - 1.0f, 4.0f));
	currentHealth.setPosition(Vector2f(position.x - 149.0f, position.y + 24.5f));
	currentHealth.setFillColor(Color::Red);
	body.setOrigin(body.getSize() / 2.0f);
	//healthBar.setOrigin(healthBar.getSize() / 2.0f);
	body.setTexture(texture);
	body.setPosition(position);
}
void Boss::Update(float time)
{
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	float Health = this->info_boss.health;
	float Maxhealth = this->info_boss.maxHealth;
	speed = this->speed;
	float current = 300 * Health / Maxhealth;
	currentHealth.setSize(Vector2f(current, 8.0f));
	currentHealth.setFillColor(Color::Red);
	if (body.getPosition().x < -100 || body.getPosition().x > 100) speed *= -1;
	velocity.x += speed;
	if (body.getPosition().y < -220) 
	velocity.y += 50;
	animation.Updatexy(row, time);
	body.setTextureRect(animation.xyRect);
	body.move(velocity * time);
	healthBar.move(velocity * time);
	currentHealth.move(velocity * time);
	row = 0;
}

void Boss::Draw(RenderWindow& window)
{
	window.draw(body);
	window.draw(healthBar);
	window.draw(currentHealth);
}
