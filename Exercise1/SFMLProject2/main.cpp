#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

struct Point {
    sf::Vertex vert;
    bool placed = false;
};

sf::Vector2f Normalize(sf::Vector2f _vector) {
    float Magnitude = sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
    return sf::Vector2f(_vector.x / Magnitude, _vector.y / Magnitude);
}

class Plane {
public:
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

class Triangle {
public:
    Triangle() : bTrianglePlaced(false) {
        for (int i = 0; i < 3; ++i) {
            points[i].placed = false;
        }
    }

    void placeTriangle(sf::RenderWindow& window, sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            for (int i = 0; i < 3; ++i) {
                if (!points[i].placed) {
                    points[i].vert.position.x = sf::Mouse::getPosition(window).x;
                    points[i].vert.position.y = sf::Mouse::getPosition(window).y;
                    points[i].placed = true;
                    break;
                }
            }
            bTrianglePlaced = points[0].placed && points[1].placed && points[2].placed;
        }
    }

    Point points[3];
    bool bTrianglePlaced;
};

float DotProduct(sf::Vector2f _vector1, sf::Vector2f _vector2) {
    return (_vector1.x * _vector2.x) + (_vector1.y * _vector2.y);
}


bool TriangleIntersectsPlane(Triangle triangle, Plane plane) {
    int positive = 0, negative = 0;
    for (int i = 0; i < 3; ++i) {
        float dot = DotProduct(triangle.points[i].vert.position - plane.pointOnPlane, plane.normal);
        if (dot > 0) positive++;
        else if (dot < 0) negative++;
    }
    return positive > 0 && negative > 0;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Project");

 
    Plane CurrentPlane;
    Triangle CurrentTriangle;

    bool bCollisionChecked = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
              
                if (!CurrentPlane.bPointPlaced || !CurrentPlane.bPlanePlaced) {
                    CurrentPlane.placePlane(window, event);
                }
                else if (!CurrentTriangle.bTrianglePlaced) {
                    CurrentTriangle.placeTriangle(window, event);
                    if (CurrentTriangle.bTrianglePlaced) {
                        bCollisionChecked = true;
                        bool collision = TriangleIntersectsPlane(CurrentTriangle, CurrentPlane);
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

 
        window.clear();

  
        if (CurrentPlane.bPointPlaced && !CurrentPlane.bPlanePlaced) {
            sf::Vertex PlaneToDraw[] = {
                sf::Vertex(CurrentPlane.pointOnPlane, sf::Color::Red),
                sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window)), sf::Color::Red)
            };
            window.draw(PlaneToDraw, 2, sf::Lines);
        }

        if (CurrentTriangle.bTrianglePlaced) {
            sf::Vertex TriangleToDraw[] = {
                CurrentTriangle.points[0].vert,
                CurrentTriangle.points[1].vert,
                CurrentTriangle.points[2].vert,
                CurrentTriangle.points[0].vert
            };
            window.draw(TriangleToDraw, 4, sf::LinesStrip);
        }
        CurrentPlane.drawPlane(window);
        window.display();
    }

    return 0;
}
