#pragma once
#include<SFML/Graphics.hpp>
#include"Collider.h"
#include"Animation.h"
using namespace sf;
class Skill
{
public:
	Skill(Texture* texture, Vector2u imageCount, float switchTime, Vector2f position,float speed,Vector2f size);
	void Draw(RenderWindow& window);
	void Update(float time);
	Collider GetCollider() { return Collider(body); }
	Vector2f GetPosition() { return body.getPosition(); }
	Vector2f GetSize() { return body.getSize(); }
	float GetDmg() { return dmg; }
	float dmg;
	Vector2f velocity;
	RectangleShape body;
	float health;
	float Gethealth(){ return this->health; }
private:
	
	Animation animation;
	
	unsigned int row;
	float speed;
	
};

