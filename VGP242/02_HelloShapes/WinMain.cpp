#include <GomiEngine/Inc/GomiEngine.h>
#include "ShapeStates.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    GomiEngine::AppConfig config;
    config.appName = L"Hello Shapes";

    GomiEngine::App& myApp = GomiEngine::MainApp();
    myApp.AddState<ShapeState>("ShapeState");
    myApp.Run(config);

    return 0;
}