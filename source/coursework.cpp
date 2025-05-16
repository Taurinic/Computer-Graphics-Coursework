#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>

//Function prototypes
void keyboardInput(GLFWwindow *window);
void mouseInput(GLFWwindow* window);

//Camera Object
Camera camera(glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

//Frame timer
float previousTime = 0.0f;    
float deltaTime = 0.0f;   

//MouseMovement
float mouseSensitivity = 0.5f; //MouseSens

//KeyboardMovement
float normalSpeed = 5.0f; //MovementSpeed
float sprintMulti = 2.0f; //SprintMultiplier


//Object structure
struct Object
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float angle = 0.0f;
    std::string name;
};










int main( void )
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW //Hi||
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework", NULL, NULL);
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Initialize GLEW
    glewExperimental = true; //Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================
    
    //Enable depth test
    glEnable(GL_DEPTH_TEST);

    //Enable back face culling
    glEnable(GL_CULL_FACE);

    //Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //Capture mouse inputs 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    //Define cube object
    //Define vertices
    const float vertices[] = {
        //front
        -1.0f, -1.0f,  1.0f,    //              + ------ +
         1.0f, -1.0f,  1.0f,    //             /|       /|
         1.0f,  1.0f,  1.0f,    //   y        / |      / |
        -1.0f, -1.0f,  1.0f,    //   |       + ------ +  |
         1.0f,  1.0f,  1.0f,    //   + - x   |  + ----|- +
        -1.0f,  1.0f,  1.0f,    //  /        | /      | /
        // right                // z         |/       |/
         1.0f, -1.0f,  1.0f,    //           + ------ +
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         //back
          1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
          //left
          -1.0f, -1.0f, -1.0f,
          -1.0f, -1.0f,  1.0f,
          -1.0f,  1.0f,  1.0f,
          -1.0f, -1.0f, -1.0f,
          -1.0f,  1.0f,  1.0f,
          -1.0f,  1.0f, -1.0f,
          //bottom
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f,  1.0f,
          //top
          -1.0f,  1.0f,  1.0f,
           1.0f,  1.0f,  1.0f,
           1.0f,  1.0f, -1.0f,
          -1.0f,  1.0f,  1.0f,
           1.0f,  1.0f, -1.0f,
          -1.0f,  1.0f, -1.0f,
    };

    //Define texture coordinates
    const float uv[] = {
        //front
        0.0f, 0.0f,     //vertex co-ordinates are the same for each side
        1.0f, 0.0f,     //of the cube so repeat every six vertices
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        //right
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        //back
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        //left
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        //bottom
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        //top
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    //Define indices
    unsigned int indices[] = {
        0,   1,  2,  3,  4,  5,     //front
        6,   7,  8,  9, 10, 11,     //right
        12, 13, 14, 15, 16, 17,     //back
        18, 19, 20, 21, 22, 23,     //left
        24, 25, 26, 27, 28, 29,     //bottom
        30, 31, 32, 33, 34, 35      //top
    };

    //Create the Vertex Array Object (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Create Vertex Buffer Object (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Create texture buffer
    unsigned int uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

    //Create Element Buffer Object (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Compile shader program
    unsigned int shaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

    //Load the textures
    unsigned int texture;
    texture = loadTexture("../assets/Wall.jpg");

    //Send the texture uniforms to the fragment shader
    glUseProgram(shaderID);
    unsigned int textureID;
    textureID = glGetUniformLocation(shaderID, "textureMap");
    glUniform1i(textureID, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    std::vector<Object> objects;
    Object object;
    object.name = "cube";
    object.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.angle = 0.0f;

    //RoomStart\\-------------------

    //Floor
    for (float x = 2.0f; x <= 10.0f; x += 2.0f) {
        for (float z = 2.0f; z <= 10.0f; z += 2.0f) {
            object.position = glm::vec3(x, -2.0f, z);
            objects.push_back(object);
        }
    }

    //Roof
    for (float x = 0.0f; x <= 12.0f; x += 2.0f) {
        for (float z = 0.0f; z <= 12.0f; z += 2.0f) {
            object.position = glm::vec3(x, 6.0f, z);
            objects.push_back(object);
        }
    }


    //Left Wall 
    for (float x = 0.0f; x <= 12.0f; x += 2.0f) {
        for (float y = 0.0f; y <= 4.0f; y += 2.0f) {
            object.position = glm::vec3(x, y, 0.0f);
            objects.push_back(object);
        }
    }

    //Back Wall 
    for (float z = 2.0f; z <= 12.0f; z += 2.0f) {
        for (float y = 0.0f; y <= 4.0f; y += 2.0f) {
            object.position = glm::vec3(0.0f, y, z);
            objects.push_back(object);
        }
    }

    //Right Wall 
    for (float x = 2.0f; x <= 12.0f; x += 2.0f) {
        for (float y = 0.0f; y <= 4.0f; y += 2.0f) {
            object.position = glm::vec3(x, y, 12.0f);
            objects.push_back(object);
        }
    }

    //Front Wall 
    for (float z = 0.0f; z <= 12.0f; z += 2.0f) {
        for (float y = 0.0f; y <= 4.0f; y += 2.0f) {
            object.position = glm::vec3(12.0f, y, z);
            objects.push_back(object);
        }
    }

    //EndRoom\\----------------------------




    // Light settings
    glm::vec3 lightPos(0.0f, 1.0f, 2.0f); // Example light position
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse(0.8f, 0.8f, 0.8f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

    // Attenuation constants
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;


    



    //Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;

        // Input
        keyboardInput(window);
        mouseInput(window);

        // Clear
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Send VBO
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Send UV buffer
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Calculate camera matrices
        camera.calculateMatrices();

        // Set shared uniforms (view, projection, camera position)
        GLint viewID = glGetUniformLocation(shaderID, "view");
        GLint projID = glGetUniformLocation(shaderID, "projection");
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &camera.view[0][0]);
        glUniformMatrix4fv(projID, 1, GL_FALSE, &camera.projection[0][0]);

        // View position
        GLint viewPosID = glGetUniformLocation(shaderID, "viewPos");
        glUniform3fv(viewPosID, 1, &camera.eye[0]);

        // Set point light uniforms
        glUniform3fv(glGetUniformLocation(shaderID, "pointLight.position"), 1, &lightPos[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "pointLight.ambient"), 1, &lightAmbient[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "pointLight.diffuse"), 1, &lightDiffuse[0]);
        glUniform3fv(glGetUniformLocation(shaderID, "pointLight.specular"), 1, &lightSpecular[0]);
        glUniform1f(glGetUniformLocation(shaderID, "pointLight.constant"), constant);
        glUniform1f(glGetUniformLocation(shaderID, "pointLight.linear"), linear);
        glUniform1f(glGetUniformLocation(shaderID, "pointLight.quadratic"), quadratic);

        // Loop through objects
        for (int i = 0; i < static_cast<unsigned int>(objects.size()); i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(objects[i].position);
            glm::mat4 scale = Maths::scale(objects[i].scale);
            glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation);
            glm::mat4 model = translate * rotate * scale;

            // Send model matrix to shader
            GLint modelID = glGetUniformLocation(shaderID, "model");
            glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

            // Draw elements
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteProgram(shaderID);

    //Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Determine current speed
    float currentSpeed = normalSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        currentSpeed *= sprintMulti;

    // Move the camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.eye += currentSpeed * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eye -= currentSpeed * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eye -= currentSpeed * deltaTime * camera.right;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eye += currentSpeed * deltaTime * camera.right;
}

void mouseInput(GLFWwindow* window)
{
    //Get mouse cursor position and reset to centre
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    float offsetX = float(xPos - 1024 / 2);
    float offsetY = float(768 / 2 - yPos); //Inverted Y

    //Apply sensitivity
    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;

    //Update yaw and pitch angles
    camera.yaw += offsetX;
    camera.pitch += offsetY;

    //Calculate camera vectors from the yaw and pitch angles
    camera.calculateCameraVectors();
}





