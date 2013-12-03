#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
using namespace std;
///TODO: Implement Plane representing an infinite plane
///choose your representation , add more fields and fill in the functions
class Plane: public Object3D
{
public:
    Plane(){}
    Plane( const Vector3f& normal , float d , Material* m):Object3D(m){
        this->normal = normal;
        this->d = -d;
        this->material = m;
    }
    ~Plane(){}
    virtual bool intersect( const Ray& r , Hit& h , float tmin){
        Vector3f r_o = r.getOrigin();
        Vector3f r_d = r.getDirection();
        float denom = Vector3f::dot(normal, r_d);
        if (denom == 0){
            return false;
        }
        float t = -(d + Vector3f::dot(normal, r_o))/denom;
        if (t > tmin && t < h.getT()){
            h.set(t, material, normal);
            
            return true;
        }
        return false;
    }

protected:
    Vector3f normal;
    float d;
    Material* material;
};
#endif //PLANE_H


