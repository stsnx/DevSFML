#pragma once
#include<SFML/Graphics.hpp>
#include "Animation.h"
#include"Collider.h"
using namespace sf;
class Boss
{
public:
	Boss(Texture* texture, Vector2u imageCount, float switchTime, Vector2f position);
	void Update(float time);
	void Draw(RenderWindow& window);
	Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }
	Vector2f GetSize() { return body.getSize(); }
	float GetHealth() { return info_boss.health; }
	float GetMaxHealth() { return info_boss.maxHealth; }
	struct info
	{
		float maxHealth;
		float health;
		int atkSpd;
	} info_boss;
	float dmg;
private:
	RectangleShape body;
	Animation animation;
	unsigned int row;
	float speed;
	Vector2f velocity;
	RectangleShape healthBar;
	RectangleShape currentHealth;


};