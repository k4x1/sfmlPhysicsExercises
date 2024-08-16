#include "Liquid.h"

Liquid::Liquid(sf::Vector2f _position, sf::Vector2f _size, float _dragCoefficient){
    position = _position;
    size = _size;
    dragCoefficient = _dragCoefficient;
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(0, 0, 255, 100)); 
}

bool Liquid::Contains(sf::Vector2f _point) {
    return (_point.x > position.x && _point.x < position.x + size.x &&
        _point.y > position.y && _point.y < position.y + size.y);
}

void Liquid::Render(sf::RenderWindow& _window) {
    _window.draw(shape);
}
