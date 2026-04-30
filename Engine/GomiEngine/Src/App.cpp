#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace GomiEngine;
using namespace GomiEngine::Core;
using namespace GomiEngine::Input;
using namespace GomiEngine::Graphics;

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

    auto handle = myWindow.GetWindowHandle();
    InputSystem::StaticInitialize(handle);
    GraphicsSystem::StaticInitialize(handle, config.fullScreen);

    // after initializing singletons, initialize current state
    ASSERT(mCurrentState != nullptr, "App: need an app state to run");
    mCurrentState->Initialize();

    // run the application
    InputSystem* input = InputSystem::Get();
    mRunning = true;
    while (mRunning)
    {
        myWindow.ProcessMessage();
        input->Update();

        if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
        {
            Quit();
            continue;
        }

        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }

        float deltaTime = TimeUtil::GetDeltaTime();
#if defined(_DEBUG)
        if (deltaTime > 0.5f)
        {
            LOG("App: really long deltaTime, skipping frame %f", deltaTime);
            continue;
        }
#endif
        mCurrentState->Update(deltaTime);

        // render flow
        GraphicsSystem* gs = GraphicsSystem::Get();
        gs->BeginRender();
        mCurrentState->Render();
        gs->EndRender();
    }
    // terminate active state first
    mCurrentState->Terminate();
    // for all systems built, terminate all singletons
    GraphicsSystem::StaticTerminate();
    InputSystem::StaticTerminate();

    // close the application
    myWindow.Terminate();
    LOG("App Ended");
}
void App::Quit()
{
    mRunning = false;
}
void App::ChangeState(const std::string& stateName)
{
    auto iter = mAppStates.find(stateName);
    if (iter != mAppStates.end())
    {
        mNextState = iter->second.get();
    }
}