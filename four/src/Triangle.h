#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include "Segment.h"
#include "POMUtils.h"
#include <vecmath.h>
#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle: public Object3D
{
public:
    Triangle();
    ///@param a b c are three vertex positions of the triangle
    Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object3D(m){
        this->a = a;
        this->b = b;
        this->c = c;
        this->material = m;
        hasTex = false;
    }

    virtual bool intersect( const Ray& ray,  Hit& hit , float tmin){
        //Uses Cramer's rule to calculate barycentric coordinates
        Vector3f r_d = ray.getDirection();
        Vector3f r_o = ray.getOrigin();
        
        Matrix3f m_A = Matrix3f(a-b, a-c, r_d);
        Matrix3f m_Beta = Matrix3f(a-r_o, a-c, r_d);
        Matrix3f m_Gamma = Matrix3f(a-b, a-r_o, r_d);
        Matrix3f m_T = Matrix3f(a-b, a-c, a-r_o);
        
        float A_determinant = m_A.determinant();
        
        float beta = m_Beta.determinant()/A_determinant;
        float gamma = m_Gamma.determinant()/A_determinant;
        float t = m_T.determinant()/A_determinant;
        
        if (beta+gamma <= 1 && beta >= 0 && gamma >=0 && t>tmin && t<hit.getT()){
            //Check for intersection here.
            Vector3f interpolatedNormal = normals[0]*(1-beta-gamma)+normals[1]*beta+normals[2]*gamma;
            //Do segment intersection to get appropriate texCoordinate.
            Segment incidentSegment = POMUtils::convertRayTo2DSegment(ray, interpolatedNormal);
            //incidentSegment.print();
            float length = incidentSegment.end()[0];
            //Regular method.
            /*for (int i=0; i<numPoints; i++){
                float n = numPoints;
                float d1 = i/n*length;
                float d2 = (i+1)/n*length;
                //TODO: Use deltas to get heights from height map.
                float h1; 
                float h2;
                Segment parallaxSegment = Segment(Vector2f(d1,h1), Vector2f(d2,h2));
                Vector2f intersection;
                if (Segment::intersect(incidentSegment, parallaxSegment, intersection)){
                    float delta = intersection.x();
                    //TODO: Use delta to get u,v texCoordinate
                    //material->setTexCoord(TODO: set tex coord here)
                }
            }*/
            //Binary method (In progress).
            float a = 0;
            float c = numPoints;
            float n = numPoints;
            while(true){
                float b = ((int)c)/2;
                float d1 = a/n*length;
                float d2 = b/n*length;
                float d3 = c/n*length;
                float h1 = 0.5f;
                float h2 = 0.5f;
                float h3 = 0.5f;
                Segment parallaxSegment1 = Segment(Vector2f(d1,h1), Vector2f(d2,h2));
                parallaxSegment1.print();
                Segment parallaxSegment2 = Segment(Vector2f(d2,h2), Vector2f(d3,h3));
                parallaxSegment2.print();
                Vector2f intersection;
                if (Segment::intersect(incidentSegment, parallaxSegment1, intersection)){
                    float delta = intersection.x();
                    cout <<"Left intersect: " << delta << " " << length <<  endl;
                    break;
                    //TODO: Use delta to get u,v texCoordinate
                    //material->setTexCoord(TODO: set tex coord here)
                }
                else if (Segment::intersect(incidentSegment, parallaxSegment2, intersection)){
                    float delta = intersection.x();
                    cout << "Right intersect: " << delta << endl;
                    break;
                    //TODO: Use delta to get u,v texCoordinate
                    //material->setTexCoord(TODO: set tex coord here)
                }

            }
            material->setTexCoord(texCoords[0]*(1-beta-gamma)+texCoords[1]*beta+texCoords[2]*gamma);
            hit.set(t, material, interpolatedNormal);
            return true;
        }
                            
        
        return false;
    }
    bool hasTex;
    Vector3f normals[3];
    Vector2f texCoords[3];
    int numPoints = 1000;
protected:
    Vector3f a;
    Vector3f b;
    Vector3f c;
    Material* material;

};

#endif //TRIANGLE_H
