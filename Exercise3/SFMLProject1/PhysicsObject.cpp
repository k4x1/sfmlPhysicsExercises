    #include "PhysicsObject.h"
    #include "PhysicsLibrary.h"



    PhysicsObject::PhysicsObject(sf::Vector2f _startPosition, sf::Vector2f _endPosition, float _mass)
    {
        StartPosition = _startPosition;
        EndPosition = _endPosition;
        Mass = _mass;
        Length = GetVectorLength(EndPosition - StartPosition);
    }

    void PhysicsObject::ApplyForce(sf::Vector2f _force)
    {
        Acceleration += (_force / Mass) / 60.0f;
    }

    void PhysicsObject::ApplyImpulse(sf::Vector2f _impulse)
    {
        Acceleration += (_impulse / Mass);
    }

    void PhysicsObject::UpdatePhysics()
    {
        Velocity += Acceleration;
        StartPosition += Velocity * g_MoveScale / 60.0f;
        EndPosition += Velocity * g_MoveScale / 60.0f;
        Acceleration = { 0.0f, 0.0f };
    }

    void PhysicsObject::SetMass(float _mass)
    {
        Mass = _mass;
    }

    void PhysicsObject::CollideObject(sf::RenderWindow& _window)
    {
        // Hitting the right wall
        float WallPenetration = fmin(_window.getSize().x - StartPosition.x - GetRadius(),
            _window.getSize().x - EndPosition.x - GetRadius());

        StartPosition.x += fmin(WallPenetration, 0.0f);
        EndPosition.x += fmin(WallPenetration, 0.0f);

        if (WallPenetration < 0) {
            Velocity.x *= -1;
        }

        // Hitting the left wall
        WallPenetration = fmin(StartPosition.x - GetRadius(),
            EndPosition.x - GetRadius());

        StartPosition.x -= fmin(WallPenetration, 0.0f);
        EndPosition.x -= fmin(WallPenetration, 0.0f);

        if (WallPenetration < 0) {
            Velocity.x *= -1;
        }

        // Hitting the floor
    
        WallPenetration = fmin(_window.getSize().y - StartPosition.y - GetRadius(),
            _window.getSize().y - EndPosition.y - GetRadius());

        StartPosition.y += fmin(WallPenetration, 0.0f);
        EndPosition.y += fmin(WallPenetration, 0.0f);

        if (WallPenetration < 0) {
            Velocity.y *= -1;
        }

        // Hitting the ceiling 
        WallPenetration = fmin(StartPosition.y - GetRadius(),
            EndPosition.y - GetRadius());

        StartPosition.y -= fmin(WallPenetration, 0.0f);
        EndPosition.y -= fmin(WallPenetration, 0.0f);

        if (WallPenetration < 0) {
            Velocity.y *= -1;
        }
    }

    void PhysicsObject::CollideCapsules(std::vector<PhysicsObject*> _capsules)
    {
        for (PhysicsObject* other : _capsules)
        {
            if (other == this) continue;

            float shortestDistance = std::numeric_limits<float>::max();
            sf::Vector2f shortestVector;

            // Check all combinations of points and lines
            sf::Vector2f vectors[4] = {
                -VectorToLine(StartPosition, other->GetStartPosition(), other->GetEndPosition()),
                -VectorToLine(EndPosition, other->GetStartPosition(), other->GetEndPosition()),
                VectorToLine(other->GetStartPosition(), GetStartPosition(), GetEndPosition()),
                VectorToLine(other->GetEndPosition(), GetStartPosition(), GetEndPosition())
            };

            for (int i = 0; i < 4; i++)
            {
                float distance = GetVectorLength(vectors[i]);
                if (distance < shortestDistance)
                {
                    shortestDistance = distance;
                    shortestVector = vectors[i];
                }
            }

            float combinedRadii = GetRadius() + other->GetRadius();
            if (shortestDistance < combinedRadii)
            {
                float impulse = 1.0f;
                // Collision response
                sf::Vector2f pushVector = Normalize(shortestVector);
                float midDistnce = combinedRadii - shortestDistance;
                pushVector *= midDistnce;

                // Separate the objects
                StartPosition += pushVector;
                EndPosition += pushVector;
                /*other->StartPosition -= penetrationVector / 2.0f;
                other->EndPosition -= penetrationVector / 2.0f;*/

                // Apply impulse
                ApplyImpulse(Normalize(pushVector) * impulse);
                other->ApplyImpulse(Normalize(pushVector) * -impulse);

            }
                
        }
    }


    float PhysicsObject::GetRadius()
    {
        return sqrt(Mass)*g_MassSizeScale;
    }

    float PhysicsObject::GetLength()
    {
        return Length;
    }

    sf::Vector2f PhysicsObject::GetStartPosition()
    {
        return StartPosition;
    }

    sf::Vector2f PhysicsObject::GetEndPosition()
    {
        return EndPosition;
    }

    float PhysicsObject::GetMass()
    {
        return Mass;
    }

