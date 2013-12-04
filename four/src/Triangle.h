#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include "Segment.h"
#include "POMUtils.h"
#include <vecmath.h>
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
        this->numPoints = 1000;
    }
    //overloaded constructor with heightmap material pointer
    Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m, Texture* heightmap):Object3D(m){
    	this->a = a;
    	this->b = b;
    	this->c = c;
    	this->material = m;
    	this->heightmap = heightmap;
    	this->numPoints = 1000;
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
        	//parrallax offset of UV coord
        	Vector2f uvCoordOffset = Vector2f::ZERO;
            //Check for intersection here.
            Vector3f interpolatedNormal = normals[0]*(1-beta-gamma)+normals[1]*beta+normals[2]*gamma;
            //Do segment intersection to get appropriate texCoordinate offset.
            if (heightmap != NULL){ //only run this if there is a heightmap and texturemap supplied
		        Segment incidentSegment = POMUtils::convertRayTo2DSegment(ray, interpolatedNormal);
		        float length = incidentSegment.end()[0];
		        for (int i=0; i<numPoints; i++){
		            float d1 = i/numPoints*length;
		            float d2 = (i+1)/numPoints*length;
		            //Use deltas to get heights from height map.
		            //get hit point in UV
		            Vector2f hitUV = hit.texCoord;
		            //get ray direction projected to UV
		            Vector3f rayDirUVN = transformXYZtoUVN(ray.getDirection());
		            cout << rayDirUVN[0] << " " << rayDirUVN[1] << " " << rayDirUVN[2] << endl;
		            Vector2f rayDirUV = Vector2f(rayDirUVN[0],rayDirUVN[1]);
		            //Query heightmap at {d1,d2} along T
		            float h1 = POMUtils::QueryHeightmap(hitUV + rayDirUV * d1, heightmap);
		            float h2 = POMUtils::QueryHeightmap(hitUV + rayDirUV * d2, heightmap);
		            Segment parallaxSegment = Segment(Vector2f(d1,h1), Vector2f(d2,h2));
		            Vector2f intersection;
		            if (Segment::intersect(incidentSegment, parallaxSegment, intersection)){
		                float delta = intersection.x();
		                //get coordinate offset for this delta
		                uvCoordOffset = hitUV + rayDirUV * delta;
		            }
		        }
            }
            //barycentric interpolation with added parallax offset
            material->setTexCoord(texCoords[0]*(1-beta-gamma)+texCoords[1]*beta+texCoords[2]*gamma+uvCoordOffset);
            hit.set(t, material, interpolatedNormal);
            return true;
        }
                            
        
        return false;
    }
    
    Vector3f transformXYZtoUVN(Vector3f xyzCoords){
    	Vector3f e12_XYZ = b-a;
    	Vector3f e13_XYZ = c-a;
    	Vector2f e12_UV = texCoords[1] - texCoords[0];
    	Vector2f e13_UV = texCoords[2] - texCoords[0];
    	Vector3f uVec = e12_UV[0] == 0 ? (e13_XYZ / e13_UV[0]).normalized() : 
    									(e12_XYZ / e12_UV[0]).normalized();
    	Vector3f nVec = Vector3f::cross(e12_XYZ,e13_XYZ);
    	Vector3f vVec = Vector3f::cross(uVec,nVec);
    	uVec.print(); vVec.print(); nVec.print();
    	Matrix3f M = Matrix3f(uVec,vVec,nVec);
    	M.transpose();
    	M.print();
    	return M * xyzCoords;
    }
    
    bool hasTex;
    Vector3f normals[3];
    Vector2f texCoords[3];
    int numPoints;
protected:
    Vector3f a;
    Vector3f b;
    Vector3f c;
    Material* material;
    Texture* heightmap;

};

#endif //TRIANGLE_H
