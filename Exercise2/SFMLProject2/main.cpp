#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "PhysicsObject.h"
#include "PhysicsLibrary.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Project");
    window.setFramerateLimit(60);
    sf::CircleShape CircleShape(50.0f);
    CircleShape.setFillColor(sf::Color::Green);

    std::vector<PhysicsObject*> PhysicsObjects;

    PhysicsObject* CurrentObject = new PhysicsObject(sf::Vector2f(640,360),1);
    bool bApplyingWind = false;
    bool bCreatingObject = false;
    while (window.isOpen()) 
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    CurrentObject = new PhysicsObject((sf::Vector2f)sf::Mouse::getPosition(window), 1.0f);
                    PhysicsObjects.push_back(CurrentObject);
                    bCreatingObject = true;
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    bApplyingWind = true;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    bApplyingWind = false;
                }  
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    bCreatingObject = false;
                }
                break;
            }
      
        }
  
        if (bCreatingObject) {
            CurrentObject->SetMass(CurrentObject->GetMass() + 2.0f / 60);
        }

        window.clear();
        for (auto obj : PhysicsObjects) {
            obj->ApplyForce(g_Gravity * obj->GetMass());
            if (bApplyingWind) {
                obj->ApplyForce(sf::Vector2f(10.0f, 0));
            }
            obj->UpdatePhysics(window);
            obj->CollideObject(window);
            CircleShape.setPosition(obj->GetPosition());
            CircleShape.setRadius(obj->GetRadius());
            CircleShape.setOrigin(obj->GetRadius(), obj->GetRadius());
            window.draw(CircleShape);
        }
    
      

        window.display();
    }

    for (auto iter : PhysicsObjects) {
        delete iter;
    }
    
    return 0;
}
