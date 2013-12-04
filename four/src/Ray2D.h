#ifndef RAY2D_H
#define RAY2D_H

#include <cassert>
#include <iostream>
#include <Vector2f.h>

using namespace std;

// Ray class mostly copied from Peter Shirley and Keith Morley put into 2D
class Ray2D
{
public:

    Ray2D( const Vector2f& orig, const Vector2f& dir )
    {
        origin = orig; 
        direction = dir;
    }

    Ray2D( const Ray2D& r )
    { 
        origin = r.origin;
        direction = r.direction;
    }

    const Vector2f& getOrigin() const
    {
        return origin;
    }

    const Vector2f& getDirection() const
    {
        return direction;
    }

    Vector2f pointAtParameter( float t ) const
    {
        return origin + direction * t;
    }

private:

    // don't use this constructor
    Ray()
    {
        assert( false );
    }

    Vector2f origin;
    Vector2f direction;

};

inline ostream& operator << ( ostream& os, const Ray2D& r)
{
    os << "Ray <" << r.getOrigin() << ", " << r.getDirection() << ">";
    return os;
}

#endif // RAY2D_H
