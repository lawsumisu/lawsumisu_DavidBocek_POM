//
//  Segment.h
//  
//
//  Created by Law Sumisu on 11/29/13.
//
//
/*  Segment logic is based on writing from Graphics Gems, as cited by this page
 *  http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
 */
#ifndef _Segment_h
#define _Segment_h

#include <vecmath.h>
#include <cstdlib>

class Segment{
public:
    Segment(Vector2f start, Vector2f end){
        o = start;
        dir = end-start;
    }
    
    Vector2f direction(){
        return dir;
    }
    Vector2f origin(){
        return o;
    }
    Vector2f end(){
        return o+dir;
    }
    void print(){
        cout << "Segment: " << "(" << o.x() << "," << o.y() << ") -> (" << end().x() << "," << end().y() << ")" << endl;
    }
    static bool intersect(Segment s1, Segment s2, Vector2f& intersection){
        float denom = cross(s1.direction(), s2.direction());
        float t_num = cross(s2.origin()-s1.origin(), s2.direction());
        float u_num = cross(s1.origin()-s2.origin(), s1.direction());
        
        if (denom == 0){
            //Lines are parallel.
            if (u_num == 0){
                //Lines are collinear
                intersection = s2.origin();
                return true;
            }
            else if (t_num == 0){
                intersection = s1.origin();
                return true;
            }
            else{
                //Parallel lines never intersect.
                return false;
            }
        }
        else{
            //Lines can intersect
            float u = u_num/denom;
            float t = t_num/denom;
            if (u >=0 && u<=1 && t>=0 && t<=1){
                intersection = s1.origin()+t*s1.direction();
                return true;
            }
            else{
                return false;
            }
        }
    }
private:
    Vector2f o;     //Start location of segment
    Vector2f dir;    //Direction of segment
    
    static float cross(Vector2f v, Vector2f w){
        return v.x()*w.y() - v.y()*w.x();
    }
};

#endif
