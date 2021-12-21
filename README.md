# Software Renderer
This is a very basic 2D software renderer designed and implemented for the win32 platform layer.
All computations are done through the CPU and memory and completely removes the GPU from the equation.

## Platform
This application interfaces with the win32 platform layer and does so without the help of any external libraries, making the code extremely efficient.


## Rendering
All rendering is done through the creation of a bitmap and then displaying that bitmap on the screen.
The rendering options are basic but the way the rendering is handled allows for a good basis to expand to larger, more intesensive, rendering options.

## Build
Microsoft Visual Studio 2019 is required to build the application

To build: simply run the ```win32_build.bat``` from the directory containing ```win32_build.bat``` and the folder ```code```
This will create a ```build``` directory and there you will find a working executable for windows
