#include <GomiEngine/Inc/GomiEngine.h>
#include "ShapeStates.h"
#include "GameState.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    GomiEngine::AppConfig config;
    config.appName = L"Hello Mesh builder";

    GomiEngine::App& myApp = GomiEngine::MainApp();
    //myApp.AddState<SkySphereXState>("SkySphereXState");
    myApp.AddState<GameState>("GameState");

    myApp.Run(config);

    return 0;
}