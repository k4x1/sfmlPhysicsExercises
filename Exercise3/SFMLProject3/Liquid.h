#pragma once
#include <SFML/Graphics.hpp>

class Liquid {
public:
    Liquid(sf::Vector2f _position, sf::Vector2f _size, float _dragCoefficient);

    bool Contains(sf::Vector2f point);
    void Render(sf::RenderWindow& window);
    float dragCoefficient;
private:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::RectangleShape shape;
};
