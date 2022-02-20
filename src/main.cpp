
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


class Movement
{
public:
    float aPos;
    float speed;
    float distance;
    float x;
    float y;

    Movement(float aPos, float speed, float distance)
    {
        this->aPos = aPos;
        this->speed = speed;
        this->distance = distance;
    }

    void Update()
    {
        if (aPos + speed > 360.0f)
        {
            aPos = 0;
        }
        else
            aPos += speed;

        this->x = distance * glm::cos(glm::radians(aPos));
        this->y = distance * glm::sin(glm::radians(aPos));
    }
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
unsigned int loadCubeMap(std::vector<std::string> faces);

unsigned int generateInstanceVBO(unsigned int amount, glm::mat4* matrices, Model* loadedModel) {
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < loadedModel->m_meshes.size(); i++)
    {
        //instanced array
        unsigned int VAO = loadedModel->m_meshes.at(i)->getVAO();
        glBindVertexArray(VAO);


        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
    return buffer;
}

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

    std::shared_ptr<Shader> skyboxVertexShader(new Shader("skybox.vert", VERTEX_SHADER));
    std::shared_ptr<Shader> skyboxFragmentShader(new Shader("skybox.frag", FRAGMENT_SHADER));

    std::shared_ptr<Shader> lightingVertexShader(new Shader("light.vert", VERTEX_SHADER));
    std::shared_ptr<Shader> lightingFragmentShader(new Shader("light.frag", FRAGMENT_SHADER));

    std::shared_ptr<Shader> lightCubeVertexShader(new Shader("lightCube.vert", VERTEX_SHADER));
    std::shared_ptr<Shader> lightCubeFragmentShader(new Shader("lightCube.frag", FRAGMENT_SHADER));

    ShaderProgram shaderProgram(vertexShader, fragmentShader);
    ShaderProgram skyboxShaderProgram(skyboxVertexShader, skyboxFragmentShader);
    ShaderProgram lightingShaderProgram(lightingVertexShader, lightingFragmentShader);
    ShaderProgram lightCubeShaderProgram(lightCubeVertexShader, lightCubeFragmentShader);

    Model* houseModel = new Model("res/models/Cube/Cube.obj", &lightingShaderProgram);
    Model* tipModel = new Model("res/models/Tip/tip.obj", &lightingShaderProgram);
    Model* planeModel = new Model("res/models/plane/plane.obj", &lightingShaderProgram);

    Movement pointLightMovement = Movement(40.0f, 1.5f, 25.0f);

    std::vector<GraphNode*> houseGraphNodes;
    std::vector<GraphNode*> roofGraphNodes;

    //macierz transformacji instancji
    unsigned int dim = 200;
    unsigned int amount = dim * dim;
    glm::mat4* houseMatrices = new glm::mat4[amount];
    glm::mat4* roofMatrices = new glm::mat4[amount];
    glm::mat4* planeMatrix = new glm::mat4(1);

    GraphNode* world = new GraphNode();
    //graf sceny
    GraphNode* root = new GraphNode(NULL, NULL, glm::mat4(1));

    GraphNode* planeGraphNode = new GraphNode(root, planeMatrix, glm::mat4(1));

    //root->addChild(planeGraphNode);

    // generate a large list of semi-random model transformation matrices
 // ------------------------------------------------------------------



    float x = 0;
    float y = 0;
    float z = 0;

    srand(static_cast<unsigned int>(glfwGetTime()));

    for (unsigned int i = 0; i < dim; i++)
    {


        for (unsigned int j = 0; j < dim; j++)
        {
            

            /* GraphNode* wallsGraphNode = new GraphNode();
             GraphNode* roofGraphNode = new GraphNode();
             glm::mat4 model = glm::mat4(1.0f);
             glm::mat4 modelRoof = glm::mat4(1.0f);*/

             if (j % 2 == 0)
             {
                 z = j * (-2.0f);
             }
             else
                 z = j * (2.0f);

             if (i % 2 == 0)
             {
                 x = i * (-2.0f);
             }
             else
                 x = i * (2.0f);

             GraphNode* houseNode = new GraphNode(root, &houseMatrices[i * dim + j], glm::mat4(1));
             //houseNode->Translate((i - (float)dim / 2) * 5, 1, (j - (float)dim / 2) * 5);
             houseNode->Translate(x, 0.0f, z);
             GraphNode* roofNode = new GraphNode(houseNode, &roofMatrices[i * dim + j], glm::mat4(1));
             roofNode->Translate(0, 1.0, 0);
             roofNode->Scale(glm::vec3(1.2, 1.0, 1.2));

        }



    }

