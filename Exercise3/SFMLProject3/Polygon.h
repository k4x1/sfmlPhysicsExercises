#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include "PhysicsLibrary.h"
struct Polygon {
    std::vector<sf::Vector2f> vertices;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float mass;
    float angularVelocity;
    float angularAcceleration;
    float momentOfInertia;

    Polygon(const std::vector<sf::Vector2f>& verts, const sf::Vector2f& vel, const sf::Vector2f& accel, float m);

    void ApplyImpulse(const sf::Vector2f& impulse, const sf::Vector2f& contactPoint);
    void Update(float deltaTime);
    void SetPosition(const sf::Vector2f& position);
    sf::Vector2f GetCenter() const;
    static void ProjectPolygon(const Polygon& polygon, const sf::Vector2f& axis, float& min, float& max);
    static sf::Vector2f CalculateImpulse(const Polygon& polygonA, const Polygon& polygonB, const sf::Vector2f& collisionNormal, const sf::Vector2f& contactPoint);
    static bool Overlap(float minA, float maxA, float minB, float maxB);
    static bool SeparatingAxisTheorem(const Polygon& polygonA, const Polygon& polygonB);
};

