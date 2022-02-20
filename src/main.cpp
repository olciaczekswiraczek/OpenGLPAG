
#include <corecrt_math_defines.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "GraphNode.h"
#include "Camera.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const char* glsl_version = "#version 430";
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool wireframeMode = false;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int NUMBER_OF_SEGMENTS = 30;
int NUMBER_OF_RINGS = 30;

int main()
{
   

    Window window(SCR_WIDTH, SCR_HEIGHT, "Zadanie 2");
    glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetScrollCallback(window.getWindow(), scroll_callback);

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

    std::shared_ptr<Shader> coneVertexShader(new Shader("cone.vert", VERTEX_SHADER));
    std::shared_ptr<Shader> coneFragmentShader(new Shader("cone.frag", FRAGMENT_SHADER));

    ShaderProgram shaderProgram(vertexShader, fragmentShader);
    ShaderProgram coneShaderProgram(coneVertexShader, coneFragmentShader);

    Model* star = new Model("res/models/Sun/Sun.obj", &shaderProgram);
    Model* planet1 = new Model("res/models/Neptune/Neptune.obj", &shaderProgram);
    Model* planet2 = new Model("res/models/Mars/Mars.obj", &shaderProgram);
    Model* planet3 = new Model("res/models/Jupiter/Jupiter.obj", &shaderProgram);
    Model* planet4 = new Model("res/models/Neptune/Neptune.obj", &shaderProgram);
    Model* moon1 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* moon2 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* moon3 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* moon4 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* moon5 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* moon6 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* moon7 = new Model("res/models/Moon/Moon.obj", &shaderProgram);

    Mesh* coneMesh = new Mesh();
    coneMesh->generateCone(20, 10.0f, 5.0f);
    Model* coneModel = new Model(coneMesh);
    coneModel->setShaderProgram(&coneShaderProgram);

    GraphNode* solarSystem = new GraphNode();
    GraphNode* starGraphNode = new GraphNode(star);

    GraphNode* planet1GraphNode = new GraphNode(planet1);
    GraphNode* planet2GraphNode = new GraphNode(planet2);
    GraphNode* planet3GraphNode = new GraphNode(planet3);
    GraphNode* planet4GraphNode = new GraphNode(planet4);
    GraphNode* coneGraphNode = new GraphNode(coneModel);

    GraphNode* handler1 = new GraphNode();
    GraphNode* handler2 = new GraphNode();
    GraphNode* handler3 = new GraphNode();
    GraphNode* handler4 = new GraphNode();
    GraphNode* handler5 = new GraphNode();
    GraphNode* handler6 = new GraphNode();
    GraphNode* handler7 = new GraphNode();

    GraphNode* moon1GraphNode = new GraphNode(moon1);
    GraphNode* moon2GraphNode = new GraphNode(moon2);
    GraphNode* moon3GraphNode = new GraphNode(moon3);
    GraphNode* moon4GraphNode = new GraphNode(moon4);
    GraphNode* moon5GraphNode = new GraphNode(moon5);
    GraphNode* moon6GraphNode = new GraphNode(moon6);
    GraphNode* moon7GraphNode = new GraphNode(moon7);



    // create graph nodes transformations to position them in the scene
// ----------------------------------------------------------------
    
    glm::mat4* transformStarGraphNode = new glm::mat4(1);
    *transformStarGraphNode = glm::translate(*(transformStarGraphNode), glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
    *transformStarGraphNode = glm::scale(*transformStarGraphNode, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down

    glm::mat4* transformPlanet1GraphNode = new glm::mat4(1);
    *transformPlanet1GraphNode = glm::translate(*(transformPlanet1GraphNode), glm::vec3(0.0f, 0.0f, 10.0f));
    *transformPlanet1GraphNode = glm::rotate(*(transformPlanet1GraphNode), 10.2f, glm::vec3(1, 0, 0));
    *transformPlanet1GraphNode = glm::scale(*transformPlanet1GraphNode, glm::vec3(0.8f));

    glm::mat4* transformPlanet2GraphNode = new glm::mat4(1);
	*transformPlanet2GraphNode = glm::translate(*(transformPlanet2GraphNode), glm::vec3(-20.0f, 0.0f, 0.0f));
    *transformPlanet2GraphNode = glm::rotate(*(transformPlanet1GraphNode), -5.2f, glm::vec3(1, 0, 0));
	*transformPlanet2GraphNode = glm::scale(*transformPlanet2GraphNode, glm::vec3(0.8f));

    glm::mat4* transformPlanet3GraphNode = new glm::mat4(1);
    *transformPlanet3GraphNode = glm::translate(*(transformPlanet3GraphNode), glm::vec3(32.0f, 0.0f, 0.0f));
    *transformPlanet2GraphNode = glm::rotate(*(transformPlanet1GraphNode), 7.5f, glm::vec3(1, 0, 0));
    *transformPlanet3GraphNode = glm::scale(*transformPlanet3GraphNode, glm::vec3(0.6f));

    glm::mat4* transformPlanet4GraphNode = new glm::mat4(1);
    *transformPlanet4GraphNode = glm::translate(*(transformPlanet4GraphNode), glm::vec3(0.0f, 0.0f, -45.0f));
    *transformPlanet2GraphNode = glm::rotate(*(transformPlanet1GraphNode), -8.7f, glm::vec3(1, 0, 0));
    *transformPlanet4GraphNode = glm::scale(*transformPlanet4GraphNode, glm::vec3(0.5f));

    glm::mat4* transformConeGraphNode = new glm::mat4(1);
    *transformConeGraphNode = glm::translate(*(transformConeGraphNode), glm::vec3(60.0f, 0.0f, 0.0f));
    *transformConeGraphNode = glm::scale(*transformConeGraphNode, glm::vec3(0.3f));

    glm::mat4* transformMoon1GraphNode = new glm::mat4(1);
    *transformMoon1GraphNode = glm::translate(*(transformMoon1GraphNode), glm::vec3(4.0f, 0.0f, 0.0f));
    *transformMoon1GraphNode = glm::scale(*transformMoon1GraphNode, glm::vec3(0.07f));

    glm::mat4* transformMoon2GraphNode = new glm::mat4(1);
    *transformMoon2GraphNode = glm::translate(*(transformMoon2GraphNode), glm::vec3(0.0f, 0.0f, 7.0f));
    *transformMoon2GraphNode = glm::scale(*transformMoon2GraphNode, glm::vec3(0.05));

    glm::mat4* transformMoon3GraphNode = new glm::mat4(1);
    *transformMoon3GraphNode = glm::translate(*(transformMoon3GraphNode), glm::vec3(3.0f, 0.0f, 2.0f));
    *transformMoon3GraphNode = glm::scale(*transformMoon3GraphNode, glm::vec3(0.08f));

    glm::mat4* transformMoon4GraphNode = new glm::mat4(1);
    *transformMoon4GraphNode = glm::translate(*(transformMoon4GraphNode), glm::vec3(-7.0f, 0.0f, 0.0f));
    *transformMoon4GraphNode = glm::scale(*transformMoon4GraphNode, glm::vec3(0.09));
    
    glm::mat4* transformMoon5GraphNode = new glm::mat4(1);
    *transformMoon5GraphNode = glm::translate(*(transformMoon5GraphNode), glm::vec3(0.0f, 0.0f, -11.0f));
    *transformMoon5GraphNode = glm::scale(*transformMoon5GraphNode, glm::vec3(0.05));

    glm::mat4* transformMoon6GraphNode = new glm::mat4(1);
    *transformMoon6GraphNode = glm::translate(*(transformMoon6GraphNode), glm::vec3(8.0f, 0.0f, 0.0f));
    *transformMoon6GraphNode = glm::scale(*transformMoon6GraphNode, glm::vec3(0.1));

    glm::mat4* transformMoon7GraphNode = new glm::mat4(1);
    *transformMoon7GraphNode = glm::translate(*(transformMoon7GraphNode), glm::vec3(14.0f, 0.0f, 0.0f));
    *transformMoon7GraphNode = glm::scale(*transformMoon7GraphNode, glm::vec3(0.07));



    starGraphNode->setTransform(transformStarGraphNode);

    

    planet1GraphNode->setTransform(transformPlanet1GraphNode);
    planet2GraphNode->setTransform(transformPlanet2GraphNode);
    planet3GraphNode->setTransform(transformPlanet3GraphNode);
    planet4GraphNode->setTransform(transformPlanet4GraphNode);

    moon1GraphNode->setTransform(transformMoon1GraphNode);
    moon2GraphNode->setTransform(transformMoon2GraphNode);
    moon3GraphNode->setTransform(transformMoon3GraphNode);
    moon4GraphNode->setTransform(transformMoon4GraphNode);
    moon5GraphNode->setTransform(transformMoon5GraphNode);
    moon6GraphNode->setTransform(transformMoon6GraphNode);
    moon7GraphNode->setTransform(transformMoon7GraphNode);

    coneGraphNode->setTransform(transformConeGraphNode);
  /*
    handler1->addChild(moon1GraphNode);
    handler1->addOrbit(5.0, &coneShaderProgram, 180, 0.0f);
    handler2->addChild(moon2GraphNode);
    handler3->addChild(moon3GraphNode);
    handler4->addChild(moon4GraphNode);
    handler5->addChild(moon5GraphNode);
    handler6->addChild(moon6GraphNode);
    handler7->addChild(moon6GraphNode);

    planet1GraphNode->addChild(handler1);
    planet1GraphNode->addChild(handler2);
    planet2GraphNode->addChild(handler3);
    planet3GraphNode->addChild(handler4);
    planet4GraphNode->addChild(handler5);
    planet4GraphNode->addChild(handler6);
    planet4GraphNode->addChild(handler7);*/

    planet1GraphNode->addChild(moon1GraphNode);
    planet1GraphNode->addOrbit(4.0, &coneShaderProgram, 180, 0.0f);
    planet1GraphNode->addChild(moon2GraphNode);
    planet1GraphNode->addOrbit(7.0, &coneShaderProgram, 180, 0.0f);
    planet2GraphNode->addChild(moon3GraphNode);
    planet2GraphNode->addOrbit(3.0, &coneShaderProgram, 180, 0.0f);
    planet3GraphNode->addChild(moon4GraphNode);
    planet3GraphNode->addOrbit(7.0, &coneShaderProgram, 180, 0.0f);
    planet4GraphNode->addChild(moon5GraphNode);
    planet4GraphNode->addOrbit(11.0, &coneShaderProgram, 180, 0.0f);
    planet4GraphNode->addChild(moon6GraphNode);
    planet4GraphNode->addOrbit(8.0, &coneShaderProgram, 180, 0.0f);
    planet4GraphNode->addChild(moon7GraphNode);
    planet4GraphNode->addOrbit(14.0, &coneShaderProgram, 180, 0.0f);

    starGraphNode->addChild(planet1GraphNode);
    starGraphNode->addOrbit(10.0, &coneShaderProgram, 360, 0.0f);
    starGraphNode->addChild(planet2GraphNode);
    starGraphNode->addOrbit(20.0, &coneShaderProgram, 720, 0.0f);
    starGraphNode->addChild(planet3GraphNode);
    starGraphNode->addOrbit(32.0, &coneShaderProgram, 1080, 0.0f);
    starGraphNode->addChild(planet4GraphNode);
    starGraphNode->addOrbit(45.0, &coneShaderProgram, 1340, 0.0f);
    starGraphNode->addChild(coneGraphNode);
    starGraphNode->addOrbit(60.0, &coneShaderProgram, 1700, 0.0f);

    solarSystem->addChild(starGraphNode);


    // configure global opengl state
    glEnable(GL_DEPTH_TEST);


   // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -500.0f)); //tak naprawde przesuwamy obiekty, a nie kamere
    //view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
  //  projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);


    int angle = 1;
    int angle2 = 0;
    int angle3 = 0;

    // render loop
    while (window.isOpen())
    {
        // per-frame time logic
       // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetWindowPos(ImVec2(60, 60));
            ImGui::SetWindowSize(ImVec2(250, 100));
            ImGui::Begin("Adjustments");
            ImGui::Text("Number of segments");
            ImGui::SliderInt("", &NUMBER_OF_SEGMENTS, 3, 60);
            ImGui::Text("Rotate root node:");
            ImGui::SliderInt("  ", &angle, -10, 10);
            ImGui::Text("Rotate root node:");
            ImGui::SliderInt("   ", &angle3, -10, 10);
            ImGui::Text("Rotate root node:");
            ImGui::SliderInt("    ", &angle2, -10, 10);
            if (ImGui::Button("Generate!"))
            {
                coneMesh->generateCone(NUMBER_OF_SEGMENTS, 8, 4);
            }
            if (ImGui::Checkbox("Wireframe mode", &wireframeMode)) {

                if (wireframeMode) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            ImGui::End();
          
        }

        

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        processInput(window.getWindow());
        starGraphNode->Rotate(angle, glm::vec3(0, 1, 0));
        starGraphNode->Rotate(angle2, glm::vec3(1, 0, 0));
        starGraphNode->Rotate(angle3, glm::vec3(0, 0, 1));
     
        
        // set projection and view matrix
        //-------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();
       
     
     
        // set projection and view matrix
        //-------------------------------

        coneShaderProgram.Use();
        coneShaderProgram.setMat4("projection",projection);
        coneShaderProgram.setMat4("view", view);


        shaderProgram.Use();
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", view);
       

        // rotate all graph nodes
        // ----------------------

        
        planet1GraphNode->Rotate(2.5f, glm::vec3(0, 1, 0));
        planet2GraphNode->Rotate(4.3f, glm::vec3(0, 1, 0));
        planet3GraphNode->Rotate(1.5f, glm::vec3(0, 1, 0));
        planet4GraphNode->Rotate(3.1f, glm::vec3(0, 1, 0));
        coneGraphNode->Rotate(1.2f, glm::vec3(0, 1, 0));

        moon1GraphNode->Rotate(1.2f, glm::vec3(1, 1, 0));
        moon2GraphNode->Rotate(0.9f, glm::vec3(0, 1, 0));
        moon3GraphNode->Rotate(2.3f, glm::vec3(0, 1, 0));
        moon4GraphNode->Rotate(0.5f, glm::vec3(0, 1, 0));
        moon5GraphNode->Rotate(2.9f, glm::vec3(0, 1, 0));
        moon6GraphNode->Rotate(1.9f, glm::vec3(0, 1, 0));

        

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians((float)angle), glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, glm::radians((float)angle2), glm::vec3(0.0, 1.0, 0.0));
        model = glm::rotate(model, glm::radians((float)angle3), glm::vec3(0.0, 0.0, 1.0));
     
        star->setTransform(&model);


        solarSystem->Update();
        solarSystem->Draw();

     

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents(); //poll IO events (keys pressed/released, mouse moved etc.)
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)) {

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
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