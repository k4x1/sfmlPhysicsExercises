#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsObject.h"
class PhysicsObject {
private:
	sf::Vector2f Acceleration = { 0,0 };
	sf::Vector2f Velocity = { 0,0 };
	sf::Vector2f StartPosition = { 0,0 };
	sf::Vector2f EndPosition = { 0,0 };

	float Mass = 1.0f;
	float Length = 0.0f;
public:
	float Restitution = 0.0f;
	PhysicsObject(sf::Vector2f _startPosition, sf::Vector2f _endPosition, float _mass);

	void ApplyForce(sf::Vector2f _force);
	void ApplyImpulse(sf::Vector2f _impulse);
	
	void UpdatePhysics();
	
	void SetMass(float _mass);
		
	void CollideObject(sf::RenderWindow& _window);
	void CollideCapsules(std::vector<PhysicsObject*> _capsules);

	float GetMass();
	float GetRadius();
	float GetLength();

	sf::Vector2f GetStartPosition();
	sf::Vector2f GetEndPosition();
};