/*
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : main.cpp
Description : Main file for a physics simulation using SFML, demonstrating spring-connected objects and user interaction.
Author : Kazuo Reis de Andrade
Mail : kazuo.andrade@mds.ac.nz
*/
 
#include <SFML/Graphics.hpp>
#include <vector>
#include "PhysicsObject.h"
#include "Spring.h"

void CreateSpringRope(std::vector<PhysicsObject*>& _physicsObjects, std::vector<Spring*>& _springs)
{
    for (int i = 0; i < 10; i++)
    {
        float x = i * 100.0f + 100.0f;
        float y = i % 2 == 0 ? 10.0f : 100.0f;
        //float y = 10.0f;
        sf::Vector2f position = sf::Vector2f(x, y);
        PhysicsObject* CurrentObject = new PhysicsObject(position, 1.0f);

        if (i > 0)
        {
            Spring* newSpring = new Spring(_physicsObjects[_physicsObjects.size() - 1], CurrentObject);
            _springs.push_back(newSpring);
        }

        _physicsObjects.push_back(CurrentObject);
    }
}

int main()
{
    //Create the window with a set resolution:
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Project");
    window.setFramerateLimit(60);

    sf::CircleShape CircleShape(50.0f);
    CircleShape.setFillColor(sf::Color::Green);

    sf::RectangleShape RectangleShape(sf::Vector2f(1.0f, 1.0f));
    RectangleShape.setFillColor(sf::Color::Red);

    std::vector<PhysicsObject*> Objects;
    std::vector<Spring*> Springs;
    CreateSpringRope(Objects, Springs);

    bool bApplyWind = false;
    bool bMovingRope = false;
    while (window.isOpen())
    {
        //Receive and deal with events here (mouse clicks, key events, window buttons etc).
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    bMovingRope = true;
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    bApplyWind = true;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    bMovingRope = false;
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {

                    bApplyWind = false;
                }
                break;
            }
        }

        // Calculate elapsed time since last frame
    

        for (auto Spring : Springs)
        {
            Spring->Simulate();
        }
        for (auto Object : Objects)
        {
            Object->AddForce(g_Gravity * Object->GetMass());
            if (bApplyWind)
            {
                Object->AddForce(sf::Vector2f(1.0f, 0.0f));
            }

            Object->UpdatePhysics();
            Object->CollideObject(window);
        }
        

        if (bMovingRope)
        {
            Objects[0]->SetPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
            Objects[0]->SetVelocity(sf::Vector2f(0.0f, 0.0f));
        }

        window.clear();
        //Do all your drawing in here/////
        for (auto Spring : Springs)
        {
            Spring->Draw(RectangleShape, window);
        }

        for (auto Object : Objects)
        {
            CircleShape.setPosition(Object->GetPosition());
            CircleShape.setRadius(Object->GetRadius());
            CircleShape.setOrigin(Object->GetRadius(), Object->GetRadius());
            window.draw(CircleShape);
        }

        //////////////////////////////////
        window.display();
    }

    for (auto iter : Objects)
    {
        delete iter;
    }
    for (auto iter : Springs)
    {
        delete iter;
    }

    return 0;
}