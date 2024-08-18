#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsLibrary.h"
#include "PhysicsObject.h"
#include <iostream>

class Triangle {
public:
    sf::Vector2f p1, p2, p3;

    Triangle(sf::Vector2f _p1, sf::Vector2f _p2, sf::Vector2f _p3)
        : p1(_p1), p2(_p2), p3(_p3) {}

    bool ContainsPoint(sf::Vector2f point);
    void ApplyForceToOverlappingObject(PhysicsObject* obj);
};
