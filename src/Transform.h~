#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
#include "Ray.h"
#include "VecUtils.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform: public Object3D
{
public: 
    Transform(){}
    Transform( const Matrix4f& m, Object3D* obj ):o(obj){
        this->o = obj;
        this->transform = m;
    }
    ~Transform(){
    }
    virtual bool intersect( const Ray& r , Hit& h , float tmin){
        Vector3f r_o_trans = VecUtils::transformPoint(this->transform.inverse(), r.getOrigin());
        Vector3f r_d_trans = VecUtils::transformDirection(this->transform.inverse(), r.getDirection());
        Ray rTrans = Ray(r_o_trans, r_d_trans);
        if (o->intersect( rTrans , h , tmin)){
            Matrix3f normalTrans = transform.getSubmatrix3x3(0,0).inverse().transposed();
            h.set(h.getT(), h.getMaterial(), (normalTrans*h.getNormal()).normalized());
            return true;
        }
        return false;
    }

protected:
    Object3D* o; //un-transformed object
    Matrix4f transform;
                                             
};

#endif //TRANSFORM_H
