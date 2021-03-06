#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
    //generate rays for each screen-space coordinate
    virtual Ray generateRay( const Vector2f& point ) = 0 ; 

    virtual float getTMin() const = 0 ; 
    virtual ~Camera(){}
protected:
    Vector3f center; 
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
    PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
        
        this->center = center;
        this->direction = direction;
        this->horizontal = Vector3f::cross(direction, up).normalized();
        this->up = Vector3f::cross(horizontal, direction).normalized();
        this->angle = angle;
        

    }

    virtual Ray generateRay( const Vector2f& point){
        float D = 1/(tan(angle/2.0));
        float x = point[0];
        float y = point[1];
        
        Vector3f rayDirection = x*horizontal.normalized() + y*up.normalized() + D*direction.normalized();
        return Ray(center, rayDirection.normalized()); 
    }

    virtual float getTMin() const { 
        return 0.0f;
    }

private:
    
    Vector3f up;
    Vector3f horizontal;
    Vector3f direction;
    Vector3f center;
    float angle;

};

#endif //CAMERA_H
