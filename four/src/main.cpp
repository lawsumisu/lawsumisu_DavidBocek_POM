#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>
#include "Ray.h"
#include "Group.h"
#include "Light.h"

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
    
    // Fill in your implementation here.
    int screenWidth;
    int screenHeight;
    char* filename;
    char* outputName;
    
    int depthWidth;
    int depthHeight;
    char* depthName;
    
    // This loop loops over each of the input arguments.
    // argNum is initialized to 1 because the first
    // "argument" provided to the program is actually the
    // name of the executable (in our case, "a4").
    for( int argNum = 1; argNum < argc; ++argNum )
    {
        string arg;
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
        if (!strcmp(argv[argNum],"-input")){
            filename = argv[argNum+1];
        }
        else if (!strcmp(argv[argNum],"-size")){
            sscanf(argv[argNum+1], "%i", &screenWidth);
            sscanf(argv[argNum+2], "%i", &screenHeight);
        }
        else if (!strcmp(argv[argNum],"-output")){
            outputName = argv[argNum+1];
        }
        else if (!strcmp(argv[argNum], "-depth")){
            sscanf(argv[argNum+1], "%i", &depthWidth);
            sscanf(argv[argNum+2], "%i", &depthHeight);
            depthName = argv[argNum+1];
        }
    }
    SceneParser scene = SceneParser(filename);


    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    Group* group = scene.getGroup();
    Camera* camera = scene.getCamera();
    Image image(screenWidth , screenHeight);
    Image depthImage(screenWidth, screenHeight);
    for (int i = 0; i<screenWidth; i++){
        for (int j = 0; j<screenHeight; j++){
            //For every pixel on the screen, draw a ray
            float x = ((float)i)/(screenWidth/2) - 1;
            float y = ((float)j)/(screenHeight/2) - 1;
            y = y*((float)screenHeight)/screenWidth;
            Ray ray = camera->generateRay(Vector2f(x,y));
            Hit hit = Hit();
            if (group->intersect(ray, hit, camera->getTMin())){
                float tCur = hit.getT();
                Vector3f p = ray.pointAtParameter(tCur);
                Vector3f color;
                for (int k = 0; k < scene.getNumLights(); k++){
                    Light* l = scene.getLight(k);
                    Vector3f dir, col;
                    float distanceToLight;
                    l->getIllumination(p,dir,col,distanceToLight);
                    Ray ray2 = Ray(p, dir);
                    Hit hit2 = Hit();
                    if (!group->intersect(ray2, hit2, 0.1)){
                        color = color + hit.getMaterial()->Shade(ray, hit, dir,col);
                        color = color + hit.getMaterial()->Specular(ray, hit, dir,col);
                    }
                }
                color = color + scene.getAmbientLight();
                image.SetPixel( i,j, color);                
            }
            else{
                image.SetPixel(i,j, scene.getBackgroundColor());
            }
        }
    
    }
    image.SaveImage(outputName);
    return 0;
}

