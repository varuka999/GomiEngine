#include "Precompiled.h"
#include "App.h"

using namespace GomiEngine;


void App::Run(const AppConfig& config)
{
    // start
    LOG("App Started");

    // for all systems built, intiialize all singletons

    // run the application

    // for all systems built, terminate all singletons
    // close
    LOG("App Ended");
}
void App::Quit()
{
    mRunning = false;
}