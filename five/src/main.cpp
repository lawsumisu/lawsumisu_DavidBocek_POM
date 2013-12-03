#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>
#include "Ray.h"
#include "Group.h"
#include "Light.h"
#include "RayTracer.h"

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"

float randRange(float lo, float hi){
    return lo + (hi-lo)*rand()/RAND_MAX;
}
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
    
    bool castShadows = false;
    bool hasJitter = false;
    bool hasFilter = false;
    int numBounces;
    int xRes = 3;
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
            depthName = argv[argNum+3];
        }
        else if (!strcmp(argv[argNum], "-shadows")){
            castShadows = true;
        }
        else if (!strcmp(argv[argNum], "-bounces")){
            sscanf(argv[argNum+1], "%i", &numBounces);
        }
        else if (!strcmp(argv[argNum], "-resolution")){
            sscanf(argv[argNum+1], "%i", &xRes);
            xRes = fmax(xRes, 1.0f);
        }
        else if (!strcmp(argv[argNum], "-jitter")){
            hasJitter = true;
        }
        else if (!strcmp(argv[argNum], "-filter")){
            hasFilter = true;
        }
    }
    SceneParser scene = SceneParser(filename);
    srand (static_cast <unsigned> (time(0)));

    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    RayTracer tracer = RayTracer(&scene, numBounces, castShadows);
    Camera* camera = scene.getCamera();
    float hiResWidth = screenWidth*xRes;
    float hiResHeight = screenHeight*xRes;
    Image hiResImage(hiResWidth , hiResHeight);
    Image SSImage(hiResWidth, hiResHeight);
    for (int i = 0; i<hiResWidth; i++){
        for (int j = 0; j<hiResHeight; j++){
            
            float ri = 0;
            float rj = 0;
            if (hasJitter){
                ri = randRange(-0.5f, 0.5f);
                rj = randRange(-0.5f, 0.5f);
            }
            //For every pixel on the screen, draw a ray
            float x = ((float)i+ri)/(hiResWidth/2) - 1;
            float y = ((float)j+rj)/(hiResHeight/2) - 1;
            y = y*((float)screenHeight)/screenWidth; //Handle aspect ratio
            Ray ray = camera->generateRay(Vector2f(x,y));
            Hit hit = Hit();
            Vector3f color = tracer.traceRay(ray, camera->getTMin(), 0,1,hit);
            hiResImage.SetPixel(i,j, color);
            SSImage.SetPixel(i,j, color);
        }
    }
    //Now, blur the supersampled image
    if (hasFilter){
        Image blurHorizontal(hiResWidth, hiResHeight);
        float K[5] = {0.1201,0.2339, 0.2931, 0.2339, 0.1201};
        for (int i = 0; i<hiResWidth; i++){
            for (int j = 0; j<hiResHeight; j++){
                Vector3f blurredPixel;
                for(int k = 0; k<5; k++){
                    //Blur horizontally.
                    int off = k-2;
                    int l = fmin(fmax(0.0f,j+off), hiResHeight-1);
                    Vector3f pixelColor = hiResImage.GetPixel(i,l);
                    blurredPixel = blurredPixel + pixelColor*K[k];
                }
                blurHorizontal.SetPixel(i,j, blurredPixel);
            }
        }
        Image blurVertical(hiResWidth, hiResHeight);
        for (int i = 0; i<hiResWidth; i++){
            for (int j = 0; j<hiResHeight; j++){
                Vector3f blurredPixel;
                for(int k = 0; k<5; k++){
                    //Blur horizontally.
                    int off = k-2;
                    int l = fmin(fmax(0.0f,i+off), hiResWidth-1);
                    Vector3f pixelColor = hiResImage.GetPixel(l,j);
                    blurredPixel = blurredPixel + pixelColor*K[k];
                }
                blurVertical.SetPixel(i,j, blurredPixel);
                SSImage.SetPixel(i,j, blurredPixel);
            }
        }
    }
    Image image(screenWidth, screenHeight);
    for (int i = 0; i<screenWidth; i++){
        for (int j = 0; j<screenHeight; j++){
            Vector3f averagedPixel;
            for(int k1 = 0; k1<xRes; k1++){
                for (int k2 = 0; k2<xRes; k2++){
                    averagedPixel = averagedPixel + SSImage.GetPixel(i*xRes+k1, j*xRes+k2)/pow((float)xRes,2);
                }
            }
            image.SetPixel(i,j, averagedPixel);
        }
    }
    image.SaveImage(outputName);
    return 0;
}

