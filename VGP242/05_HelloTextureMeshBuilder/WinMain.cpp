#include <GomiEngine/Inc/GomiEngine.h>
#include "ShapeStates.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    GomiEngine::AppConfig config;
    config.appName = L"Hello Mesh builder";

    GomiEngine::App& myApp = GomiEngine::MainApp();
    //myApp.AddState<ShapeStates>("ShapeState");
    //myApp.AddState<VertexCubeState>("VertexCubeState");
    //myApp.AddState<CubeState>("CubeState");
    //myApp.AddState<CubePXState>("CubePXState");
    //myApp.AddState<BoxState>("BoxState");
    //myApp.AddState<PyramidState>("PyramidState");
    //myApp.AddState<PlaneState>("PlaneState");
    //myApp.AddState<CylinderState>("CylinderState");
    //myApp.AddState<SphereState>("SphereState");
    //myApp.AddState<SphereXState>("SphereXState");
    myApp.AddState<SkySphereXState>("SkySphereXState");

    myApp.Run(config);

    return 0;
}