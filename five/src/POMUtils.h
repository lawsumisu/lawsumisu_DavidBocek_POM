//
//  POMUtils.h
//  
//
//  Created by Law Sumisu on 11/26/13.
//
//

#ifndef _POMUtils_h
#define _POMUtils_h

#include <vecmath.h>
#include "Ray.h"
#include "Segment.h"
#include <cmath>

class POMUtils{
public:
    static bool lineSegIntersect(Ray ray, Vector3f q, Vector3f s, Vector3f& intersection){
        Vector3f d = ray.getDirection();    //Direction of the ray
        Vector3f o = ray.getOrigin();       //Origin of the ray.
        
        float denom = Vector3f::cross(d, s).abs();
        float t_num = Vector3f::cross(q-o, s).abs();
        float u_num = Vector3f::cross(o-q, d).abs();
        
        if (denom == 0){
            //Lines are parallel.
            if (u_num == 0){
                //Lines are collinear.
                intersection = q;
                return true;
            }
            else{
                //Parallel lines never intersect.
                return false;
            }
        }
        else{
            //Lines can intersect
            if (u_num >= 0 && u_num <=1 && t_num >= 0){
                intersection = ray.pointAtParameter(t_num);
                return true;
            }
            else{
                return false;
            }
        }
    }
    
    /*
     * Method that converts a point in <delta, height> space into a point in <x,y,z>
     * space as an input origin point.
     */
    static Vector3f getHeightMapPoint(float delta, float height, Vector3f origin, Vector3f normal, Vector3f projection){
        return origin + height*-normal + delta*projection;
    }
    /* 
     * Method that gets the projection of an incident ray on a plane.
     */
    static Vector3f getProjection(Ray ray, Vector3f normal){
        Vector3f dir = ray.getDirection();
        Vector3f normalComponent = Vector3f::dot(dir, normal)*normal;
        return dir-normal;
    }
    
    /*
     * Converts an incident ray on a plane to a 2D Segment located at the origin
     */
    static Segment convertRayTo2DSegment(Ray ray, Vector3f normal){
        float angle = acos(Vector3f::dot(ray.getDirection(), normal));
        return Segment(Vector2f::ZERO, Vector2f(tan(angle), 1));
    }
}
#endif
