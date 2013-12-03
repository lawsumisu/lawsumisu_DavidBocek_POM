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

class Segment{
public:
    Segment(Vector2f start, Vector2f end){
        o = start;
        dir = end-start;
    }
    
    Vector2f dir(){
        return dir;
    }
    Vector2f o(){
        return o;
    }
    static bool intersect(Segment s1, Segment s2, Vector2f& intersection){
        float denom = cross(s1.dir(), s2.dir());
        float t_num = cross(s2.o()-s1.o(), s2.dir());
        float u_num = cross(s1.o()-s2.o(), s1.dir());
        
        if (denom == 0){
            //Lines are parallel.
            if (u_num == 0){
                //Lines are collinear
                intersection = s2.o();
                return true;
            }
            else if (t_num == 0){
                intersection = s1.o();
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
                intersection = s1.o()+t*s1.dir();
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
}

#endif
