#pragma once

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <math.h>

const inline sf::Vector2f g_Gravity = { 0.0f, 9.81f };
const inline float g_MoveScale = 50.0f;
const inline float g_MassSizeScale = 20.0f;
const inline float g_FrictionCoefficient = 50.0f;
const inline float g_DragCoefficient = 50.0f;
inline float GetVectorLength(sf::Vector2f _vector)
{
	return sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
}

inline sf::Vector2f Normalize(sf::Vector2f _vector)
{
	float Magnitude = GetVectorLength(_vector);
	return _vector / Magnitude;
}
inline float DotProduct(sf::Vector2f _a, sf::Vector2f _b)
{
	return (_a.x * _b.x) + (_a.y * _b.y);
}
inline sf::Vector2f NormalizeVector(sf::Vector2f _vector)
{
	float length = GetVectorLength(_vector);
	if (length != 0)
	{
		return _vector / length;
	}
	else
	{
		return sf::Vector2f(0.0f, 0.0f);
	}
}

inline float RadiansToDegrees(float _radians)
{
	return _radians * 180.0f / M_PI;
}
inline float GetAngle(sf::Vector2f _vector)
{
	return RadiansToDegrees(std::atan2(_vector.y, _vector.x));
}
inline sf::Vector2f VectorToLine(sf::Vector2f _point, sf::Vector2f _lineStart, sf::Vector2f _lineEnd)
{
	sf::Vector2f line = _lineEnd - _lineStart;
	float lineLength = GetVectorLength(line);
	sf::Vector2f N = Normalize(line);
	float t = DotProduct(_point - _lineStart, N);

	if (t < 0)
	{
		return _lineStart - _point;
	}
	else if (t > lineLength)
	{
		return _lineEnd - _point;
	}
	return (_lineStart + N * t) - _point;
}