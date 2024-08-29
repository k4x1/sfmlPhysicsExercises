/*
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Spring.h
Description : Header file for the Spring class, which simulates a spring connection between two PhysicsObjects.
Author : Kazuo Reis de Andrade
Mail : kazuo.andrade@mds.ac.nz
*/
#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsLibrary.h"
class Spring
{
private:
	class PhysicsObject* ObjectA = nullptr;
	PhysicsObject* ObjectB = nullptr;

	float restLength = 0.0f;
public:
	Spring(class PhysicsObject* _objectA, PhysicsObject* _objectB);

	~Spring();

	void Simulate();

	void Draw(sf::RectangleShape& _rect, sf::RenderWindow& _window);

};

