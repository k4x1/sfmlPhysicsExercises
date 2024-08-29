/*
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : PhysicsObject.cpp
Description : Implementation file for the PhysicsObject class, which manages physics properties and behaviors for game objects.
Author : Kazuo Reis de Andrade
Mail : kazuo.andrade@mds.ac.nz
*/
#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(sf::Vector2f _position, float _mass)
{
	Position = _position;
	Mass = _mass;
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::AddForce(sf::Vector2f _force)
{
	Acceleration += (_force / Mass)/60.0f;
}


void PhysicsObject::UpdatePhysics()
{
	Velocity += Acceleration;
	Position += Velocity * g_MoveScale / 60.0f;
	Acceleration = { 0.0f, 0.0f };
}

float PhysicsObject::GetRadius()
{
	return sqrt(Mass) * g_MassSizeScale;
}

void PhysicsObject::SetPosition(sf::Vector2f _position)
{
	Position = _position;
}

sf::Vector2f PhysicsObject::GetPosition()
{
	return Position;
}

void PhysicsObject::SetVelocity(sf::Vector2f _velocity)
{
	Velocity = _velocity;
}

sf::Vector2f PhysicsObject::GetVelocity()
{
	return Velocity;
}

float PhysicsObject::GetMass()
{
	return Mass;
}

void PhysicsObject::SetMass(float _mass)
{
	Mass = _mass;
}

void PhysicsObject::CollideObject(sf::RenderWindow& _window)
{
	if (Position.x > _window.getSize().x - GetRadius())
	{
		Position.x = _window.getSize().x - GetRadius();
		Velocity.x = -Velocity.x;
	}

	if (Position.x < GetRadius())
	{
		Position.x = GetRadius();
		Velocity.x = -Velocity.x;
	}

	if (Position.y > _window.getSize().y - GetRadius())
	{
		Position.y = _window.getSize().y - GetRadius();
		Velocity.y = -Velocity.y;

		sf::Vector2f Friction = NormalizeVector(-Velocity) * g_FrictionCoefficient * (Mass * g_Gravity.y);
		AddForce(Friction);
	}

	if (Position.y < GetRadius())
	{
		Position.y = GetRadius();
		Velocity.y = -Velocity.y;
	}
}
