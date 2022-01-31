
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
    Model* planet1 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* planet2 = new Model("res/models/Mars/Mars.obj", &shaderProgram);
    Model* moon1 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* moon2 = new Model("res/models/Death_Star/Death_Star.obj", &shaderProgram);
    //shaderProgram.setColor

    Mesh* coneMesh = new Mesh();
    coneMesh->generateTorus(30, 30, 4.0f, 10.0f);
    Model* coneModel = new Model(coneMesh);
    coneModel->setShaderProgram(&coneShaderProgram);

   // Texture texture1("texture1.jpg","texture_diffuse");
    //Texture texture2("texture2.jpg");

    GraphNode* solarSystem = new GraphNode();
    GraphNode* starGraphNode = new GraphNode(star);

    GraphNode* planet1GraphNode = new GraphNode(planet1);
    GraphNode* planet2GraphNode = new GraphNode(planet2);

    GraphNode* moon1GraphNode = new GraphNode(moon1);
    GraphNode* moon2GraphNode = new GraphNode(moon2);

    GraphNode* coneGraphNode = new GraphNode(coneModel);


    // create graph nodes transformations to position them in the scene
// ----------------------------------------------------------------
    
    glm::mat4* transformStarGraphNode = new glm::mat4(1);
    *transformStarGraphNode = glm::translate(*(transformStarGraphNode), glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
    *transformStarGraphNode = glm::scale(*transformStarGraphNode, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

    glm::mat4* transformPlanet1GraphNode = new glm::mat4(1);
    *transformPlanet1GraphNode = glm::translate(*(transformPlanet1GraphNode), glm::vec3(30.0f, 9.0f, 0.0f));
    *transformPlanet1GraphNode = glm::scale(*transformPlanet1GraphNode, glm::vec3(0.1f, 0.1f, 0.1f));

    glm::mat4* transformPlanet2GraphNode = new glm::mat4(1);
	*transformPlanet2GraphNode = glm::translate(*(transformPlanet2GraphNode), glm::vec3(10.0f, 2.0f, 0.0f));
	*transformPlanet2GraphNode = glm::scale(*transformPlanet2GraphNode, glm::vec3(0.3f, 0.3f, 0.3f));

    glm::mat4* transformMoon1GraphNode = new glm::mat4(1);
    *transformMoon1GraphNode = glm::translate(*(transformMoon1GraphNode), glm::vec3(20.0f, -2.0f, 0.0f));
    *transformMoon1GraphNode = glm::scale(*transformMoon1GraphNode, glm::vec3(2.0f, 2.0f, 2.0f));

    glm::mat4* transformMoon2GraphNode = new glm::mat4(1);
    *transformMoon2GraphNode = glm::translate(*(transformMoon2GraphNode), glm::vec3(30.0f, -5.0f, 0.0f));
    *transformMoon2GraphNode = glm::scale(*transformMoon2GraphNode, glm::vec3(0.8f, 0.8f, 0.8f));

    glm::mat4* transformConeGraphNode = new glm::mat4(1);
    *transformConeGraphNode = glm::translate(*(transformConeGraphNode), glm::vec3(30.0f, 9.0f, 0.0f));
    *transformConeGraphNode = glm::scale(*transformConeGraphNode, glm::vec3(0.3f, 0.3f, 0.3f));
        

    starGraphNode->setTransform(transformStarGraphNode);
    planet1GraphNode->setTransform(transformPlanet1GraphNode);
    planet2GraphNode->setTransform(transformPlanet2GraphNode);
    moon1GraphNode->setTransform(transformMoon1GraphNode);
    moon2GraphNode->setTransform(transformMoon2GraphNode);

    coneGraphNode->setTransform(transformConeGraphNode);


    // ----------------------------------------------------------------
    //planet1GraphNode->addChild(moon1GraphNode);
   // planet1GraphNode->addChild(moon2GraphNode);

    //starGraphNode->addChild(planet1GraphNode);
   // starGraphNode->addChild(planet2GraphNode);

    starGraphNode->addChild(coneGraphNode);
    solarSystem->addChild(starGraphNode);

    
    
   

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);


   // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -500.0f)); //tak naprawde przesuwamy obiekty, a nie kamere
    //view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
  //  projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);


    int angle = 1;
    int angle2 = 1;
    int angle3 = 1;

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
            ImGui::SliderInt("", &NUMBER_OF_SEGMENTS, 7, 60);
            ImGui::Text("Number of rings");
            ImGui::SliderInt(" ", &NUMBER_OF_RINGS, 6, 60);
            ImGui::Text("Rotate root node:");
            ImGui::SliderInt("  ", &angle, 1, 360);
            ImGui::Text("Rotate root node:");
            ImGui::SliderInt("   ", &angle3, 1, 360);
            ImGui::Text("Rotate root node:");
            ImGui::SliderInt("    ", &angle2, 1, 360);
            if (ImGui::Button("Generate!"))
            {
                //torusMesh->generateTorus(NUMBER_OF_SEGMENTS, NUMBER_OF_RINGS, 4, 10);
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
        starGraphNode->Rotate(angle, glm::vec3(0, -1, 0));
     
        
        // set projection and view matrix
        //-------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();
       
     
     
        // set projection and view matrix
        //-------------------------------

        coneShaderProgram.Use();
        coneShaderProgram.setMat4(projection, "projection");
        coneShaderProgram.setMat4(view, "view");


        shaderProgram.Use();
        shaderProgram.setMat4(projection, "projection");
        shaderProgram.setMat4(view, "view");
       

        // rotate all graph nodes
        // ----------------------
        moon1GraphNode->Rotate(5.2f, glm::vec3(1, 1, 0));
        moon2GraphNode->Rotate(0.9f, glm::vec3(0, 1, 0));
        //deathStarGraphNode->Rotate(2.3f, glm::vec3(0, 1, 0));


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