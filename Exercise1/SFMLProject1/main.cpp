
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

sf::Vector2f Normalize(sf::Vector2f _vector) {
    float Magnitude = sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
    return sf::Vector2f(_vector.x / Magnitude, _vector.y / Magnitude);
}

struct Plane {
    Plane() : pointOnPlane(), normal(), bPointPlaced(false), bPlanePlaced(false) {}

    Plane(sf::Vector2f _pointOnPlane, sf::Vector2f _normal)
        : pointOnPlane(_pointOnPlane), normal(_normal), bPointPlaced(false), bPlanePlaced(false) {}

    void placePlane(sf::RenderWindow& window, sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (!bPointPlaced) {
                pointOnPlane.x = sf::Mouse::getPosition(window).x;
                pointOnPlane.y = sf::Mouse::getPosition(window).y;
                bPointPlaced = true;
            }
            else if (!bPlanePlaced) {
                sf::Vector2f planeDirection = sf::Vector2f(sf::Mouse::getPosition(window)) - pointOnPlane;
                sf::Vector2f planeNormal(-planeDirection.y, planeDirection.x);
                normal = Normalize(planeNormal);
                bPlanePlaced = true;
            }
        }
    }
    void drawPlane(sf::RenderWindow& window) {
        if (bPointPlaced && bPlanePlaced) {
            float planeLength = 1000.0f; // length of the plane 
            sf::Vector2f planeDirection(-normal.y, normal.x); // perpendicular to the normal
            sf::Vertex planeLine[] = {
                sf::Vertex(pointOnPlane + planeDirection * planeLength, sf::Color::Red),
                sf::Vertex(pointOnPlane - planeDirection * planeLength, sf::Color::Red)
            };
            window.draw(planeLine, 2, sf::Lines);
        }
    }

    sf::Vector2f pointOnPlane;
    sf::Vector2f normal;
    bool bPointPlaced;
    bool bPlanePlaced;
};

struct LineSegment {
    LineSegment() : start(), end(), bStartPlaced(false), bEndPlaced(false) {}

    LineSegment(sf::Vertex _start, sf::Vertex _end)
        : start(_start), end(_end), bStartPlaced(false), bEndPlaced(false) {}

    void placeLine(sf::RenderWindow& window, sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (!bStartPlaced) {
                start.position.x = sf::Mouse::getPosition(window).x;
                start.position.y = sf::Mouse::getPosition(window).y;
                bStartPlaced = true;
            }
            else if (!bEndPlaced) {
                end.position.x = sf::Mouse::getPosition(window).x;
                end.position.y = sf::Mouse::getPosition(window).y;
                bEndPlaced = true;
            }
        }
    }

    sf::Vertex start;
    sf::Vertex end;
    bool bStartPlaced;
    bool bEndPlaced;
};

float DotProduct(sf::Vector2f _vector1, sf::Vector2f _vector2) {
    return (_vector1.x * _vector2.x) + (_vector1.y * _vector2.y);
}

bool LineSegmentIntersectsPlane(LineSegment line, Plane plane) {
    sf::Vector2f lineDir = line.end.position - line.start.position;
    float dotStart = DotProduct(line.start.position - plane.pointOnPlane, plane.normal);
    float dotEnd = DotProduct(line.end.position - plane.pointOnPlane, plane.normal);

    // If the signs of the dot products are different, the line intersects the plane
    return dotStart * dotEnd <= 0;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Project");

    LineSegment CurrentLine;
    Plane CurrentPlane;

    bool bCollisionChecked = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (!CurrentLine.bStartPlaced || !CurrentLine.bEndPlaced) {
                    CurrentLine.placeLine(window, event);
                }
                else if (!CurrentPlane.bPointPlaced || !CurrentPlane.bPlanePlaced) {
                    CurrentPlane.placePlane(window, event);
                    if (CurrentPlane.bPlanePlaced) {
                        // Check for collision
                        bCollisionChecked = true;
                        bool collision = LineSegmentIntersectsPlane(CurrentLine, CurrentPlane);
                        if (collision) {
                            std::cout << "Collision" << std::endl;
                        }
                        else {
                            std::cout << "No Collision" << std::endl;
                        }
                    }
                }
                break;
            }
        }

        if (CurrentLine.bStartPlaced && !CurrentLine.bEndPlaced) {
            CurrentLine.end.position.x = sf::Mouse::getPosition(window).x;
            CurrentLine.end.position.y = sf::Mouse::getPosition(window).y;
        }

        window.clear();

        if (CurrentLine.bStartPlaced) {
            sf::Vertex LineToDraw[] = { CurrentLine.start, CurrentLine.end };
            window.draw(LineToDraw, 2, sf::Lines);
        }

        if (CurrentPlane.bPointPlaced && !CurrentPlane.bPlanePlaced) {
            sf::Vertex PlaneToDraw[] = {
                sf::Vertex(CurrentPlane.pointOnPlane, sf::Color::Red),
                sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window)), sf::Color::Red)
            };
            window.draw(PlaneToDraw, 2, sf::Lines);
        }
        CurrentPlane.drawPlane(window);
        window.display();
    }

    return 0;
}
