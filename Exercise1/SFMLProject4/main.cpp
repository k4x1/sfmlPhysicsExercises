#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>



sf::Vector2f Normalize(sf::Vector2f _vector) {
    float Magnitude = sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
    return sf::Vector2f(_vector.x / Magnitude, _vector.y / Magnitude);
}
struct Point {
    sf::Vertex vert;
    bool placed = false;
};

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
    sf::Color GenerateRandomColor() {
        return sf::Color(rand() % 256, rand() % 256, rand() % 256);
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

float DotProduct(sf::Vector2f _vector1, sf::Vector2f _vector2) {
    return (_vector1.x * _vector2.x) + (_vector1.y * _vector2.y);
}


enum EPlaneResult {
    ON_PLANE,
    IN_FRONT,
    BEHIND
};
EPlaneResult PlaneEquation(Plane _plane, sf::Vector2f _pointToCheck) {
    float fDistance = DotProduct(_plane.normal, _plane.pointOnPlane);
    float fDot = DotProduct(_plane.normal, _pointToCheck) - fDistance;
    if (fDot == 0) return EPlaneResult::ON_PLANE;
    if (fDot < 0) return EPlaneResult::BEHIND;

    return EPlaneResult::IN_FRONT;

}


sf::Vector2f LinePlaneIntersection(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f planePoint, sf::Vector2f planeNormal) {
    sf::Vector2f lineDir = p2 - p1;
    float t = DotProduct(planeNormal, planePoint - p1) / DotProduct(planeNormal, lineDir);
    return p1 + lineDir * t;
}


class Triangle {
public:
    Triangle() : bTrianglePlaced(false) {
        for (int i = 0; i < 3; ++i) {
            points[i].placed = false;
        }
        sf::Color newRandomColor(
            std::rand() % 256, // Red
            std::rand() % 256, // Green
            std::rand() % 256  // Blue
        );
        randomColor = newRandomColor;
    }
    bool areSameObject(const Triangle& a, const Triangle& b) {
        return &a == &b;
    }
    sf::Color randomColor;

    void placeTriangle(sf::RenderWindow& window, sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            for (int i = 0; i < 3; ++i) {
                if (!points[i].placed) {
                    points[i].vert.position.x = sf::Mouse::getPosition(window).x;
                    points[i].vert.position.y = sf::Mouse::getPosition(window).y;
                    points[i].placed = true;
                    std::cout << points[i].vert.position.x << "|" << points[i].vert.position.y << std::endl;
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

    void DrawTriangle(sf::RenderWindow& window) {
        if (bTrianglePlaced) {
            sf::VertexArray triangle(sf::Triangles, 3);
            for (int i = 0; i < 3; ++i) {
                triangle[i].position = points[i].vert.position;
                triangle[i].color = randomColor;
            }
            window.draw(triangle);
        }
    }

    void sliceTriangle(std::vector<Triangle>& _triangleArray, Plane& _plane) {
        if(TriangleIntersectsPlane(_plane)){
            Triangle newTriangle1;
            std::cout << intersectionPoints.size() << std::endl;
            std::vector<sf::Vector2f> base;

            for (int i = 0; i < 3; i++) {
                if (!newTriangle1.points[i].placed) {
                    newTriangle1.points[i].placed = true;
                    if (i == 2) {
                        std::vector<sf::Vector2f> inFront;
                        std::vector<sf::Vector2f> behind;
                        for (auto& point : points) {
                            switch (PlaneEquation(_plane, point.vert.position)) {
                            case IN_FRONT:
                                inFront.push_back(point.vert.position);
                                break;
                            case BEHIND:
                                behind.push_back(point.vert.position);
                                break;
                            default:
                                std::cout << "err" << std::endl;
                                break;
                            }
                        }
                        newTriangle1.points[i].vert.position = inFront.size() < behind.size() ? inFront[0] : behind[0];
                        base = inFront.size() > behind.size() ? inFront : behind;
                        std::cout << "newTriangle1 point " << i << ": " << newTriangle1.points[i].vert.position.x << "|" << newTriangle1.points[i].vert.position.y << std::endl;
                        continue;
                    }
                    newTriangle1.points[i].vert.position = intersectionPoints[i];
                    std::cout << "newTriangle1 point " << i << ": " << newTriangle1.points[i].vert.position.x << "|" << newTriangle1.points[i].vert.position.y << std::endl;
                }
            }
         
            newTriangle1.bTrianglePlaced = newTriangle1.points[0].placed && newTriangle1.points[1].placed && newTriangle1.points[2].placed;
       
            std::cout << "newTriangle1 placed: " << newTriangle1.points[0].placed << " | " << newTriangle1.points[1].placed << " | " << newTriangle1.points[2].placed << std::endl;
        
            if (base.size() < 2) {
                std::cerr << "Error: base does not have enough points." << std::endl;
                return;
            }
      
            Triangle newTriangle2;
            Triangle newTriangle3;
 
            newTriangle2.points[0].vert.position = intersectionPoints[0];
            newTriangle2.points[1].vert.position = intersectionPoints[1];
            newTriangle2.points[2].vert.position = base[1];

            newTriangle2.points[0].placed = true;
            newTriangle2.points[1].placed = true;
            newTriangle2.points[2].placed = true;
            newTriangle2.bTrianglePlaced = true;
           
       
            std::cout << "newTriangle2 points: " << newTriangle2.points[0].vert.position.x << "|" << newTriangle2.points[0].vert.position.y << " / "
                << newTriangle2.points[1].vert.position.x << "|" << newTriangle2.points[1].vert.position.y << "/ "
                << newTriangle2.points[2].vert.position.x << "|" << newTriangle2.points[2].vert.position.y << std::endl;

            newTriangle3.points[0].vert.position = base[0];
            newTriangle3.points[1].vert.position = base[1];
            newTriangle3.points[2].vert.position = intersectionPoints[0];

            newTriangle3.points[0].placed = true;
            newTriangle3.points[1].placed = true;
            newTriangle3.points[2].placed = true;
            newTriangle3.bTrianglePlaced = true;
            _triangleArray.push_back(newTriangle3);
            _triangleArray.push_back(newTriangle2);
            _triangleArray.push_back(newTriangle1);
            std::cout << "newTriangle3 points: " << newTriangle3.points[0].vert.position.x << "|" << newTriangle3.points[0].vert.position.y << " / "
                << newTriangle3.points[1].vert.position.x << "|" << newTriangle3.points[1].vert.position.y << " / "
                << newTriangle3.points[2].vert.position.x << "|" << newTriangle3.points[2].vert.position.y << std::endl;

    /*        std::cout << "Total triangles: " << _triangleArray.size() << std::endl;
                 auto it = std::remove_if(_triangleArray.begin(), _triangleArray.end(), [this](const Triangle& t) {
                     return &t == this;
                     });
                 if (it != _triangleArray.end()) {
                     _triangleArray.erase(it, _triangleArray.end());
                     std::cout << "newTriangle1 deleted from the vector." << std::endl;
                 }
                 else {
                     std::cout << "newTriangle1 not found in the vector." << std::endl;
                 }*/
                 bTrianglePlaced = false;
                 
        }

    }


    void reset() {
        for (int i = 0; i < 3; ++i) {
            points[i].placed = false;
        }
        bTrianglePlaced = false;
    }

    bool TriangleIntersectsPlane(Plane plane) {
        int positive = 0, negative = 0;
        for (int i = 0; i < 3; ++i) {
            float dot = DotProduct(points[i].vert.position - plane.pointOnPlane, plane.normal);
            if (dot > 0) {
                positive++;
            }
            else if (dot < 0) {
                negative++;
            }
        }

        if (positive > 0 && negative > 0) {
            for (int i = 0; i < 3; ++i) {
                sf::Vector2f p1 = points[i].vert.position;
                sf::Vector2f p2 = points[(i + 1) % 3].vert.position;
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
    bool sliced = false;
    Point points[3];
    bool bTrianglePlaced;
    std::vector<sf::Vector2f> intersectionPoints;

};



int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Project");

    Plane CurrentPlane;
    std::vector<Plane> PlaneVector;
    Triangle firstTriangle;

    bool bCollisionChecked = false;

    std::vector<Triangle> Triangles; // Vector to store resulting triangles after slicing
    Triangles.push_back(firstTriangle);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (!Triangles[0].bTrianglePlaced) {
                    Triangles[0].placeTriangle(window, event);
                }
                else if (!CurrentPlane.bPointPlaced || !CurrentPlane.bPlanePlaced) {
                    CurrentPlane.placePlane(window, event);
                    if (Triangles[0].bTrianglePlaced) {
                        bCollisionChecked = true;
                        int triSize = Triangles.size();
                        for (int i = 0; i < triSize; i++) {
                       
                                Triangles[i].sliceTriangle(Triangles, CurrentPlane);
                            
                        }
                    }
                }
                else {
                    if (Triangles[0].bTrianglePlaced) {
                        bCollisionChecked = true;

                      
                            Triangles[0].sliceTriangle(Triangles, CurrentPlane);
                
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R) {
                    CurrentPlane.reset();

                    bCollisionChecked = false;
                    // intersectionPoints.clear();
                    Triangles.clear(); // Clear the sliced triangles
                    window.clear();
                }
                break;
            }
        }
        if (CurrentPlane.bPlanePlaced) {
            PlaneVector.push_back(CurrentPlane);
            Plane newPlane;
            CurrentPlane = newPlane;
        }
        window.clear();

        if (CurrentPlane.bPointPlaced && !CurrentPlane.bPlanePlaced) {
            sf::Vertex PlaneToDraw[] = {
                sf::Vertex(CurrentPlane.pointOnPlane, sf::Color::Red),
                sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window)), sf::Color::Red)
            };
            window.draw(PlaneToDraw, 2, sf::Lines);
        }
        if (Triangles[0].bTrianglePlaced) {
            sf::Vertex TriangleToDraw[] = {
                Triangles[0].points[0].vert,
                Triangles[0].points[1].vert,
                Triangles[0].points[2].vert,
                Triangles[0].points[0].vert

            };
            window.draw(TriangleToDraw, 4, sf::LinesStrip);
        }
        Triangles[0].drawPoints(window);
        for (int i = 0; i < PlaneVector.size(); i++) {
            PlaneVector[i].drawPlane(window);
        }
        CurrentPlane.drawPlane(window);

        // Draw the resulting sliced triangles
        for (auto& triangle : Triangles) {


            triangle.DrawTriangle(window);
            triangle.drawPoints(window);
        }

        window.display();
    }

    return 0;
}
