#include "Spring.h"
#include "PhysicsObject.h"


Spring::Spring(PhysicsObject* _objectA, PhysicsObject* _objectB)
{
	ObjectA = _objectA;
	ObjectB = _objectB;

	restLength = GetVectorLength(ObjectA->GetPosition() - ObjectB->GetPosition());
}

Spring::~Spring()
{
}

void Spring::FixedUpdate()
{
	if (!ObjectA || !ObjectB) return;
	
	sf::Vector2f displacement = ObjectB->GetPosition() - ObjectA->GetPosition();
	sf::Vector2f nDisplacement = NormalizeVector(displacement);
	float distance = GetVectorLength(displacement);
	float distanceDif = distance - restLength;

	float springConstant = 5.0f;
	sf::Vector2f force = nDisplacement * springConstant * distanceDif;

	float damping = 0.9f;
	float dotForceA = DotProduct(ObjectA->GetVelocity(), nDisplacement);
	float dotForceB = DotProduct(ObjectB->GetVelocity(), -nDisplacement);
	sf::Vector2f dampingForceA = dotForceA * nDisplacement * damping;
	sf::Vector2f dampingForceB = dotForceB * -nDisplacement * damping;

	ObjectA->AddForce((force - dampingForceA) * 0.5f);
	ObjectB->AddForce((force - dampingForceB) * -0.5f);
}

void Spring::Draw(sf::RectangleShape& _rect, sf::RenderWindow& _window)
{
	float sizeX = GetVectorLength(ObjectB->GetPosition() - ObjectA->GetPosition());
	float sizeY = ObjectA->GetRadius() * 2 * 0.5f;
	_rect.setSize(sf::Vector2f(sizeX, sizeY));

	_rect.setOrigin(0.0f, ObjectA->GetRadius() * 0.5f);
	_rect.setRotation(GetAngle(ObjectB->GetPosition() - ObjectA->GetPosition()));
	_rect.setPosition(ObjectA->GetPosition());
	_window.draw(_rect);
}