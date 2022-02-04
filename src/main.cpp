
#include <corecrt_math_defines.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
#include "Light.h"


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


// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
   

    Window window(SCR_WIDTH, SCR_HEIGHT, "Zadanie 3");
    glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetScrollCallback(window.getWindow(), scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    std::shared_ptr<Shader> vertexShader(new Shader("shader.vert", VERTEX_SHADER));
    std::shared_ptr<Shader> fragmentShader(new Shader("shader.frag", FRAGMENT_SHADER));

    std::shared_ptr<Shader> lightingVertexShader(new Shader("light.vert", VERTEX_SHADER));
    std::shared_ptr<Shader> lightingFragmentShader(new Shader("light.frag", FRAGMENT_SHADER));

    std::shared_ptr<Shader> lightCubeVertexShader(new Shader("lightCube.vert", VERTEX_SHADER));
    std::shared_ptr<Shader> lightCubeFragmentShader(new Shader("lightCube.frag", FRAGMENT_SHADER));

   // ShaderProgram shaderProgram(vertexShader, fragmentShader);
    ShaderProgram lightingShaderProgram(lightingVertexShader, lightingFragmentShader);
    ShaderProgram lightCubeShaderProgram(lightCubeVertexShader, lightCubeFragmentShader);

    /*
    Model* star = new Model("res/models/Sun/Sun.obj", &shaderProgram);
    Model* planet1 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* planet2 = new Model("res/models/Mars/Mars.obj", &shaderProgram);
    Model* moon1 = new Model("res/models/Moon/Moon.obj", &shaderProgram);
    Model* moon2 = new Model("res/models/Death_Star/Death_Star.obj", &shaderProgram);

   
    GraphNode* solarSystem = new GraphNode();
    GraphNode* starGraphNode = new GraphNode(star);

    GraphNode* planet1GraphNode = new GraphNode(planet1);
    GraphNode* planet2GraphNode = new GraphNode(planet2);

    GraphNode* moon1GraphNode = new GraphNode(moon1);
    GraphNode* moon2GraphNode = new GraphNode(moon2);



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

 

    starGraphNode->setTransform(transformStarGraphNode);
    planet1GraphNode->setTransform(transformPlanet1GraphNode);
    planet2GraphNode->setTransform(transformPlanet2GraphNode);
    moon1GraphNode->setTransform(transformMoon1GraphNode);
    moon2GraphNode->setTransform(transformMoon2GraphNode);

 
    // ----------------------------------------------------------------
    planet1GraphNode->addChild(moon1GraphNode);
    planet1GraphNode->addChild(moon2GraphNode);

    starGraphNode->addChild(planet1GraphNode);
    starGraphNode->addChild(planet2GraphNode);

 
    solarSystem->addChild(starGraphNode);*/

 

    // set up vertex data (and buffer(s)) and configure vertex attributes
       // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    /*
    Light* light = new Light(vertices); 
    light->InitLightObject();

    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, light->getVBO());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);*/
    
   

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
            ImGui::Begin("Change lights");
           
            ImGui::End();
          
        }

        // input
        // -----
        processInput(window.getWindow());


        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderProgram.Use();
        lightingShaderProgram.setVec3("light.position", lightPos);
        lightingShaderProgram.setVec3("viewPos", camera.Position);

        // light properties
        glm::vec3 lightColor;
        lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
        lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
        lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        lightingShaderProgram.setVec3("light.ambient", ambientColor);
        lightingShaderProgram.setVec3("light.diffuse", diffuseColor);
        lightingShaderProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        lightingShaderProgram.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShaderProgram.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShaderProgram.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        lightingShaderProgram.setFloat("material.shininess", 32.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShaderProgram.setMat4("projection", projection);
        lightingShaderProgram.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShaderProgram.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // also draw the lamp object
        lightCubeShaderProgram.Use();
        lightCubeShaderProgram.setMat4("projection", projection);
        lightCubeShaderProgram.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShaderProgram.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);




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
      // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

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
