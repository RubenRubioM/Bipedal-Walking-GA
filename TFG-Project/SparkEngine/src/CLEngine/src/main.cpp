/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
#include <iostream>
#include <map>
#include <memory>
#include <string>

// INCLUDES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <SOIL2/SOIL2.h>


//SRC
#include <stdio.h>  /* printf, scanf, puts, NULL */
#include <stdlib.h> /* srand, rand */
#include <string.h> /* string */
#include <time.h>   /* time */

#include "Built-In-Classes/CLColor.h"
#include "CLEngine.h"
#include "ResourceManager/CLResource.h"
#include "ResourceManager/CLResourceManager.h"
#include "ResourceManager/CLResourceMesh.h"
#include "ResourceManager/CLResourceShader.h"
#include "SceneTree/CLCamera.h"
#include "SceneTree/CLMesh.h"
#include "SceneTree/CLNode.h"
#include "SceneTree/CLParticleSystem.h"
#include "SceneTree/CLPointLight.h"

using namespace std;
using namespace CLE;

int main() {

    CLEngine* device = new CLEngine(1280, 720, "Beast Brawl");

    try {
        //-------------------Resource manager-------------------
        CLResourceManager* resourceManager = CLResourceManager::GetResourceManager();
        auto resourceShaderCartoon = resourceManager->GetResourceShader("../Shaders/cartoonShader.vert", "../Shaders/cartoonShader.frag");
        auto resourceMeshGround = resourceManager->GetResourceMesh("media/training_ground.obj", true);
        auto resourceMesh = resourceManager->GetResourceMesh("media/sharky_wheel2.obj", true);
        //------------------------------------------------------------------------- ARBOLITO

        //Nodo raiz
        auto light1 = device->AddPointLight(device->GetRootNode(),1);
        light1->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        static_cast<CLPointLight*>(light1->GetEntity())->SetLightAttributes(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1, 0.1f), 1.0f, 0.00005f, 0.0000014f);

        auto light2 = device->AddPointLight(device->GetRootNode(),2);
        light2->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        static_cast<CLPointLight*>(light2->GetEntity())->SetLightAttributes(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1, 0.1f), 1.0f, 0.00007f, 0.00008f);

        auto mesh1 = device->AddMesh(device->GetRootNode(),3);

        auto mesh2 = device->AddMesh(device->GetRootNode(),5);
        auto mesh3 = device->AddMesh(mesh2, 6);

        auto camera = device->AddCamera(device->GetRootNode(),4);
        camera->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

        static_cast<CLCamera*>(camera->GetEntity())->SetCameraTarget(mesh2->GetTranslation());

        string fileBillBoard = "media/mrPinguin.png";
        device->AddBillBoard(mesh2,10, fileBillBoard, false, 100.0, 50.0);

        device->AddSkybox("media/skybox/right.jpg",
                        "media/skybox/left.jpg",
                        "media/skybox/top.jpg",
                        "media/skybox/bottom.jpg",
                        "media/skybox/front.jpg",
                        "media/skybox/back.jpg");

        device->AddShadowMapping(light2->GetEntity()->GetID());

        static_cast<CLMesh*>(mesh1->GetEntity())->SetMesh(resourceMeshGround);
        static_cast<CLMesh*>(mesh2->GetEntity())->SetMesh(resourceMesh);
        static_cast<CLMesh*>(mesh3->GetEntity())->SetMesh(resourceMesh);

        camera->SetTranslation(glm::vec3(400.0f, 200.0f, -40.9f));
        light1->SetTranslation(glm::vec3(75.9f, 1000.2f, 15.08f));
        light2->SetTranslation(glm::vec3(295.9f, 300.2f, 15.08f));

        mesh1->SetScalation(glm::vec3(1.0f, 1.0f, 1.0f));
        mesh1->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh1->SetTranslation(glm::vec3(50.0f, 50.0f, 50.0f));

        mesh2->SetScalation(glm::vec3(10.5f, 10.5f, 10.5f));
        mesh2->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh2->SetTranslation(glm::vec3(50.0f, 80.0f, -50.0f));

        mesh3->SetScalation(glm::vec3(10.5f, 10.5f, 10.5f));
        mesh3->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh3->SetTranslation(glm::vec3(0, 10, 0));

       

        //LUCES Y COLORES
        float auxCameraPos[3] = {camera->GetTranslation().x, camera->GetTranslation().y, camera->GetTranslation().z};
        float auxLightPos[3]  = {light1->GetTranslation().x, light1->GetTranslation().y, light1->GetTranslation().z};
        float auxLightPos2[3] = {light2->GetTranslation().x, light2->GetTranslation().y, light2->GetTranslation().z};

        float index = 0.01;

        double previousTime = glfwGetTime();
        int frameCount = 0;
        //device->ImGuiInit();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(device->GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 450");
        ImGui::StyleColorsDark();

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        bool open = true;
        while (device->Run()) {
            //Apartir de aqui hacemos cosas, de momento en el main para testear
            device->UpdateViewport();  //Por si reescalamos la ventana
            device->BeginScene();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();            
            ImGui::Begin("Begin");
            if (ImGui::TreeNode("Translations"))
            {
                if (ImGui::CollapsingHeader("Transformables", ImGuiTreeNodeFlags_None))
                {
                    ImGui::SliderFloat3("CameraPos", auxCameraPos, -600, 600);
                    ImGui::SliderFloat3("LightPos", auxLightPos, -1000, 1000);
                    ImGui::SliderFloat3("LightPos2", auxLightPos2, -1000, 1000);
                }
               
                ImGui::TreePop();
                
            }
            ImGui::End();

            device->SetDrawLineWidth(6);
            device->Draw3DLine(0,0,0,0,100,0, CLColor(255,0,0,1));
            glm::vec3 cameraPos(auxCameraPos[0], auxCameraPos[1], auxCameraPos[2]);
            glm::vec3 lightPos(auxLightPos[0], auxLightPos[1], auxLightPos[2]);
            glm::vec3 lightPos2(auxLightPos2[0], auxLightPos2[1], auxLightPos2[2]);

            camera->SetTranslation(cameraPos);
            light1->SetTranslation(lightPos);
            light2->SetTranslation(lightPos2);

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_E)) {
                device->DeleteNode(mesh2);
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_Q)) {
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_W)) {
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_R)) {
            }


            // Measure speed
            double currentTime = glfwGetTime();
            frameCount++;

            // If a second has passed.
            if (currentTime - previousTime >= 1.0) {
                // Display the frame count here any way you want.
                device->SetTitle("BeastBrawl <" + to_string(frameCount) + ">");

                frameCount = 0;
                previousTime = currentTime;
            }

            device->DrawObjects();
            device->PollEvents();
            device->RenderImgui();
            device->EndScene();
            index += 0.2;
        }
        device->TerminateImGui();
        delete device;

    } catch (std::exception& ex) {
        cout << "Hubo una excepción " << ex.what() << endl;
    }
    return 0;
}
