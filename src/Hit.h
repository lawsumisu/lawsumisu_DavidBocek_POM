#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
#include "Ray.h"
#include <float.h>

class Material;

class Hit
{
public:

    // constructors
    Hit()
    {
        material = NULL;
        t = FLT_MAX;
        hasTex=false;
    }

    Hit( float _t, Material* m, const Vector3f& n )
    { 
        t = _t;
        material = m;
        normal = n;
        hasTex=false;
    }

    Hit( const Hit& h )
    { 
        t = h.t; 
        material = h.material; 
        normal = h.normal;
        hasTex=h.hasTex;
    }

    // destructor
    ~Hit()
    {

    }

    float getT() const
    {
        return t;
    }
    float getTWithOffset() const
    {
    	return t_hm;
    }

    Material* getMaterial() const
    {
        return material;
    }

    const Vector3f& getNormal() const
    {
        return normal;
    }
	void set( float _t, Material* m, const Vector3f& n){
		t = _t;
		material = m;
		normal = n;
		t_hm = _t;
	}
    void set( float _t, Material* m, const Vector3f& n, float t_at_heightmap )
    {
        t = _t;
        t_hm = t_at_heightmap;
        material = m;
        normal = n;
    }
    void setTexCoord(const Vector2f & coord){
        texCoord = coord;
        hasTex = true;
    }
    bool hasTex;
    Vector2f texCoord;
private:
	float t_hm;
    float t;
    Material* material;
    Vector3f normal;

};

inline ostream& operator << ( ostream &os, const Hit& h)
{
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif // HIT_H
