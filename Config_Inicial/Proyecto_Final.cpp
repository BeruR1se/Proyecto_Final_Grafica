//Proyecto Final
//Equipo: 3
//Números de Cuenta:319306972, 319039627, 314639136
//fecha de entrega: 12/11/2025

#include <iostream>
#include <cmath>
#include <vector>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// SOIL2
#include "SOIL2/SOIL2.h"

// Own
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// protos
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

// point light positions
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,2.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f,  0.0f),
    glm::vec3(0.0f,0.0f, 0.0f)
};

// cube for lamp
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

// delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// =================  estructura completa de spotlight =================
struct SpotInfo {
    glm::vec3 position;
    glm::vec3 direction;
    float innerCutoffDeg;
    float outerCutoffDeg;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

// helpers Assimp
glm::mat4 AiToGlm(const aiMatrix4x4& m)
{
    return glm::mat4(
        m.a1, m.b1, m.c1, m.d1,
        m.a2, m.b2, m.c2, m.d2,
        m.a3, m.b3, m.c3, m.d3,
        m.a4, m.b4, m.c4, m.d4
    );
}

const aiNode* FindNodeByName(const aiNode* node, const std::string& name)
{
    if (name == node->mName.C_Str())
        return node;

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        const aiNode* found = FindNodeByName(node->mChildren[i], name);
        if (found) return found;
    }
    return nullptr;
}

aiMatrix4x4 GetNodeGlobalTransform(const aiNode* node)
{
    aiMatrix4x4 tr = node->mTransformation;
    const aiNode* parent = node->mParent;
    while (parent)
    {
        tr = parent->mTransformation * tr;
        parent = parent->mParent;
    }
    return tr;
}

// ================= carga de spots desde FBX =================
std::vector<SpotInfo> LoadSpotLightsFromFBX(const std::string& path)
{
    std::vector<SpotInfo> spots;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | aiProcess_FlipUVs
    );

    if (!scene) {
        std::cout << "Error al cargar FBX: " << importer.GetErrorString() << std::endl;
        return spots;
    }

    if (!scene->HasLights()) {
        std::cout << "El FBX no tiene luces" << std::endl;
        return spots;
    }

    for (unsigned int i = 0; i < scene->mNumLights; i++) {
        aiLight* light = scene->mLights[i];

        if (light->mType == aiLightSource_SPOT) {
            // buscar nodo
            const aiNode* lightNode = FindNodeByName(scene->mRootNode, light->mName.C_Str());

            aiMatrix4x4 globalAi;
            if (lightNode)
                globalAi = GetNodeGlobalTransform(lightNode);
            else
                globalAi = aiMatrix4x4(); // identidad

            glm::mat4 global = AiToGlm(globalAi);

            // posición: columna 3
            glm::vec3 pos = glm::vec3(global[3]);

            // dirección: eje Z invertido
            glm::vec3 zAxis = glm::vec3(global[2]);
            glm::vec3 dir = -glm::normalize(zAxis);

            SpotInfo s;
            s.position = pos;
            s.direction = dir;

            // ángulos desde blender (radianes)
            s.innerCutoffDeg = glm::degrees(light->mAngleInnerCone);
            s.outerCutoffDeg = glm::degrees(light->mAngleOuterCone);

            // colores desde blender
            s.ambient = glm::vec3(light->mColorAmbient.r,
                light->mColorAmbient.g,
                light->mColorAmbient.b);
            s.diffuse = glm::vec3(light->mColorDiffuse.r,
                light->mColorDiffuse.g,
                light->mColorDiffuse.b);
            s.specular = glm::vec3(light->mColorSpecular.r,
                light->mColorSpecular.g,
                light->mColorSpecular.b);

            // atenuación desde blender, con fallback
            s.constant = (light->mAttenuationConstant != 0.0f) ? light->mAttenuationConstant : 1.0f;
            s.linear = (light->mAttenuationLinear != 0.0f) ? light->mAttenuationLinear : 0.09f;
            s.quadratic = (light->mAttenuationQuadratic != 0.0f) ? light->mAttenuationQuadratic : 0.032f;

            spots.push_back(s);

            std::cout << "Spot importado en "
                << s.position.x << " " << s.position.y << " " << s.position.z
                << " dir " << s.direction.x << " " << s.direction.y << " " << s.direction.z
                << " dif (" << s.diffuse.r << "," << s.diffuse.g << "," << s.diffuse.b << ")"
                << std::endl;
        }
    }

    return spots;
}

