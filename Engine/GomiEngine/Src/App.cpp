#include "Precompiled.h"
#include "App.h"

using namespace GomiEngine;
using namespace GomiEngine::Core;

void App::Run(const AppConfig& config)
{
    // start
    LOG("App Started");
    // for all systems built, intiialize all singletons
    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        config.appName,
        config.winWidth,
        config.winHeight);

    // run the application
    mRunning = true;
    while (mRunning)
    {
        myWindow.ProcessMessage();
        if (!myWindow.IsActive())
        {
            Quit();
            continue;
        }
    }

    // for all systems built, terminate all singletons
    // close
    myWindow.Terminate();
    LOG("App Ended");
}
void App::Quit()
{
    mRunning = false;
}