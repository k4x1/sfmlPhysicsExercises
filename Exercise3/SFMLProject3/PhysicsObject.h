#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsObject.h"
class PhysicsObject {
private:
	sf::Vector2f Acceleration = { 0,0 };
	sf::Vector2f Velocity = { 0,0 };
	sf::Vector2f Position = { 0,0 };
	float Mass = 1.0f;
public:
	PhysicsObject(sf::Vector2f _position, float _mass);

	void ApplyForce(sf::Vector2f _force);
	void UpdatePhysics();
	void SetMass(float _mass);
	void CollideObject(sf::RenderWindow& _window);
	void ApplyDrag(float _dragCoefficient);
	float GetMass();
	float GetRadius();
	sf::Vector2f GetPosition();
};