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
    
    static Vector3f getHeightMapPoint(float height, float delta, Vector3f normal, Vector3f tangent, Vector3f origin){
        return origin + height*-normal + delta*tangent;
    }
}
#endif
