#include "Precompiled.h"
#include "GomiEngine.h"

GomiEngine::App& GomiEngine::MainApp()
{
    static App sApp;
    return sApp;
}