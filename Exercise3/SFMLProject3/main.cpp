#include <SFML/Graphics.hpp>
#include "physicslibrary.h"
#include <vector>
#include <iostream>
#include "Polygon.h"


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
        if (Polygon::SeparatingAxisTheorem(polygonA, polygonB)) {
            std::cout << "Collision detected!" << std::endl;

            // Calculate collision normal
            sf::Vector2f collisionNormal = Normalize(polygonB.vertices[0] - polygonA.vertices[0]);

            // Calculate and apply impulse
            sf::Vector2f contactPoint = (polygonA.GetCenter() + polygonB.GetCenter()) / 2.0f;
            sf::Vector2f impulse = Polygon::CalculateImpulse(polygonA, polygonB, collisionNormal, contactPoint);
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
