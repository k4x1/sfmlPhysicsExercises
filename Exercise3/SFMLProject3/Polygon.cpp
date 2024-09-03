#include "Polygon.h"
#include <cmath>

Polygon::Polygon(const std::vector<sf::Vector2f>& verts, const sf::Vector2f& vel, const sf::Vector2f& accel, float m)
    : vertices(verts), velocity(vel), acceleration(accel), mass(m), angularVelocity(0), angularAcceleration(0) {
    // Calculate moment of inertia (simplified for convex polygons)
    momentOfInertia = 0.0f;
    for (const auto& vertex : vertices) {
        momentOfInertia += mass * (vertex.x * vertex.x + vertex.y * vertex.y);
    }
}

void Polygon::ApplyImpulse(const sf::Vector2f& impulse, const sf::Vector2f& contactPoint) {
    velocity += impulse / mass;
    sf::Vector2f r = contactPoint - GetCenter();
    angularVelocity += DotProduct(r, sf::Vector2f(-impulse.y, impulse.x)) / momentOfInertia;
}

void Polygon::Update(float deltaTime) {
    velocity += acceleration * deltaTime;
    angularVelocity += angularAcceleration * deltaTime;

    for (auto& vertex : vertices) {
        vertex += velocity * deltaTime;
        sf::Vector2f center = GetCenter();
        float angle = angularVelocity * deltaTime;
        float s = std::sin(angle);
        float c = std::cos(angle);
        vertex -= center;
        float xnew = vertex.x * c - vertex.y * s;
        float ynew = vertex.x * s + vertex.y * c;
        vertex.x = xnew + center.x;
        vertex.y = ynew + center.y;
    }
}

void Polygon::SetPosition(const sf::Vector2f& position) {
    sf::Vector2f offset = position - vertices[0];
    for (auto& vertex : vertices) {
        vertex += offset;
    }
}

sf::Vector2f Polygon::GetCenter() const {
    sf::Vector2f center(0, 0);
    for (const auto& vertex : vertices) {
        center += vertex;
    }
    return center / static_cast<float>(vertices.size());
}

void Polygon::ProjectPolygon(const Polygon& polygon, const sf::Vector2f& axis, float& min, float& max)
{
    min = DotProduct(polygon.vertices[0], axis);
    max = min;
    for (const auto& vertex : polygon.vertices) {
        float projection = DotProduct(vertex, axis);
        if (projection < min) {
            min = projection;
        }
        if (projection > max) {
            max = projection;
        }
    }
}

sf::Vector2f Polygon::CalculateImpulse(const Polygon& polygonA, const Polygon& polygonB, const sf::Vector2f& collisionNormal, const sf::Vector2f& contactPoint)
{
    sf::Vector2f rA = contactPoint - polygonA.GetCenter();
    sf::Vector2f rB = contactPoint - polygonB.GetCenter();

    sf::Vector2f relativeVelocity = (polygonB.velocity + sf::Vector2f(-polygonB.angularVelocity * rB.y, polygonB.angularVelocity * rB.x)) -
        (polygonA.velocity + sf::Vector2f(-polygonA.angularVelocity * rA.y, polygonA.angularVelocity * rA.x));
    float velocityAlongNormal = DotProduct(relativeVelocity, collisionNormal);

    if (velocityAlongNormal > 0) {
        return sf::Vector2f(0, 0); // Polygons are separating
    }

    float restitution = 1.0f; // Perfectly elastic collision
    float rA_cross_N = rA.x * collisionNormal.y - rA.y * collisionNormal.x;
    float rB_cross_N = rB.x * collisionNormal.y - rB.y * collisionNormal.x;
    float invMassSum = (1 / polygonA.mass) + (1 / polygonB.mass) + (rA_cross_N * rA_cross_N) / polygonA.momentOfInertia + (rB_cross_N * rB_cross_N) / polygonB.momentOfInertia;

    float impulseMagnitude = -(1 + restitution) * velocityAlongNormal / invMassSum;

    return impulseMagnitude * collisionNormal;
}

bool Polygon::Overlap(float minA, float maxA, float minB, float maxB)
{
    return !(minA > maxB || minB > maxA);
}

bool Polygon::SeparatingAxisTheorem(const Polygon& polygonA, const Polygon& polygonB)
{
    std::vector<sf::Vector2f> axes;

    // Get the axes from polygonA
    for (size_t i = 0; i < polygonA.vertices.size(); ++i) {
        sf::Vector2f p1 = polygonA.vertices[i];
        sf::Vector2f p2 = polygonA.vertices[(i + 1) % polygonA.vertices.size()];
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f axis = Normalize(sf::Vector2f(-edge.y, edge.x));
        axes.push_back(axis);
    }

    // Get the axes from polygonB
    for (size_t i = 0; i < polygonB.vertices.size(); ++i) {
        sf::Vector2f p1 = polygonB.vertices[i];
        sf::Vector2f p2 = polygonB.vertices[(i + 1) % polygonB.vertices.size()];
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f axis = Normalize(sf::Vector2f(-edge.y, edge.x));
        axes.push_back(axis);
    }

    // Check for overlap on all axes
    for (const auto& axis : axes) {
        float minA, maxA, minB, maxB;
        ProjectPolygon(polygonA, axis, minA, maxA);
        ProjectPolygon(polygonB, axis, minB, maxB);
        if (!Overlap(minA, maxA, minB, maxB)) {
            return false; // No collision
        }
    }

    return true; // Collision
}
