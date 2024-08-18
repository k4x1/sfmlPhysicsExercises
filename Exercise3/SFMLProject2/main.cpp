#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "PhysicsObject.h"
#include "PhysicsLibrary.h"
#include "Triangle.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Project");
    window.setFramerateLimit(60);

    std::vector<PhysicsObject*> PhysicsObjects;

    // Create a fixed triangle
    Triangle triangle(sf::Vector2f(400, 300), sf::Vector2f(500, 500), sf::Vector2f(300, 500));

    // Create physics objects
    for (int i = 0; i < 10; i++) {
        float startX = rand() % (window.getSize().x - 400) + 200;
        float startY = rand() % (window.getSize().y - 400) + 200;

        float endX = startX + (rand() % 200) - 100;
        float endY = startY + (rand() % 200) - 100;
        PhysicsObject* obj = new PhysicsObject(sf::Vector2f(startX, startY), sf::Vector2f(endX, endY), rand() % 4 + 1);
        PhysicsObjects.push_back(obj);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        for (auto obj : PhysicsObjects) {
            obj->ApplyForce(g_Gravity * obj->GetMass());
            obj->UpdatePhysics();
            obj->CollideCapsules(PhysicsObjects);
            obj->CollideObject(window);

            // Check for collision with the triangle
            if (triangle.ContainsPoint(obj->GetStartPosition()) || triangle.ContainsPoint(obj->GetEndPosition())) {
                sf::Vector2f center = (triangle.p1 + triangle.p2 + triangle.p3) / 3.0f;
                sf::Vector2f force = obj->GetStartPosition() - center;
                obj->ApplyImpulse(Normalize(force) * 10.0f);
            }


            // Draw the object
            sf::CircleShape circleShape(obj->GetRadius());
            circleShape.setFillColor(sf::Color::Green);
            circleShape.setPosition(obj->GetStartPosition());
            circleShape.setOrigin(obj->GetRadius(), obj->GetRadius());
            window.draw(circleShape);

            circleShape.setPosition(obj->GetEndPosition());
            window.draw(circleShape);

            sf::RectangleShape rectangleShape(sf::Vector2f(obj->GetLength(), obj->GetRadius() * 2));
            rectangleShape.setFillColor(sf::Color::Green);
            rectangleShape.setOrigin(0.0f, obj->GetRadius());
            rectangleShape.setRotation(GetAngle(obj->GetEndPosition() - obj->GetStartPosition()));
            rectangleShape.setPosition(obj->GetStartPosition());
            window.draw(rectangleShape);
        }

        // Draw the triangle
        sf::ConvexShape convex;
        convex.setPointCount(3);
        convex.setPoint(0, triangle.p1);
        convex.setPoint(1, triangle.p2);
        convex.setPoint(2, triangle.p3);
        convex.setFillColor(sf::Color::Blue);
        window.draw(convex);

        window.display();
    }

    for (auto iter : PhysicsObjects) {
        delete iter;
    }

    return 0;
}
