#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

struct Point {
    sf::Vertex vert;
    bool placed = false;
};

float DotProduct(sf::Vector2f _vector1, sf::Vector2f _vector2) {
    return (_vector1.x * _vector2.x) + (_vector1.y * _vector2.y);
}

sf::Vector2f Normalize(sf::Vector2f _vector) {
    float Magnitude = sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
    return sf::Vector2f(_vector.x / Magnitude, _vector.y / Magnitude);
}

sf::Vector2f LinePlaneIntersection(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f planePoint, sf::Vector2f planeNormal) {
    sf::Vector2f lineDir = p2 - p1;
    float t = DotProduct(planeNormal, planePoint - p1) / DotProduct(planeNormal, lineDir);
    return p1 + lineDir * t;
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

    void reset() {
        pointOnPlane = sf::Vector2f();
        normal = sf::Vector2f();
        bPointPlaced = false;
        bPlanePlaced = false;
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

    void drawPoints(sf::RenderWindow& window) {
        for (int i = 0; i < 3; ++i) {
            if (points[i].placed) {
                sf::CircleShape circle(5);
                circle.setFillColor(sf::Color::Green);
                circle.setPosition(points[i].vert.position - sf::Vector2f(5, 5)); 
                window.draw(circle);
            }
        }
    }

    void reset() {
        for (int i = 0; i < 3; ++i) {
            points[i].placed = false;
        }
        bTrianglePlaced = false;
    }

    Point points[3];
    bool bTrianglePlaced;
};

bool TriangleIntersectsPlane(Triangle triangle, Plane plane, std::vector<sf::Vector2f>& intersectionPoints) {
    int positive = 0, negative = 0;
    for (int i = 0; i < 3; ++i) {
        float dot = DotProduct(triangle.points[i].vert.position - plane.pointOnPlane, plane.normal);
        if (dot > 0) {
            positive++;
        }
        else if (dot < 0) {
            negative++;
        }
    }

    if (positive > 0 && negative > 0) {
        for (int i = 0; i < 3; ++i) {
            sf::Vector2f p1 = triangle.points[i].vert.position;
            sf::Vector2f p2 = triangle.points[(i + 1) % 3].vert.position;
            float dot1 = DotProduct(p1 - plane.pointOnPlane, plane.normal);
            float dot2 = DotProduct(p2 - plane.pointOnPlane, plane.normal);

            if (dot1 * dot2 < 0) {
                intersectionPoints.push_back(LinePlaneIntersection(p1, p2, plane.pointOnPlane, plane.normal));
            }
        }
        return true;
    }
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Project");

    Plane CurrentPlane;
    Triangle CurrentTriangle;

    bool bCollisionChecked = false;
    std::vector<sf::Vector2f> intersectionPoints;

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
                        intersectionPoints.clear();
                        bool collision = TriangleIntersectsPlane(CurrentTriangle, CurrentPlane, intersectionPoints);
                        if (collision) {
                            std::cout << "Collision" << std::endl;
                        }
                        else {
                            std::cout << "No Collision" << std::endl;
                        }
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R) {
                    CurrentPlane.reset();
                    CurrentTriangle.reset();
                    bCollisionChecked = false;
                    intersectionPoints.clear();
                    window.clear();
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

        CurrentTriangle.drawPoints(window);
        CurrentPlane.drawPlane(window);

        if (bCollisionChecked && intersectionPoints.size() == 2) {
            sf::Vector2f midpoint = (intersectionPoints[0] + intersectionPoints[1]) / 2.0f;
            sf::CircleShape collisionCircle1(5);
            sf::CircleShape collisionCircle2(5);
            collisionCircle1.setFillColor(sf::Color::Blue);
            collisionCircle1.setPosition(intersectionPoints[0] - sf::Vector2f(5, 5)); 
            collisionCircle2.setPosition(intersectionPoints[1] - sf::Vector2f(5, 5));
            collisionCircle2.setFillColor(sf::Color::Blue);
            window.draw(collisionCircle1);
            window.draw(collisionCircle2);
        }

        window.display();
    }

    return 0;
}
