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
    sf::CircleShape circleShape(50.0f);
    circleShape.setFillColor(sf::Color::Green);

    sf::RectangleShape rectangleShape(sf::Vector2f(200.0f,50.0f));
    rectangleShape.setFillColor(sf::Color::Green);

    std::vector<PhysicsObject*> PhysicsObjects;

    bool bApplyingWind = false;
    bool bCreatingObject = false;
    PhysicsObject* CurrentObject = nullptr;
    for (int i = 0; i < 10; i++)
    {
        float startX = rand() % (window.getSize().x-400)+200;
        float startY = rand() % (window.getSize().y-400)+200;   
        
        float endX = startX + (rand() % 200) - 100;
        float endY = startY + (rand() % 200) - 100;
        CurrentObject = new PhysicsObject(sf::Vector2f(startX, startY),sf::Vector2f(endX,endY), rand()%4+1);
        PhysicsObjects.push_back(CurrentObject);
    }
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
                    bApplyingWind = true;
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Right)
                {

                }  
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    bApplyingWind = false;
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
            obj->UpdatePhysics();
            obj->CollideObject(window);
            obj->CollideCapsules(PhysicsObjects);
            circleShape.setPosition(obj->GetStartPosition());
            circleShape.setRadius(obj->GetRadius());
            circleShape.setOrigin(obj->GetRadius(), obj->GetRadius());
            window.draw(circleShape);

            circleShape.setPosition(obj->GetEndPosition());
            window.draw(circleShape);

            rectangleShape.setSize(sf::Vector2f(obj->GetLength(), obj->GetRadius() * 2));
            rectangleShape.setOrigin(0.0f,obj->GetRadius());
            rectangleShape.setRotation(GetAngle(obj->GetEndPosition()-obj->GetStartPosition()));
            rectangleShape.setPosition(obj->GetStartPosition());
            window.draw(rectangleShape);
        }
        
      

        window.display();
    }

    for (auto iter : PhysicsObjects) {
        delete iter;
    }
    
    return 0;
}
