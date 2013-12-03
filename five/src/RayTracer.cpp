#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
    Vector3f R = incoming - 2*Vector3f::dot(normal, incoming)*normal;
    return R.normalized();
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
    float num = pow(index_n,2)*(1-pow(Vector3f::dot(normal, incoming),2));
    float denom = pow(index_nt,2);
    float value = 1- num/denom;
    
    if(value >=0){
        Vector3f direction = incoming-normal*Vector3f::dot(normal,incoming);
        transmitted = index_n/index_nt * direction - sqrt(value)*normal;
        transmitted.normalize();
        return true;
    }
    return false;
    
}
float weight(float index_n, float index_nt, float c){
    float R0 = pow((index_nt - index_n)/(index_nt+index_n),2);
    float R = R0 + (1-R0)*pow((1-c),5);
    return R;
}
RayTracer::RayTracer( SceneParser * scene, int max_bounces, bool castShadows
  //more arguments if you need...
                      ) :
  m_scene(scene)

{
  g=scene->getGroup();
  m_maxBounces = max_bounces;
    this->castShadows = castShadows;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit& hit ) const
{
    hit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
    if (g->intersect(ray, hit, tmin, false)){
        float tCur = hit.getT();
        Vector3f p = ray.pointAtParameter(tCur);
        Vector3f color = m_scene->getAmbientLight()*hit.getMaterial()->getDiffuseColor();
        for (int k = 0; k < m_scene->getNumLights(); k++){
            Light* l = m_scene->getLight(k);
            Vector3f dir, col;
            float distanceToLight;
            l->getIllumination(p,dir,col,distanceToLight);
            Ray ray2 = Ray(p, dir);
            Hit hit2 = Hit(distanceToLight, NULL, Vector3f( 0, 0, 0 ) );
            bool isShadowed = g->intersect(ray2, hit2, EPSILON, true);
            if (!castShadows || hit2.getT() == distanceToLight){
                color = color + hit.getMaterial()->Shade(ray, hit,dir,col);
            }
        }
        if (bounces < m_maxBounces){
            //Calculate Reflection
            Vector3f mirrorDir = mirrorDirection(hit.getNormal(), ray.getDirection());
            Ray mirrorRay = Ray(p, mirrorDir);
            Hit mirrorHit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
            Vector3f mirrorColor = traceRay(mirrorRay, EPSILON, bounces+1, refr_index, mirrorHit);
            
            float index_nt = hit.getMaterial()->getRefractionIndex();
            Vector3f normal = hit.getNormal();
            if (Vector3f::dot(hit.getNormal(), ray.getDirection()) > 0){
                index_nt = 1.0;
                normal = -normal;
            }
            if (index_nt > 0){
                //Calculate Refraction
                Vector3f transmitted;
                if (transmittedDirection(normal, ray.getDirection(), refr_index, index_nt, transmitted)){
                    float c = abs(Vector3f::dot(normal, transmitted));
                    if (refr_index <= index_nt) c = abs(Vector3f::dot(normal, ray.getDirection()));
                    float R = weight(refr_index, index_nt, c);
                    Ray refractedRay = Ray(p, transmitted);
                    Hit refractedHit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
                    Vector3f refractedColor = traceRay(refractedRay, EPSILON, bounces+1, index_nt, refractedHit);
                    color = color + ((1-R)*refractedColor + (R)*mirrorColor)*hit.getMaterial()->getSpecularColor();
                }
                else{
                    color = color + mirrorColor*hit.getMaterial()->getSpecularColor(); 
                }
            }
            else{
               color = color + mirrorColor*hit.getMaterial()->getSpecularColor(); 
            }
            return color;
        }
        else return color;
    }
    else{
        return m_scene->getBackgroundColor(ray.getDirection());
    }
}
