#pragma once
#include <SFML/Graphics.hpp>
const inline sf::Vector2f g_Gravity = { 0.0f, 9.81f };
const inline float g_MoveScale = 50.0f;
const inline float g_FrictionCoefficient = 0.28f;
const inline float g_MassSizeScale = 20.0f;
inline float GetVectorLength( sf::Vector2f _vector) {
	return sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
}
inline sf::Vector2f Normalize( sf::Vector2f vector) {
    float Magnitude = GetVectorLength(vector);
    if (Magnitude != 0) {
        return vector / Magnitude;
    }
    return sf::Vector2f(0, 0); 
}
