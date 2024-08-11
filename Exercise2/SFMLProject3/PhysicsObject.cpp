#include "PhysicsObject.h"
#include "PhysicsLibrary.h"

PhysicsObject::PhysicsObject(sf::Vector2f _position, float _mass)
{
    Position = _position;
    Mass = _mass;
}

void PhysicsObject::ApplyForce(sf::Vector2f _force)
{
    Acceleration += (_force / Mass) / 60.0f;
}

void PhysicsObject::UpdatePhysics()
{
    Velocity += Acceleration;
    Position += Velocity * g_MoveScale / 60.0f;
    Acceleration = { 0.0f, 0.0f };
}

void PhysicsObject::SetMass(float _mass)
{
    Mass = _mass;
}

void PhysicsObject::CollideObject(sf::RenderWindow& _window)
{
    if (Position.y > _window.getSize().y - GetRadius()) {
        Position.y = _window.getSize().y - GetRadius();
        Velocity.y = -Velocity.y;
    }
    if (Position.y < GetRadius()) {
        Position.y = GetRadius();
        Velocity.y = -Velocity.y;
    }
    if (Position.x > _window.getSize().x - GetRadius()) {
        Position.x = _window.getSize().x - GetRadius();
        Velocity.x = -Velocity.x;
    }
    if (Position.x < GetRadius()) {
        Position.x = GetRadius();
        Velocity.x = -Velocity.x;
    }
}

float PhysicsObject::GetRadius()
{
    return sqrt(Mass)*g_MassSizeScale;
}

float PhysicsObject::GetMass()
{
    return Mass;
}

sf::Vector2f PhysicsObject::GetPosition()
{
    return Position;
}
