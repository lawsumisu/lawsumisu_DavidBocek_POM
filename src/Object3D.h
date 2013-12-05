#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Ray.h"
#include "Hit.h"
#include "Material.h"
#include "Light.h"

class Object3D
{
public:
    Object3D()
    {
        material = NULL;
    }
    virtual ~Object3D(){}

    Object3D( Material* material){
        this->material = material ; 
        this->heightmap = NULL;
    }
    
    Object3D( Material* material, Texture* heightmap){
    	this->material = material;
    	this->heightmap = heightmap;
    }

    virtual bool intersect( const Ray& r , Hit& h, float tmin) = 0;
	virtual bool heightmapIntersect( const Ray& r , Hit& h, float tmin, Vector3f& lightDir) = 0;

    char* type;
protected:

    Material* material;
    Texture* heightmap;
};

#endif

