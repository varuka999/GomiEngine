#include "GameState.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}
void GameState::Terminate()
{

}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

enum class Shape
{
    None,
    AABB,
    AABBFilled,
    Sphere,
    GroundPlane,
    GroundCircle,
    Transform,
};
const char* gShapeNames[] =
{
    "None",
    "AABB",
    "AABBFilled",
    "Sphere",
    "GroundPlane",
    "GroundCircle",
    "Transform"
};
Shape gCurrentShape = Shape::None;
Color gShapeColor = Colors::White;
float gPlaneSize = 1.0f;
float gRingSize = 16.0f;
void GameState::Render()
{
    switch (gCurrentShape)
    {
    case Shape::None:
        break;
    case Shape::AABB:
        SimpleDraw::AddAABB(1.0f, 1.0f, 1.0f, gPlaneSize, gPlaneSize, gPlaneSize, gShapeColor);
        break;
    case Shape::AABBFilled:
        SimpleDraw::AddFilledAABB(1.0f, 1.0f, 1.0f, gPlaneSize, gPlaneSize, gPlaneSize, gShapeColor);
        break;
    case Shape::Sphere:
        SimpleDraw::AddSphere(gRingSize, gRingSize, gPlaneSize, {0.0f, 0.0f, 0.0f}, gShapeColor);
        break;
    case Shape::GroundPlane:
        SimpleDraw::AddGroundPlane(gPlaneSize, gShapeColor);
        break;
    case Shape::GroundCircle:
        SimpleDraw::AddGroundCircle(gRingSize, gPlaneSize, { 0.0f, 0.0f, 0.0f }, gShapeColor);
        break;
    case Shape::Transform:
        //SimpleDraw::AddTransform(mMatrix)
        break;
    default:
        break;
    }

    SimpleDraw::Render(mCamera);
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    // CONTENT GOES BETWEEN BEGIN AND END
    ImGui::ColorEdit4("ShapeColor", &gShapeColor.r);
    int currentShape = (int)gCurrentShape;
    if (ImGui::Combo("Shape", &currentShape, gShapeNames, std::size(gShapeNames)))
    {
        gCurrentShape = (Shape)currentShape;
    }
    ImGui::DragFloat("PlaneSize", &gPlaneSize, 1.0f, 1.0f, 10000.0f);
    ImGui::DragFloat("RingSize", &gRingSize, 1.0f, 1.0f, 10000.0f);
    ImGui::End();
}
void GameState::UpdateCamera(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 1.5f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}