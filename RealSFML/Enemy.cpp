#include "Enemy.h"
Enemy::Enemy(Texture* texture, Vector2u imageCount, float switchTime, Vector2f position):
	animation(texture, imageCount, switchTime)
{
	row = 1;
	this->info_enemy.maxHealth = 600;
	this->info_enemy.health = this->info_enemy.maxHealth;
	this->dmg = 800;
	float Health = this->info_enemy.health;
	float Maxhealth = this->info_enemy.maxHealth;
	body.setSize(Vector2f(32.0f, 40.0f));
	healthBar.setSize(Vector2f(40.0f, 4.0f));
	healthBar.setPosition(Vector2f(position.x - 19.0f, position.y - 24.5f));
	healthBar.setFillColor(Color::White);
	float current = 40 * Health / Maxhealth;
	currentHealth.setSize(Vector2f(current-1.0f, 4.0f));
	currentHealth.setPosition(Vector2f(position.x - 18.0f, position.y - 24.5f));
	currentHealth.setFillColor(Color::Red);
	body.setOrigin(body.getSize() / 2.0f);
	//healthBar.setOrigin(healthBar.getSize() / 2.0f);
	body.setTexture(texture);
	body.setPosition(position);
}
void Enemy::Update(float time)
{
	velocity.x = 0.0f;
	velocity.y = 100.0f;
	float Health = this->info_enemy.health;
	float Maxhealth = this->info_enemy.maxHealth;
	float current = 40 * Health / Maxhealth;
	currentHealth.setSize(Vector2f(current, 4.0f));
	currentHealth.setFillColor(Color::Red);
	animation.Updateyx(row, time);
	body.setTextureRect(animation.xyRect);
	body.move(velocity * time);
	healthBar.move(velocity * time);
	currentHealth.move(velocity * time);
	row = 1;
}

void Enemy::Draw(RenderWindow& window)
{
	window.draw(body);
	window.draw(healthBar);
	window.draw(currentHealth);
}
