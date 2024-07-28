#include <SFML/Graphics.hpp>
#include <iostream>

/**/
enum EPlaneResult {
    ON_PLANE,
    IN_FRONT,
    BEHIND
};
sf::Vector2f Normalize(sf::Vector2f _vector) {
    float Magnitude = sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
    return _vector /= Magnitude;
}
struct Plane
{
    Plane() {
        pointOnPlane = sf::Vector2f();
        normal = sf::Vector2f();
    }
    Plane(sf::Vector2f _pointOnPlane, sf::Vector2f _normal) {
        pointOnPlane = _pointOnPlane;
        normal = _normal;
    }
    sf::Vector2f pointOnPlane;
    sf::Vector2f normal;
};
struct LineSegment
{
    LineSegment() {
        start = sf::Vertex();
        end = sf::Vertex();
    }
    LineSegment(sf::Vertex _Start, sf::Vertex _end) {
        start = _Start;
        end = _end;
    }
    sf::Vertex start;
    sf::Vertex end;
};
float DotProduct(sf::Vector2f _vector1, sf::Vector2f _vector2) {
    return (_vector1.x * _vector2.x) + (_vector1.y * _vector2.y);
}
EPlaneResult PlaneEquation(Plane _plane, sf::Vector2f _pointToCheck) {
    float fDistance = DotProduct(_plane.normal, _plane.pointOnPlane);
    float fDot = DotProduct(_plane.normal, _pointToCheck)-fDistance;
    if (fDot == 0) return EPlaneResult::ON_PLANE;
    if (fDot < 0) return EPlaneResult::BEHIND;

    return EPlaneResult::IN_FRONT;

}
int main()
{
    //Create the window with a set resolution:
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Project");
    
    //Creating a convex shape:
    sf::ConvexShape Triangle;
    Triangle.setPointCount(3);
    Triangle.setPoint(0, sf::Vector2f(640.0f, 200.0f));
    Triangle.setPoint(1, sf::Vector2f(840.0f, 500.0f));
    Triangle.setPoint(2, sf::Vector2f(440.0f, 500.0f));
    Triangle.setFillColor(sf::Color::Green);
    
    LineSegment CurrentLine;
    Plane CurrentPlane;

    bool bLinePlaced = false;
    bool bPlanePlaced = false;
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
                if (!bLinePlaced) {
                    CurrentLine.start.position.x = sf::Mouse::getPosition(window).x;
                    CurrentLine.start.position.y = sf::Mouse::getPosition(window).y;
                    bLinePlaced = true;
                } 
                else if (!bPlanePlaced){
                    sf::Vector2f PlaneDirection(CurrentLine.start.position - CurrentLine.end.position);
                    sf::Vector2f PlaneNormal(-PlaneDirection.y, PlaneDirection.x);
                    PlaneNormal = Normalize(PlaneNormal);
                    CurrentPlane.normal = PlaneNormal;
                    CurrentPlane.pointOnPlane = CurrentLine.start.position;
                    bPlanePlaced = true;
                }
                else
                {
                    EPlaneResult Result = PlaneEquation(CurrentPlane, sf::Vector2f(sf::Mouse::getPosition(window)));

                    switch (Result)
                    {
                    case EPlaneResult::BEHIND:
                        std::cout << "Point is behind the plane!" << std::endl;
                        break;

                    case EPlaneResult::IN_FRONT:
                        std::cout << "Point is in front of the plane!" << std::endl;
                        break;

                    case EPlaneResult::ON_PLANE:
                        std::cout << "Point is on the plane!" << std::endl;
                        break;
                    }
                }

                break;
            }

        }
        if (bLinePlaced && !bPlanePlaced) {
            CurrentLine.end.position.x = sf::Mouse::getPosition(window).x;
            CurrentLine.end.position.y = sf::Mouse::getPosition(window).y;
         
        }
        window.clear();
        //Do all your drawing in here/////

        if (bLinePlaced ) {
            sf::Vertex LineToDraw[] = { CurrentLine.start, CurrentLine.end };
            window.draw(LineToDraw,2,sf::Lines);
        }
     
        

        //////////////////////////////////
        window.display();
    }

    return 0;
}