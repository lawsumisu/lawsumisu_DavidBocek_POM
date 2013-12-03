Assignment 3 - README

The file can be ran be typing ./a5 followed by a sequence of arguments in the command line, such as input filename and output file for the bmp image. Inputs can be entered in any order, as long as -input, -size, and -output are all specified.

Optional commands:
* -shadows: adds shadows to the image.
* -resolution: sets the supersampling rate of the image. Must be an integer >= 1. Defaults to 3 if omitted. 
* -jitter: Jitters the samples
* -filter: Provides Gaussian blur for down-sampling.

Collaborators ------------------------------------------------------------

* Walter Menendez: Basically tackled this problem set together.
* Arjun Narayanan: Helped me with getting reflection and refraction to work normally


Project Statement -------------------------------------------------------------------

I am working with David Bocek on trying to implement Parallax for images. 

Parallax Occlusion Mapping

Overview: 
Implement texture map lookups with Parallax Occlusion Mapping algorithm (or POM). This gives simple meshes with a texture mapped plane a nice 3d look without having to sacrifice the significant performance and memory downsides of adding more complicated 3d meshes. This technique is widely used in games and virtual reality today, including: CryEngine 2; CryEngine 3; Uniengine (used in occulus rift). The technique also allows for the creation of stereoscopic images from only a single image source.
Features: Much faster method of creating 3d looking detail, avoiding the typical “sheared texture” look when viewing an object from a low tangential angle. Can create convincing 3d effects fast enough to run in real-time.
Drawbacks: The 3d object being simulated must actually be modeled at some point. Requires an additional map -- a depthmap/reliefmap -- to be created. Cannot handle concave shape details, since the depthmap is 2 dimensional and only records the highest point visible from the 2d plane.
Implementation Details: 
The steps are roughly: creation of a 3d model to be simulated; creation of a normalmap and depthmap for this model; placing a textured plane in the scene; rendering the plane with a slightly modified texture lookup algorithm. This last step is where most of the coding and work comes in for us, since we can use open source models and map generators that already exist. The basic idea is to adjust the u,v value of the texture lookup using the angle of the view, the depth value for the map at that point, and some trigonometry and antialiasing techniques (MIP-maps are popular in common implementations). Checks must also be made for occlusion, which can be calculated using the depthmap, normalmap, and tangent angle. These last two sections are the code heavy meat of the project, but can produce really stunning results. Plenty of literature exists currently, so hopefully we can adapt our current raytracer or raycaster to work with this!
Project Goals: 
Our goal is to code our own texture lookup algorithm to adjust the u,v coordinates and deal with occlusion. We will probably use third party map creation tools and maybe some simple models we create or third party models. The ideal end product should be able to render textures -- like jutting stone/rocks or decals of chunks taken out of a material -- on a 2d plane mesh while looking convincingly three dimensional with movement. Stretch goals could include: modeling a 3d object using only a rectangular prism, with the side planes of the prism each having a respective texture map; or, creating a stereoscopic image without needing to take multiple starting images. (We would need a 3d scene of the image beforehand, or a way to procedurally generate a depthmap from an image, but tools exist for this)


Notes ------------------------------------------------------------------------
Text files for images 1-5 and 7-9 have been removed as they were no longer expected to conform to the staff solutions for Assignment 4.
