#pragma once

#include "PhysicsLibrary.h"
#include <SFML/Graphics.hpp>

class PhysicsObject
{
private:
	sf::Vector2f Acceleration = { 0.0f, 0.0f };
	sf::Vector2f Velocity = { 0.0f, 0.0f };
	sf::Vector2f Position = { 0.0f, 0.0f };
	float Mass = 1.0f;
public:
	PhysicsObject(sf::Vector2f _position, float _mass);
	~PhysicsObject();

	void AddForce(sf::Vector2f _force);

	void ApplyResistance(float density);

	void UpdatePhysics();

	float GetRadius();

	void SetPosition(sf::Vector2f _position);
	sf::Vector2f GetPosition();
	void SetVelocity(sf::Vector2f _velocity);
	sf::Vector2f GetVelocity();
	float GetMass();
	void SetMass(float _mass);

	void CollideObject(sf::RenderWindow& _window);

};