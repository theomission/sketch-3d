# sketch-3d

A simple cross-platform 3D renderer. This is a learning experience project.

You can find the compiled demos (as of January 2015 the 22th) at this link: https://www.dropbox.com/sh/qhfo9fjf0jfxvjq/AADApvgPJ404oKv4PP980AQ7a?dl=0

The build is managed by CMake. Following are information for the different platforms on how to build the project and where to get the dependencies.

To properly build the project, you need to following libraries:
* Assimp
* Boost (unit\_test\_framework)
* FreeImage
* FreeType
* Glew
* OpenGL

And optionally:
* OIS

## Installation
### Windows
The minimum version for CMake is 2.8. CMake can be found at : http://www.cmake.org/files/
On Windows, the build also require the DirectX SDK.

The libraries can be found at those links:
* Assimp : http://sourceforge.net/projects/assimp/files/assimp-3.1/
* Boost (unit\_test\_framework) : http://www.boost.org/users/history/
* DirectX SDK : http://www.microsoft.com/en-us/download/details.aspx?id=6812
* FreeImage : http://freeimage.sourceforge.net/download.html
* FreeType (version 2.5.0 is need to properly build the project) : http://www.freetype.org/download.html
* Glew : http://glew.sourceforge.net/install.html

And optionally:
* OIS : http://sourceforge.net/projects/wgois/

The following environment variables should be set so that CMake can properly find the libraries on your system. Variables in curly brackets {} mean that any of those can be set for CMake:
* ASSIMPSDIR : Points at the base of your Assimp directory
* {Boost_DIR, BOOST_ROOT, BOOSTROOT} : Points at the base of your Boost directory
* {DXSDK_DIR, DIRECTX_HOME, DIRECTX_ROOT, DIRECTX_BASE} : Points at the base of your DirectX SDK directory
* FREEIMAGE_DIR : Points at the base of your FreeImage directory
* FREETYPE_DIR : Points at the base of your FreeType directory
* OISDIR : Points at the base of your OIS directory

### Linux
On Linux, you also need the libx11 library on top of all the other libraries. You also have to install CMake. All the libraries can be installed via apt-get
```
sudo apt-get install cmake
sudo apt-get install libassimp-dev
sudo apt-get install libboost-all-dev
sudo apt-get install libfreeimage-dev
sudo apt-get install libfreetype6-dev
sudo apt-get install libglew-dev
sudo apt-get install libois-dev
sudo apt-get install libx11-dev
```

### Mac OS X
CMake, Assimp, Boost and FreeImage can be installed using Homebrew.
```
brew install cmake
brew install assimp
brew install boost
brew install freeimage
```
OpenGL should be installed with the latest Mac OS update. You should also install the Xcode Command Line Tools. Then, install GLEW for OpenGL.
```
brew install glew
```

## Build
After the libraries are all downloaded and configured to be found by CMake, you need to follow these steps.
1. Create a folder where you will put the files generated by CMake (such as ./build)
2. Go in this folder and run the following command:
```
cmake ../. -G "Generator Name"
```
where the relative path "../." points to the root folder of the git repository and "Generator Name" is the name of a generator accepted by CMake which you will use to generate the binaries.
You can find all the generator names here : http://www.cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html

Here are some example depending of your OS:
* For Windows, use "Visual Studio 11" for Visual Studio 2012
* For Linux, use "Unix Makefiles"
* For Mac OS X, use "Unix Makefiles" or Xcode

## Samples
Currently the following samples are provided to showcase different 3d programming techniques:
* A burning piece of paper
* Cel shading
* Deffered shading
* Normal mapping
* Parallax mapping
* Shadow mapping using PCF
* Sponza Demo: A demo featuring Deferred rendering, normal mapping, GPU skinned meshes and shadows in the Sponza scene.
* Skeletal animation
* SSAO
* An ocean simulation based on Tessendorf paper
