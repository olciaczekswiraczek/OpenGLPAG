
#include <corecrt_math_defines.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "GraphNode.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const char* glsl_version = "#version 430";


int main()
{


    Window window(SCR_WIDTH, SCR_HEIGHT, "Menger Sponge");
    glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();



    std::shared_ptr<Shader> vertexShader(new Shader("shader.vert", VERTEX_SHADER));
    std::shared_ptr<Shader> fragmentShader(new Shader("shader.frag", FRAGMENT_SHADER));

    ShaderProgram shaderProgram(vertexShader, fragmentShader);

    Model* star = new Model("Death_Star.obj", &shaderProgram);
    Model* star2 = new Model("Death_Star.obj", &shaderProgram);

    Texture texture1("texture1.jpg","texture_diffuse");
    //Texture texture2("texture2.jpg");

    GraphNode* solarSystem = new GraphNode();
    GraphNode* starGraphNode = new GraphNode(star);
    GraphNode* star2GraphNode = new GraphNode(star2);

    // create graph nodes transformations to position them in the scene
// ----------------------------------------------------------------
    glm::mat4* transformStarGraphNode = new glm::mat4(1);
    *transformStarGraphNode = glm::translate(*(transformStarGraphNode), glm::vec3(0.0f, -2.75f, 0.0f));
    //*transformStarGraphNode = glm::scale(*transformStarGraphNode, glm::vec3(0.1f, 0.1f, 0.1f));

  


    star->setTransform(transformStarGraphNode);

    starGraphNode->addChild(star2GraphNode);
    solarSystem->addChild(starGraphNode);
    
    
   

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -200.0f)); //tak naprawde przesuwamy obiekty, a nie kamere
    //view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);

    // render loop
    while (window.isOpen())
    {
        glfwPollEvents(); //poll IO events (keys pressed/released, mouse moved etc.)

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        

        texture1.Use(0);
      
        //drawing
       
        //shaderProgram.setColor(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));  


        glm::mat4 model = glm::mat4(1.0f);
     
        // set projection and view matrix
        //-------------------------------
      
        shaderProgram.Use();
       glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getProgramID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
       // shaderProgram.setMat4(projection, "projection");
       // shaderProgram.setMat4(view, "view");

        starGraphNode->Rotate(2.3f, glm::vec3(0, 1, 0));
        
        star->Draw();

        solarSystem->Update();
        solarSystem->Draw();

     

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.getWindow());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

//obrot wokol osi z
    // model - glm::translate(model, glm::vec3(0.5f, 0.2f, 0.0)); //przesuwa pivot, kolejnosc transformacji ma znaczenie

     // render your GUI
    /* ImGui::Begin("Cube Rotation/Color");
     static int level;
     ImGui::SliderInt("Level", &level, 0, 3);

     static float rotationOX = 0.0f;
     ImGui::SliderFloat("Rotation OX", &rotationOX, 0.0f, 1.0f);
     model = glm::rotate(model, glm::radians(rotationOX * 360.f), glm::vec3(1.0f, 0.0f, 0.0f));

     static float rotationOY = 0.0f;
     ImGui::SliderFloat("Rotation OY", &rotationOY, 0.0f, 1.0f);
     model = glm::rotate(model, glm::radians(rotationOY * 360.f), glm::vec3(0.0f, 1.0f, 0.0f));

     static float color[4] = { 1.0f,1.0f,1.0f,1.0f };
     // color picker
     ImGui::ColorEdit3("color", color);
     // multiply triangle's color with this color
     shaderProgram.setColor(glm::vec4(color[0], color[1], color[2], 1.0f));
     ImGui::End();*/