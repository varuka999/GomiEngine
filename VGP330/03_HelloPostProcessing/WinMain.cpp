#include <GomiEngine/Inc/GomiEngine.h>
#include "GameState.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    GomiEngine::AppConfig config;
    config.appName = L"Hello Post Processing";

    GomiEngine::App& myApp = GomiEngine::MainApp();
    myApp.AddState<GameState>("GameState");

    myApp.Run(config);

    return 0;
}