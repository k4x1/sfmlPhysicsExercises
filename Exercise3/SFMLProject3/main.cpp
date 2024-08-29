#include <SFML/Graphics.hpp>
#include "physicslibrary.h"
#include <vector>
#include <iostream>
struct Polygon {
    std::vector<sf::Vector2f> vertices;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float mass;
    float angularVelocity;
    float angularAcceleration;
    float momentOfInertia;

    Polygon(const std::vector<sf::Vector2f>& verts, const sf::Vector2f& vel, const sf::Vector2f& accel, float m)
        : vertices(verts), velocity(vel), acceleration(accel), mass(m), angularVelocity(0), angularAcceleration(0) {
        // Calculate moment of inertia (simplified for convex polygons)
        momentOfInertia = 0.0f;
        for (const auto& vertex : vertices) {
            momentOfInertia += mass * (vertex.x * vertex.x + vertex.y * vertex.y);
        }
    }

    void ApplyImpulse(const sf::Vector2f& impulse, const sf::Vector2f& contactPoint) {
        velocity += impulse / mass;
        sf::Vector2f r = contactPoint - GetCenter();
        angularVelocity += DotProduct(r, sf::Vector2f(-impulse.y, impulse.x)) / momentOfInertia;
    }

    void Update(float deltaTime) {
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

    void SetPosition(const sf::Vector2f& position) {
        sf::Vector2f offset = position - vertices[0];
        for (auto& vertex : vertices) {
            vertex += offset;
        }
    }

    sf::Vector2f GetCenter() const {
        sf::Vector2f center(0, 0);
        for (const auto& vertex : vertices) {
            center += vertex;
        }
        return center / static_cast<float>(vertices.size());
    }
};
void ProjectPolygon(const Polygon& polygon, const sf::Vector2f& axis, float& min, float& max) {
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
sf::Vector2f CalculateImpulse(const Polygon& polygonA, const Polygon& polygonB, const sf::Vector2f& collisionNormal, const sf::Vector2f& contactPoint) {
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


bool Overlap(float minA, float maxA, float minB, float maxB) {
    return !(minA > maxB || minB > maxA);
}    
bool SeparatingAxisTheorem(const Polygon& polygonA, const Polygon& polygonB) {
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
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Polygon Collision and Bouncing");

    // Define two polygons with different dimensions
    Polygon polygonA = { { {130, 90}, {210, 100}, {220, 150}, {90, 150} }, {100, 150}, {0, 0}, 2.0f };
    Polygon polygonB = { { {300, 250}, {420, 300}, {430, 350}, {300, 350} }, {-100, -150}, {0, 0},2.0f };

    sf::Clock clock;
    bool isMouseHeld = false;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();
        // Update polygons using Euler integration
        polygonA.Update(deltaTime);
        polygonB.Update(deltaTime);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isMouseHeld = true;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isMouseHeld = false;
                }
            }
        }


        if (isMouseHeld) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            polygonA.SetPosition(sf::Vector2f(mousePosition.x, mousePosition.y));
        }
     
        // Bounce off walls
        for (auto& polygon : { &polygonA, &polygonB }) {
            for (auto& vertex : polygon->vertices) {
                if (vertex.x < 0 || vertex.x > window.getSize().x) {
                    polygon->velocity.x = -polygon->velocity.x;
                    break;
                }
                if (vertex.y < 0 || vertex.y > window.getSize().y) {
                    polygon->velocity.y = -polygon->velocity.y;
                    break;
                }
            }
        }

        // Check for collision
        if (SeparatingAxisTheorem(polygonA, polygonB)) {
            std::cout << "Collision detected!" << std::endl;

            // Calculate collision normal
            sf::Vector2f collisionNormal = Normalize(polygonB.vertices[0] - polygonA.vertices[0]);

            // Calculate and apply impulse
            sf::Vector2f contactPoint = (polygonA.GetCenter() + polygonB.GetCenter()) / 2.0f;
            sf::Vector2f impulse = CalculateImpulse(polygonA, polygonB, collisionNormal, contactPoint);
            polygonA.ApplyImpulse(-impulse, contactPoint);
            polygonB.ApplyImpulse(impulse, contactPoint);

            // Move polygons apart to prevent overlap
            float penetrationDepth = 1.0f; // Adjust this value as needed
            sf::Vector2f correction = collisionNormal * penetrationDepth / ((1 / polygonA.mass) + (1 / polygonB.mass));
            for (auto& vertex : polygonA.vertices) {
                vertex -= correction * (1 / polygonA.mass);
            }
            for (auto& vertex : polygonB.vertices) {
                vertex += correction * (1 / polygonB.mass);
            }
        }

        window.clear(sf::Color::Black);

        // Draw polygons
        sf::ConvexShape shapeA;
        shapeA.setPointCount(polygonA.vertices.size());
        for (size_t i = 0; i < polygonA.vertices.size(); ++i) {
            shapeA.setPoint(i, polygonA.vertices[i]);
        }
        shapeA.setFillColor(sf::Color::Green);
        window.draw(shapeA);

        sf::ConvexShape shapeB;
        shapeB.setPointCount(polygonB.vertices.size());
        for (size_t i = 0; i < polygonB.vertices.size(); ++i) {
            shapeB.setPoint(i, polygonB.vertices[i]);
        }
        shapeB.setFillColor(sf::Color::Blue);
        window.draw(shapeB);

        window.display();
    }

    return 0;
}
