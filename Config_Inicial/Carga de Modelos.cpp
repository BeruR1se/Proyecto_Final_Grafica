//Practica 6
//Alumno: Alvarez Lugo Victor Manuel
//Número de Cuenta:319306972
//Fecha de Entrega: 26 de septiembre del 2025


// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, " Practica 6 Alvarez Lugo Victor Manuel", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");

    // Load models
    /*Model dog((char*)"Models/RedDog.obj");
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    */

    Model ghost((char*)"Models/hifive.obj"); //Se carga el modelo del fantasma
    Model vampire((char*)"Models/sunflower_vampire.obj"); //Se carga el modelo del vampiro
    Model wolf((char*)"Models/Wolf.obj"); //Se carga el modelo del lobo 
    Model castle((char*)"Models/Castillo.obj"); //Se carga el modelo del castillo
    Model bat((char*)"Models/SB_Bat.obj");//Se carga el modelo del murcielago
    Model forest((char*)"Models/Grass battleground.obj");//Se carga el modelo del entorno
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        /*glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);*/

        ////Carga del Segundo Modelo
        //model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f)); //Se hace una traslacion del modelo
        //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));//Se hace al doble del tamaño
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //dog.Draw(shader);//Se carga el modelo al Shader

        //Se dibuja el Modelo del Bosque
        glm::mat4 modelForest(1);
        modelForest = glm::translate(modelForest, glm::vec3(0.0f, 0.0f, 0.0f)); //Se hace una traslacion del modelo
        modelForest = glm::scale(modelForest, glm::vec3(0.1f, 0.1f, 0.1f));//Se escala para que abarque mas o menos la ventana
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelForest));
        forest.Draw(shader);// Dibujamos el modelo

        glm::mat4 modelBat(1);//Se dibuja el Modelo del Murcielago
        modelBat = glm::translate(modelBat, glm::vec3(0.0f, 10.0f, 0.0f)); //Se hace una traslacion del modelo para que quede arriba de la casa
        modelBat = glm::scale(modelBat, glm::vec3(3.0f, 3.0f, 3.0f));//Se hace al triple del tamaño
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBat));
        bat.Draw(shader);// Dibujamos el modelo


        glm::mat4 modelCastle(1);//Se dibuja el modelo del castillo
        modelCastle = glm::translate(modelCastle, glm::vec3(0.0f, 0.0f, 0.0f)); //Se coloca en el origen 
        modelCastle = glm::scale(modelCastle, glm::vec3(1.0f, 1.0f, 1.0f));//Se hace a la mitad del tamaño
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCastle));
        castle.Draw(shader);//Dibujamos el modelo


        glm::mat4 modelWolf(1);//Se dibuja el modelo del lobo
        modelWolf = glm::translate(modelWolf, glm::vec3(5.0f, 0.0f, 0.0f)); //Se hace una traslacion del modelo
        modelWolf = glm::scale(modelWolf, glm::vec3(1.0f, 1.0f, 1.0f));//Se hace al doble del tamaño
        modelWolf = glm::rotate(modelWolf, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f)); //Se rota el modelo en el eje Y
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelWolf));
        wolf.Draw(shader);//Dibujamos el modelo


        glm::mat4 modelGhost(1);//Se dibuja el modelo del fantasma
        modelGhost = glm::translate(modelGhost, glm::vec3(0.0f, 5.0f, 5.0f)); //Se hace una traslacion del modelo para que quede enfrente de la casa
        modelGhost = glm::scale(modelGhost, glm::vec3(2.0f, 2.0f, 2.0f));// Se hace al doble del tamaño
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGhost));
        ghost.Draw(shader);//Dibujamos el modelo
        

        glm::mat4 modelVampire(1);//Se dibuja el modelo del Vampiro
        modelVampire = glm::translate(modelVampire, glm::vec3(-5.0f, 0.0f, 0.0f)); //Se hace una traslacion del modelo
        modelVampire = glm::scale(modelVampire, glm::vec3(1.0f, 1.0f, 1.0f));//Se cambia la escala del modelo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelVampire));
        vampire.Draw(shader);//Se dibuja el modelo

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

