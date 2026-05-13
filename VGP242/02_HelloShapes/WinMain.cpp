#include <GomiEngine/Inc/GomiEngine.h>
#include "ShapeStates.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    GomiEngine::AppConfig config;
    config.appName = L"Hello Shapes";

    GomiEngine::App& myApp = GomiEngine::MainApp();
    //myApp.AddState<ShapeStates>("ShapeState");
    //myApp.AddState<QuadStates>("QuadState");
    myApp.AddState<PentaStates>("PentaState");
    myApp.AddState<PentaStates2>("PentaState2");
    myApp.AddState<PentaStates3>("PentaState3");
    myApp.Run(config);

    return 0;
}