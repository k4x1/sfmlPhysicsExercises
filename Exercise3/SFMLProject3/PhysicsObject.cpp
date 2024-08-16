#include "PhysicsObject.h"
#include "PhysicsLibrary.h"
#include <iostream>
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

        if (Velocity.x != 0 || Velocity.y != 0) {
            sf::Vector2f velocityUnitVector = Normalize(Velocity);
            sf::Vector2f frictionForce = -velocityUnitVector * g_FrictionCoefficient;     
      //      std::cout <<"x: " << Velocity.x << " | y:" << Velocity.y << std::endl;
            Velocity.y = -(Velocity + frictionForce).y;
        }
        else {
            Velocity.y = -Velocity.y;
        }
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
void PhysicsObject::ApplyDrag(float dragCoefficient) {

    float speed = GetVectorLength(Velocity);

    if (speed == 0) return;

    sf::Vector2f velocityUnitVector = Velocity / speed;

    float dragMagnitude = 0.5f * 1.0f * speed * speed * GetRadius() * dragCoefficient;

    sf::Vector2f dragForce = -dragMagnitude * velocityUnitVector;

    std::cout << dragForce.y << std::endl;
    ApplyForce(dragForce);
}
