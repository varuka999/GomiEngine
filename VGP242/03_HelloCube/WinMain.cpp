#include <GomiEngine/Inc/GomiEngine.h>
#include "ShapeStates.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    GomiEngine::AppConfig config;
    config.appName = L"Hello Cubes";

    GomiEngine::App& myApp = GomiEngine::MainApp();
    myApp.AddState<ShapeStates>("ShapeState");

    myApp.Run(config);

    return 0;
}