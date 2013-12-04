#ifndef PLANETRIANGLE_H
#define PLANETRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class PlaneTriangle: public Object3D
{
public:
    PlaneTriangle(){ 
        //unit ball at the center
    }

    PlaneTriangle(float width, float height , Material* material ):Object3D(material){
        
        this->width = width;
        this->height = height;
        this->material = material;

        v1 = Vector3f(-width/2, height/2, 0);
        v2 = Vector3f(width/2, height/2, 0);
        v3 = Vector3f(-width/2, -height/2,0);
        v4 = Vector3f (width/2, -height/2, 0);
        normal = Vector3f::cross(v1-v2, v3-v1).normalized();
        normal.print();
        v1.print();
    }


    ~PlaneTriangle(){}

    virtual bool intersect( const Ray& r , Hit& h , float tmin){
        Triangle* t1 = new Triangle(v1, v3, v4, material);
        Triangle* t2 = new Triangle(v1, v4, v2, material);
        float t = 0;
        if (t1->triangleIntersect(r, h, tmin, t)){
            Vector3f incidentPoint = r.pointAtParameter(t)+ v4;
            Vector2f uv = Vector2f(incidentPoint.x()/width, -incidentPoint.y()/height);
            material->setTexCoord(uv);
            h.set(t, material, normal);
            cout << "here" << endl;
            return true;
        }
        else if (t2->triangleIntersect(r,h,tmin, t)){
            Vector3f incidentPoint = r.pointAtParameter(t)+v4;
            Vector2f uv = Vector2f(incidentPoint.x()/width, -incidentPoint.y()/height);
            material->setTexCoord(uv);
            h.set(t, material, normal);
            cout << "there" << endl;
            return true;
        }
        else{
            return false;
        }

    }

protected:
    float width;
    float height;

    Vector3f v1;
    Vector3f v2;
    Vector3f v3;
    Vector3f v4;
    Vector3f normal;
};


#endif
