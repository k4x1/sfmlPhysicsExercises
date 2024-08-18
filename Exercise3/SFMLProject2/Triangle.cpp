#include "Triangle.h"
#include "PhysicsLibrary.h"

float sign(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Triangle::ContainsPoint(sf::Vector2f point) {
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(point, p1, p2);
    d2 = sign(point, p2, p3);
    d3 = sign(point, p3, p1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}
void Triangle::ApplyForceToOverlappingObject(PhysicsObject* obj)
{
    if (ContainsPoint(obj->GetStartPosition()) || ContainsPoint(obj->GetEndPosition())) {
        sf::Vector2f center = (p1 + p2 + p3) / 3.0f;
        sf::Vector2f force = obj->GetStartPosition() - center;
        obj->ApplyForce(Normalize(force) * 10.0f); 
    }
}