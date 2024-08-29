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
	Acceleration += (_force / Mass);
}

void PhysicsObject::ApplyResistance(float density)
{
	// Calculate the speed of the PhysicsObject
	float speed = GetVectorLength(Velocity);

	// Calculate the normalized direction of the velocity (opposite for drag)
	sf::Vector2f dragDirection = NormalizeVector(-Velocity);

	// Calculate the drag force magnitude
	float dragMagnitude = density * g_DragCoefficient * speed * speed;

	// Calculate the drag force vector
	sf::Vector2f DragForce = dragDirection * dragMagnitude;

	// Apply the drag force to the PhysicsObject
	AddForce(DragForce );
}

void PhysicsObject::UpdatePhysics()
{
	Velocity += Acceleration;
	Position += Velocity * g_MoveScale;
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