int main()
{
    // Init GLFW
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // ====== tus modelos ======
    Model Interior((char*)"Models/museocompleto.obj");
    Model Cuadro1((char*)"Models/Geometric_Balance_1102172132_texture.obj");
    Model Cuadro2((char*)"Models/Colorful_Geometry_1102172936_texture.obj");
    Model Pieza1((char*)"Models/Abstract_Harmony_1102171507_texture.obj");
    Model Pieza2((char*)"Models/Elegance_in_Red_1102171459_texture.obj");
    Model Pieza3((char*)"Models/Elegance_in_Stone_1102171426_texture.obj");
    // cuadros
    Model cuadro3((char*)"Models/cuadronuevo1.obj");
    Model cuadro4((char*)"Models/cuadronuevo2.obj");
    Model cuadro5((char*)"Models/cuadronuevo3.obj");
    Model cuadro6((char*)"Models/cuadronuevo4.obj");
    Model cuadro7((char*)"Models/cuadronuevo5.obj");
    Model cuadro8((char*)"Models/cuadronuevo6.obj");
    Model cuadro9((char*)"Models/cuadronuevo7.obj");
    Model cuadro10((char*)"Models/cuadronuevo8.obj");
    Model cuadro11((char*)"Models/cuadronuevo9.obj");
    Model cuadro12((char*)"Models/cuadronuevo10.obj");
    Model cuadro13((char*)"Models/cuadronuevo11.obj");
    Model cuadro14((char*)"Models/cuadronuevo12.obj");
    Model cuadro15((char*)"Models/cuadronuevo13.obj");
    Model cuadro16((char*)"Models/cuadronuevo14.obj");
    Model cuadro17((char*)"Models/cuadronuevo15.obj");
    Model cuadro18((char*)"Models/cuadronuevo16.obj");
    // estatuas
    Model pieza4((char*)"Models/estatuanueva1.obj");
    Model pieza5((char*)"Models/estatuanueva2.obj");
    Model pieza6((char*)"Models/estatuanueva3.obj");
    Model pieza7((char*)"Models/estatuanueva4.obj");
    Model pieza8((char*)"Models/estatuanueva5.obj");
    Model pieza9((char*)"Models/estatuanueva6.obj");
    Model pieza10((char*)"Models/estatuanueva7.obj");
    Model pieza11((char*)"Models/estatuanueva8.obj");
    Model pieza12((char*)"Models/estatuanueva9.obj");
    Model pieza13((char*)"Models/estatuanueva10.obj");
    Model pieza14((char*)"Models/estatuanueva11.obj");
    Model pieza15((char*)"Models/estatuanueva12.obj");
    Model pieza16((char*)"Models/estatuanueva13.obj");
    Model pieza17((char*)"Models/estatuanueva14.obj");
    // luminarias
    Model luminaria1((char*)"Models/luminaria1.obj");
    Model luminaria2((char*)"Models/luminaria2.obj");
    Model luminaria3((char*)"Models/luminaria3.obj");
    Model luminaria4((char*)"Models/luminaria4.obj");
    Model luminaria5((char*)"Models/luminaria5.obj");
    Model luminaria6((char*)"Models/luminaria6.obj");
    Model luminaria7((char*)"Models/luminaria7.obj");
    Model luminaria8((char*)"Models/luminaria8.obj");
    Model luminaria9((char*)"Models/luminaria9.obj");
    Model luminaria10((char*)"Models/luminaria10.obj");
    Model luminaria11((char*)"Models/luminaria11.obj");
    Model luminaria12((char*)"Models/luminaria12.obj");
    Model luminaria13((char*)"Models/luminaria13.obj");
    Model luminaria14((char*)"Models/luminaria14.obj");
    Model luminaria15((char*)"Models/luminaria15.obj");
    Model luminaria16((char*)"Models/luminaria16.obj");
    Model luminaria17((char*)"Models/luminaria17.obj");
    Model luminaria18((char*)"Models/luminaria18.obj");
    // bancas
    Model banca1((char*)"Models/banca1.obj");
    Model banca2((char*)"Models/banca2.obj");
    Model banca3((char*)"Models/banca3.obj");
    Model banca4((char*)"Models/banca4.obj");
    Model banca5((char*)"Models/banca5.obj");
    Model banca6((char*)"Models/banca6.obj");
    Model banca7((char*)"Models/banca7.obj");
    Model banca8((char*)"Models/banca8.obj");
    Model banca9((char*)"Models/banca9.obj");
    Model banca10((char*)"Models/banca10.obj");
    Model banca11((char*)"Models/banca11.obj");
    // recepcion
    Model recepcion((char*)"Models/recepcion.obj");
    Model recepcionista((char*)"Models/recepcionista.obj");



    //==========================EXTERIOR=======================
    Model jardin((char*)"Models/jardin.obj");
    Model arboles1((char*)"Models/arboles1.obj");
    Model arboles2((char*)"Models/arboles2.obj");
    Model arboles3((char*)"Models/arboles3.obj");
    Model arboles4((char*)"Models/arboles4.obj");
    Model arboles5((char*)"Models/arboles5.obj");
    Model arboles6((char*)"Models/arboles6.obj");
    Model arboles7((char*)"Models/arboles7.obj");
    Model asientomadera((char*)"Models/asientomadera.obj");
    Model botes((char*)"Models/botes.obj");
    Model cafeteria((char*)"Models/cafeteria.obj");
    Model cuadruples((char*)"Models/cuadruples.obj");
    Model curvas1((char*)"Models/curvas1.obj");
    Model curvas2((char*)"Models/curvas2.obj");
    Model juegos((char*)"Models/juegos.obj");
    Model macetones((char*)"Models/macetones.obj");
    Model malla1((char*)"Models/malla1.obj");
    Model malla2((char*)"Models/malla2.obj");
    Model palapas((char*)"Models/palapas.obj");
    Model piramide((char*)"Models/piramide.obj");
    Model sendero1((char*)"Models/sendero1.obj");
    Model sendero2((char*)"Models/sendero2.obj");
    Model sendero3((char*)"Models/sendero3.obj");
    Model sendero4((char*)"Models/sendero4.obj");
    Model sendero5((char*)"Models/sendero5.obj");
    Model subeybaja((char*)"Models/subeybaja.obj");
    


    // ===== CARGA DE SPOTS DESDE BLENDER =====
    std::vector<SpotInfo> blenderSpots = LoadSpotLightsFromFBX("Models/spotlights.fbx");

    // VAO para el cubo de luz
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // material
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
    glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

    glm::mat4 projection = glm::perspective(
        camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
        0.5f,
        500.0f
    );

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // dir light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 1.0f, 1.0f, 0.95f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);

        // point light 0
        glUniform3f(
            glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x,
            pointLightPositions[0].y,
            pointLightPositions[0].z
        );
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

        // ===== SPOTS DEL FBX =====
        const int MAX_SPOTS = 32;
        int numSpots = (int)blenderSpots.size();
        if (numSpots > MAX_SPOTS - 1) numSpots = MAX_SPOTS - 1; // dejamos uno para la camara

        for (int i = 0; i < numSpots; i++) {
            std::string base = "spotLights[" + std::to_string(i) + "]";

            glUniform3fv(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
                1, glm::value_ptr(blenderSpots[i].position));
            glUniform3fv(glGetUniformLocation(lightingShader.Program, (base + ".direction").c_str()),
                1, glm::value_ptr(glm::normalize(blenderSpots[i].direction)));

            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()),
                glm::cos(glm::radians(blenderSpots[i].innerCutoffDeg)));
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()),
                glm::cos(glm::radians(blenderSpots[i].outerCutoffDeg)));

            glUniform3fv(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
                1, glm::value_ptr(blenderSpots[i].ambient));
            glUniform3fv(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
                1, glm::value_ptr(blenderSpots[i].diffuse));
            glUniform3fv(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
                1, glm::value_ptr(blenderSpots[i].specular));

            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()),
                blenderSpots[i].constant);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()),
                blenderSpots[i].linear);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()),
                blenderSpots[i].quadratic);
        }

        // apagar las que sobran
        for (int i = numSpots; i < MAX_SPOTS; i++) {
            std::string base = "spotLights[" + std::to_string(i) + "]";
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 0.032f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()), glm::cos(glm::radians(12.0f)));
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()), glm::cos(glm::radians(13.0f)));
        }

        // spotlight de la cámara (al final)
        int camIndex = numSpots;
        if (camIndex < MAX_SPOTS) {
            std::string base = "spotLights[" + std::to_string(camIndex) + "]";
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
                camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".direction").c_str()),
                camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()),
                glm::cos(glm::radians(12.0f)));
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()),
                glm::cos(glm::radians(18.0f)));
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
                0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
                0.2f, 0.2f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
                0.0f, 0.0f, 0.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 0.032f);
        }

        // matrices
        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model = glm::mat4(1.0f);

        
       
      

        // INTERIOR
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Interior.Draw(lightingShader);

        // CUADROS
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Cuadro1.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Cuadro2.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Pieza1.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Pieza2.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Pieza3.Draw(lightingShader);

        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro3.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro4.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro5.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro6.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro7.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro8.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro9.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro10.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro11.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro12.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro13.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro14.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro15.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro16.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro17.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadro18.Draw(lightingShader);

        // ESTATUAS
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza4.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza5.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza6.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza7.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza8.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza9.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza10.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza11.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza12.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza13.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza14.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza15.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza16.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); pieza17.Draw(lightingShader);

        // LUMINARIAS
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria1.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria2.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria3.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria4.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria5.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria6.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria7.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria8.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria9.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria10.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria11.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria12.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria13.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria14.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria15.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria16.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria17.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); luminaria18.Draw(lightingShader);

        // BANCAS
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca1.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca2.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca3.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca4.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca5.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca6.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca7.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca8.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca9.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca10.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); banca11.Draw(lightingShader);

        // RECEPCION
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); recepcion.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); recepcionista.Draw(lightingShader);

        //========================== EXTERIOR ===========================================

        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); jardin.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); arboles1.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); arboles2.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); arboles3.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); arboles4.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); arboles5.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); arboles6.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); arboles7.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); asientomadera.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); botes.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cafeteria.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); cuadruples.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); curvas1.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); curvas2.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); juegos.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); macetones.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); malla1.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); malla2.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); palapas.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); piramide.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); sendero1.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); sendero2.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); sendero3.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); sendero4.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); sendero5.Draw(lightingShader);
        model = glm::mat4(1); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); subeybaja.Draw(lightingShader);
        // lamp shader para ver la point light
        lampShader.Use();
        GLint modelLocL = glGetUniformLocation(lampShader.Program, "model");
        GLint viewLocL = glGetUniformLocation(lampShader.Program, "view");
        GLint projLocL = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLocL, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLocL, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLocL, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// movimiento
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_T])
        pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G])
        pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_Y])
        pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H])
        pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_U])
        pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J])
        pointLightPositions[0].z += 0.01f;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
