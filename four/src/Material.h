#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>
#include <cmath>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
#include "VecUtils.h"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:

    Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
      diffuseColor( d_color),specularColor(s_color), shininess(s)
      {

      }

      virtual ~Material()
      {

      }

      virtual Vector3f getDiffuseColor() const 
      {
          return  diffuseColor;
      }


      Vector3f Shade( const Ray& ray, const Hit& hit,
          const Vector3f& dirToLight, const Vector3f& lightColor ) {
          
          Vector3f k_d = getDiffuseColor();
          if (t.valid()){
              k_d = t(texCoord[0], texCoord[1]);
          }
          
          Vector3f n = hit.getNormal().normalized();
          Vector3f shade = k_d*max(0.0f,Vector3f::dot(dirToLight, n))*lightColor;
          
          return shade;
      }
    
      Vector3f Specular(const Ray& ray, const Hit& hit,
                        const Vector3f& dirToLight, const Vector3f& lightColor){
          Vector3f n = hit.getNormal().normalized();
          Vector3f specular;
          Vector3f k_s = specularColor;
          if (t.valid()) k_s = t(texCoord[0], texCoord[1]); 
          if (Vector3f::dot(n, dirToLight) > 0){
              Vector3f r_d = ray.getDirection();
              Vector3f r_reflect = r_d - 2*(Vector3f::dot(r_d, n)*n);
              float LR = max(0.0f,Vector3f::dot(r_reflect.normalized(), dirToLight));
              float intensity = pow(LR, shininess);
              specular = LR*intensity*k_s;
          }
          return specular;
      }
      void loadTexture(const char * filename){
          t.load(filename);
      }
      void setTexCoord(Vector2f uv){
          this->texCoord = uv;
      }
protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    Vector2f texCoord;
    float shininess;
    Texture t;
};



#endif // MATERIAL_H
