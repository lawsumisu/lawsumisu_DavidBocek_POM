#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
    Sphere(){ 
        //unit ball at the center
    }

    Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
        
        this->center = center;
        this->radius = radius;
        this->material = material;
    }


    ~Sphere(){}

    virtual bool intersect( const Ray& r , Hit& h , float tmin, bool isShadow){
        Vector3f r_o = r.getOrigin()-center;
        Vector3f r_d = r.getDirection();
        
        float a = Vector3f::dot(r_d, r_d);
        float b = 2*Vector3f::dot(r_o, r_d);
        float c = Vector3f::dot(r_o,r_o)-radius*radius;
        
        vector<float> solutions = solveQuadratic(a,b,c);
        if (solutions.size() == 2){
            // We have two solutions, so now find the closest "positive" (greater
            // than or equal to tmin) t value from these solutions.
            float t = numeric_limits<float>::infinity();
            for (int i = 0; i<solutions.size();i++){
                //Take the min of the solution values that are also >= tmin.
                if (solutions[i] >= tmin && solutions[i] < t) t = solutions[i];
            }
            float thit = h.getT();
            if (t < thit){
                //If this time is less than thit, then change the Hit to
                //reflect the intersection between r and this object.
                Vector3f position = r.pointAtParameter(t);
                Vector3f n = position - center; //Calculate normal.
                h.set(t, this->material, n.normalized());
                return true;
            }
            return false;
        }
        return false;
    }

protected:

    Vector3f center;
    float radius;
    
    // Returns a vector v where v[0] and v[1] are the real solutions to the
    // quadratic equation defined by ax^2+bx+c = 0.
    // if v is empty, then there are no real solutions.
    vector<float> solveQuadratic(float a, float b, float c){
        
        vector<float> output;
        
        float descriminant = b*b - 4*a*c;
        if (descriminant < 0){
            return output;
        }
        float x1 = (-b+sqrt(descriminant))/(2*a);
        float x2 = (-b-sqrt(descriminant))/(2*a);
        
        output.push_back(x1);
        output.push_back(x2);
        return output;
    }

};


#endif
