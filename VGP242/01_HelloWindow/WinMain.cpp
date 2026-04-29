#include <GomiEngine/Inc/GomiEngine.h>

using namespace GomiEngine;

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Window";

    App& mainApp = MainApp();
    mainApp.Run(config);
    return 0;
}