    glm::mat4 planeMat = glm::mat4(1.0f);
    planeMat = glm::translate(planeMat, glm::vec3(0.0f, -0.5f, 0.0f));
    planeMat = glm::scale(planeMat, glm::vec3(40.0f));

    unsigned int planeBuffer = generateInstanceVBO(1, &planeMat, planeModel);



    std::cout << "Graph nodes size =  " << houseGraphNodes.size() << std::endl;
    std::cout << "Graph nodes tip size =  " << roofGraphNodes.size() << std::endl;


    world->addChild(root);


    //std::cout << "House children: " << houseGraphNodes.at(1)->getChildren().size() << std::endl;
    std::cout << "root children: " << root->getChildren().size() << std::endl;
    //std::cout << "world children: " << world->getChildren().size() << std::endl;

    unsigned int houseBuffer = generateInstanceVBO(amount, houseMatrices, houseModel);
    unsigned int tipBuffer = generateInstanceVBO(amount, roofMatrices, tipModel);





    // set up vertex data (and buffer(s)) and configure vertex attributes
       // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


    // positions of the point lights
    glm::vec3 lightsPositions[] = {
        glm::vec3(10.0f,  7.0f,  -5.0f), //point
        glm::vec3(10.0f,  8.0f, -7.0f),    //spot1
        glm::vec3(-4.0f,  10.0f, -12.0f),  //spot2
        //glm::vec3(0.0f,  10.0f, -3.0f)    //spot2
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    std::vector<std::string> faces
    {
         ("res/textures/skybox/right.jpg"),
         ("res/textures/skybox/left.jpg"),
         ("res/textures/skybox/top.jpg"),
         ("res/textures/skybox/bottom.jpg"),
         ("res/textures/skybox/front.jpg"),
         ("res/textures/skybox/back.jpg"),
    };

    unsigned int cubemapTexture = loadCubeMap(faces);


    int angle = 1;
    int angle2 = 1;
    int angle3 = 1;

    bool dirLightFlag = true;
    bool pointLightFlag = true;
    bool spotLight1Flag = true;
    bool spotLight2Flag = true;

    float dirLightDirX = -0.0f;
    float dirLightDirY = -1.0f;
    float dirLightDirZ = -0.0f;
    float dirLightAmbient[3] = { 0.7f, 0.7f, 0.7f };
    float dirLightSpecular[3] = { 0.7f, 0.7f, 0.7f };
    float dirLightDiffuse[3] = { 0.7f, 0.7f, 0.7f };


    float pointLightAmbient[3] = { 0.7f, 0.7f, 0.7f };
    float pointLightSpecular[3] = { 0.7f, 0.7f, 0.7f };
    float pointLightDiffuse[3] = { 0.7f, 0.7f, 0.7f };

    float spotLightAmbient[3] = { 0.9f, 0.9f, 0.9f };
    float spotLightSpecular[3] = { 0.9f, 0.9f, 0.9f };
    float spotLightDiffuse[3] = { 0.9f, 0.9f, 0.9f };

    float ambient[3] = { 0.7f, 0.7f, 0.7f };
    float specular[3] = { 0.7f, 0.7f, 0.7f };
    float diffuse[3] = { 0.7f, 0.7f, 0.7f };

    float constant = 1.0f;
    float linear = 0.01;
    float quadratic = 0.01;
    bool* k = new bool(true);

    bool rotateRoot = false;

    std::cout << "Texturki: " << houseModel->textures_loaded.size();
    std::cout << "Texturki: " << houseModel->textures_loaded[0]->getType();
    std::cout << "Texturki: " << houseModel->textures_loaded[0]->getTextureID();
    std::cout << "Texturki: " << houseModel->textures_loaded[1]->getType();
    std::cout << "Texturki: " << houseModel->textures_loaded[1]->getTextureID();

    std::cout << "Sciezka: " << houseModel->textures_loaded[0]->getPath();

    lightingShaderProgram.setInt("texture_diffuse1", 0);
    lightingShaderProgram.setInt("texture_specular1", 1);

    skyboxShaderProgram.Use();
    skyboxShaderProgram.setInt("skybox", 0);

    // render loop
    while (window.isOpen())
    {

        // input
        // -----
        processInput(window.getWindow());



        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

            ImGui::Checkbox("Directional Light", &dirLightFlag);
            ImGui::SliderFloat("Directional Light X", &dirLightDirX, -1, 1);
            ImGui::SliderFloat("Directional Light Y", &dirLightDirY, -1, 1);
            ImGui::SliderFloat("Directional Light Z", &dirLightDirZ, -1, 1);
            ImGui::ColorEdit3("Directional Light Color", dirLightAmbient);

            ImGui::Checkbox("Pointlight", &pointLightFlag);
            ImGui::ColorEdit3("Pointlight Color", pointLightAmbient);

            ImGui::Checkbox("Spotlight 1", &spotLight1Flag);
            ImGui::Checkbox("Spotlight 2", &spotLight2Flag);
            ImGui::ColorEdit3("Spotlight 1 Color", spotLightAmbient);

            // ImGui::ColorEdit3("Spotlight 2 Color", spotambient1);
             //

            ImGui::Checkbox("Rotate root", &rotateRoot);

            ImGui::End();

        }

         if (rotateRoot) {
           
            root->Rotate(glm::vec3(0.0f, 0.02f, 0.0f));
            //world->Update();
            
        }

         root->Update();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        //aktualizacja transformacji w?z?ów grafu sceny
        

        // shader configuration
// --------------------


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderProgram.Use();
        lightingShaderProgram.setVec3("viewPos", camera.Position);
        lightingShaderProgram.setFloat("material.shininess", 32.0f);


        /*
          Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
          the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
          by defining light types as classes and set their values in there, or by using a more efficient uniform approach
          by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
       */
       // directional light
        lightingShaderProgram.setBool("dirLights[0].flag", dirLightFlag);
        lightingShaderProgram.setVec3("dirLights[0].direction", dirLightDirX, dirLightDirY, dirLightDirZ);
        lightingShaderProgram.setVec3("dirLights[0].ambient", dirLightAmbient[0], dirLightAmbient[1], dirLightAmbient[2]); // , 0.01f, 0.01f, 0.01f);
        lightingShaderProgram.setVec3("dirLights[0].diffuse", 0.3f, 0.3f, 0.3f);
        lightingShaderProgram.setVec3("dirLights[0].specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShaderProgram.setBool("pointLights[0].flag", pointLightFlag);
        lightingShaderProgram.setVec3("pointLights[0].position", lightsPositions[0].x, lightsPositions[0].y, lightsPositions[0].z);
        lightingShaderProgram.setVec3("pointLights[0].ambient", pointLightAmbient[0], pointLightAmbient[1], pointLightAmbient[2]);
        lightingShaderProgram.setVec3("pointLights[0].diffuse", diffuse[0], diffuse[1], diffuse[2]);
        lightingShaderProgram.setVec3("pointLights[0].specular", specular[0], specular[1], specular[2]);
        lightingShaderProgram.setFloat("pointLights[0].constant", constant);
        lightingShaderProgram.setFloat("pointLights[0].linear", linear);
        lightingShaderProgram.setFloat("pointLights[0].quadratic", quadratic);

        // spotLight 1
        lightingShaderProgram.setBool("spotLights[0].flag", spotLight1Flag);
        lightingShaderProgram.setVec3("spotLights[0].position", lightsPositions[1].x, lightsPositions[1].y, lightsPositions[1].z);
        lightingShaderProgram.setVec3("spotLights[0].direction", glm::vec3(0.0f, -2.5f, -1.0f));// glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));
        lightingShaderProgram.setVec3("spotLights[0].ambient", spotLightAmbient[0], spotLightAmbient[1], spotLightAmbient[2]);
        lightingShaderProgram.setVec3("spotLights[0].diffuse", spotLightDiffuse[0], spotLightDiffuse[1], spotLightDiffuse[2]);
        lightingShaderProgram.setVec3("spotLights[0].specular", spotLightSpecular[0], spotLightSpecular[1], spotLightSpecular[2]);
        lightingShaderProgram.setFloat("spotLights[0].constant", 1.0f);
        lightingShaderProgram.setFloat("spotLights[0].linear", 0.02);
        lightingShaderProgram.setFloat("spotLights[0].quadratic", 0.02);
        lightingShaderProgram.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
        lightingShaderProgram.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));

