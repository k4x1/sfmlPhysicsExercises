#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsLibrary.h"
class Spring
{
public:
	Spring(class PhysicsObject* _objectA, PhysicsObject* _objectB);

	~Spring();

	void FixedUpdate();

	void Draw(sf::RectangleShape& _rect, sf::RenderWindow& _window);

private:
	class PhysicsObject* ObjectA = nullptr;
	PhysicsObject* ObjectB = nullptr;

	float restLength = 0.0f;
};

