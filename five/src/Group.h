#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:

    Group(){
        num_objects = 0;
    }

    Group( int num_objects ){
        this->num_objects = num_objects;
        for (int i=0; i< num_objects; i++){
            this->objects.push_back(NULL);
        }
    }

    ~Group(){

    }

    virtual bool intersect( const Ray& r , Hit& h , float tmin ,bool isShadow) {
        bool hasIntersected = false;
        for (int i = 0; i<num_objects; i++){
            if (objects[i]->intersect(r, h, tmin, isShadow)){
                hasIntersected = true;
                if (isShadow) return true;
            }
        }
        return hasIntersected;
    }

    void addObject( int index , Object3D* obj ){
        this->objects[index] = obj;
    }

    int getGroupSize(){ 
        return this->num_objects;
    }

private:
    
    int num_objects;
    vector<Object3D*> objects;

};

#endif