        //spotlight 2
        lightingShaderProgram.setBool("spotLights[1].flag", spotLight2Flag);
        lightingShaderProgram.setVec3("spotLights[1].position", lightsPositions[2].x, lightsPositions[2].y, lightsPositions[2].z);
        lightingShaderProgram.setVec3("spotLights[1].direction", glm::vec3(0.0f, -2.5f, -2.0f));
        lightingShaderProgram.setVec3("spotLights[1].ambient", spotLightAmbient[0], spotLightAmbient[1], spotLightAmbient[2]);
        lightingShaderProgram.setVec3("spotLights[1].diffuse", spotLightDiffuse[0], spotLightDiffuse[1], spotLightDiffuse[2]);
        lightingShaderProgram.setVec3("spotLights[1].specular", spotLightSpecular[0], spotLightSpecular[1], spotLightSpecular[2]);
        lightingShaderProgram.setFloat("spotLights[1].constant", 1.0f);
        lightingShaderProgram.setFloat("spotLights[1].linear", 0.02);
        lightingShaderProgram.setFloat("spotLights[1].quadratic", 0.02);
        lightingShaderProgram.setFloat("spotLights[1].cutOff", glm::cos(glm::radians(12.5f)));
        lightingShaderProgram.setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(15.0f)));


        lightingShaderProgram.setMat4("projection", projection);
        lightingShaderProgram.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShaderProgram.setMat4("model", model);


        // also draw the lamp object(s)
        lightCubeShaderProgram.Use();
        lightCubeShaderProgram.setMat4("projection", projection);
        lightCubeShaderProgram.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        if (dirLightFlag) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightsPositions[3]);
            model = glm::scale(model, glm::vec3(0.2f));
            model = glm::rotate(model, glm::radians((float)180.0f), glm::vec3(dirLightDirX, dirLightDirY, dirLightDirZ));
            lightCubeShaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        if (pointLightFlag)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightsPositions[0]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        if (spotLight1Flag)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightsPositions[1]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        if (spotLight2Flag)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightsPositions[2]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        pointLightMovement.Update();
        lightsPositions[0].x = pointLightMovement.x / 5.0f; 
        lightsPositions[0].z = pointLightMovement.y / 5.0f;

        lightingShaderProgram.Use();
        glBindBuffer(GL_ARRAY_BUFFER, houseBuffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &houseMatrices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //  houseModel->textures_loaded[0]->Use(1);
        //  houseModel->textures_loaded[1]->Use(2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, houseModel->textures_loaded[0]->getTextureID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, houseModel->textures_loaded[1]->getTextureID());

        for (unsigned int i = 0; i < houseModel->m_meshes.size(); i++)
        {

            glBindVertexArray(houseModel->m_meshes.at(i)->getVAO());
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(houseModel->m_meshes.at(i)->m_elementBuffer.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }


        glBindBuffer(GL_ARRAY_BUFFER, tipBuffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &roofMatrices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tipModel->textures_loaded[0]->getTextureID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tipModel->textures_loaded[1]->getTextureID());

        for (unsigned int i = 0; i < tipModel->m_meshes.size(); i++)
        {

            glBindVertexArray(tipModel->m_meshes.at(i)->getVAO());
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(tipModel->m_meshes.at(i)->m_elementBuffer.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }

        glBindBuffer(GL_ARRAY_BUFFER, planeBuffer);
        glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(glm::mat4), &planeMat[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planeModel->textures_loaded[0]->getTextureID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, planeModel->textures_loaded[1]->getTextureID());

        for (unsigned int i = 0; i < planeModel->m_meshes.size(); i++)
        {

            glBindVertexArray(planeModel->m_meshes.at(i)->getVAO());
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(planeModel->m_meshes.at(i)->m_elementBuffer.size()), GL_UNSIGNED_INT, 0, 1);
            glBindVertexArray(0);
        }

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShaderProgram.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShaderProgram.setMat4("view", view);
        skyboxShaderProgram.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default


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

unsigned int loadCubeMap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
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
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}