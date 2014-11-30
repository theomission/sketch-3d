#include <math/Quaternion.h>
#include <math/Vector3.h>

#include <render/Material.h>
#include <render/Mesh.h>
#include <render/Node.h>
#include <render/Renderer.h>
#include <render/SceneTree.h>
#include <render/Shader.h>

#include <system/Window.h>
#include <system/WindowEvent.h>
using namespace Sketch3D;

#include <string>
#include <vector>
using namespace std;

#ifdef OIS_AVAILABLE
#include <OIS.h>
#endif

#if defined(__WIN32__) || defined(_WIN32)
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
#else
int main(int argc, char** argv) {
#endif

    Window window("Sample_CelShading", 1024, 768, true);
    Renderer::GetInstance()->Initialize(RENDER_SYSTEM_OPENGL, window);
    Renderer::GetInstance()->SetClearColor(0.2f, 0.2f, 0.2f);

    // Load the mesh
    VertexAttributesMap_t vertexAttributes;
    vertexAttributes[VERTEX_ATTRIBUTES_POSITION] = 0;
    vertexAttributes[VERTEX_ATTRIBUTES_NORMAL] = 1;
    Mesh teapotMesh("Media/teapot.nff", vertexAttributes);

    // Create the material
    vector<string> vertexInputs;
    vertexInputs.push_back("in_vertex");
    vertexInputs.push_back("in_normal");
    Shader* normalRenderShader = Renderer::GetInstance()->CreateShader("Shaders/normalRender.vert", "Shaders/normalRender.frag", vertexInputs);
    Shader* contourRenderShader = Renderer::GetInstance()->CreateShader("Shaders/contourRender.vert", "Shaders/contourRender.frag", vertexInputs);

    Material normalRenderMaterial(normalRenderShader);
    Material contourRenderMaterial(contourRenderShader);

    // Create the node
    Node teapotNode;
    teapotNode.SetMesh(&teapotMesh);
    Renderer::GetInstance()->GetSceneTree().AddNode(&teapotNode);

    Renderer::GetInstance()->CameraLookAt(Vector3(0.0f, 2.5f, -7.5f), Vector3::LOOK);
    Renderer::GetInstance()->SetBlendingEquation(BLENDING_EQUATION_ADD);
    Renderer::GetInstance()->SetBlendingFactor(BLENDING_FACTOR_ONE, BLENDING_FACTOR_ONE);

    // Initialize OIS if available
#ifdef OIS_AVAILABLE
    size_t windowHandle = (size_t)window.GetHandle();

    OIS::ParamList paramList;
    paramList.insert(pair<string, string>("WINDOW", to_string(windowHandle)));

    OIS::InputManager* inputManager = OIS::InputManager::createInputSystem(paramList);

    OIS::Keyboard* keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, false));
#endif

    float teapotAngle = 0.0f;

    while (window.IsOpen()) {
        WindowEvent windowEvent;
        if (window.PollEvents(windowEvent)) {
        }

#ifdef OIS_AVAILABLE
        keyboard->capture();

        if (keyboard->isKeyDown(OIS::KC_ESCAPE)) {
            break;
        }
#endif
        Quaternion rotationX, rotationY;
        rotationX.MakeFromAngleAxis(-PI_OVER_2, Vector3::RIGHT);
        rotationY.MakeFromAngleAxis(teapotAngle, Vector3::LOOK);
        teapotNode.SetOrientation(rotationX * rotationY);
        teapotAngle += 0.0005f;

        Renderer::GetInstance()->Clear();
        
        Renderer::GetInstance()->SetCullingMethod(CULLING_METHOD_FRONT_FACE);
        teapotNode.SetMaterial(&contourRenderMaterial);
        contourRenderShader->SetActive(true);
        contourRenderShader->SetUniformFloat("offset", 0.025f);
        Renderer::GetInstance()->Render();
        
        Renderer::GetInstance()->SetCullingMethod(CULLING_METHOD_BACK_FACE);
        teapotNode.SetMaterial(&normalRenderMaterial);
        Renderer::GetInstance()->Render();
        
        Renderer::GetInstance()->EndRender();
    }

    return 0;
}