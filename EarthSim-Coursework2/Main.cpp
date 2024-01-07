#include <iostream>

//GLAD
#include <glad/glad.h>

//GLM
#include "glm/ext/vector_float3.hpp"
#include <glm/gtc/type_ptr.hpp> //Access to value_ptr

//ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//LEARNOPENGL
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>

#include "main.h"

using namespace std;
using namespace glm;
 
//Window
int windowWidth;
int windowHeight;

//VAO vertex attribute positions in correspondence to vertex attribute type
enum VAO_IDs { Triangles, Indices, Colours, Textures, NumVAOs = 2 };

//VAOs & VBOs
GLuint VAOs[NumVAOs];

GLuint earthVAO;
GLuint moonVAO;

GLuint earthVBO;
GLuint moonVBO;

GLuint SatVAO;
GLuint SatVBO;

GLuint skyboxVAO;
GLuint skyboxVBO;

//Buffer types
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
//Buffer objects
GLuint Buffers[NumBuffers];

//Relative position within world space

vec3 cameraPosition = vec3(0.0f, 25.0f, 150.0f);

//The direction of travel
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
//Up position within world space
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

//Camera sidways rotation
float cameraYaw = -90.0f;
//Camera vertical rotation
float cameraPitch = 0.0f;
//Determines if first entry of mouse into window
bool mouseFirstEntry = true;
//Positions of camera from given last frame
float cameraLastXPos = 800.0f / 2.0f;
float cameraLastYPos = 600.0f / 2.0f;

//MVP Matrix
mat4 mvp;
mat4 model;
mat4 view;
mat4 projection;

// Models 
mat4 modelEarth;
mat4 modelMoon;
mat4 modelCamera;
mat4 modelSatellite;

//Time
float deltaTime = 1.0f;
//Last value of time change

float lastFrame = 0.0f;

// Rotation Speed for the Earth
float EarthrotationSpeed = 0.5f;

// Rotation Speed for the Moon
float MoonRotationSpeed = 1.0f;

// Rotation Speed for the Moon Axis
float MoonAxisRotationSpeed = 1.0;

//Textures

GLuint moonTexture;
GLuint earthTexture;
GLuint CubemapTexture;
GLuint SatelliteTexture;

// Cubemap Textures

GLuint LoadCubemap(vector <std::string> faces) {

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;

    for (GLuint i = 0; i < faces.size(); i++) {

        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
        else
        {
            cout << " Cubemap Failed to Load! at path: " << faces[i] << endl;
            
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

int main()
{
    //Initialisation of GLFW
    glfwInit();

    windowWidth = 1280;
    windowHeight = 720;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL EarthSim-Project", NULL, NULL);

    //Checks if window has been successfully instantiated
    if (window == NULL)
    {
        cout << "GLFW Window did not instantiate\n";
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Binds OpenGL to window
    glfwMakeContextCurrent(window);

    //Initialisation of GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "GLAD failed to initialise\n";
        return -1;
    }

    //Loading of shaders
    Shader Shaders("shaders/vertexShader.vert", "shaders/fragmentShader.frag");
    Shader SkyboxShaders("shaders/skybox.vert", "shaders/skybox.frag");

    Shaders.use();
    SkyboxShaders.use();

    float skyboxVertices[] = {

    // Skybox faces

   -200.0f,  200.0f, -200.0f,
   -200.0f, -200.0f, -200.0f,
    200.0f, -200.0f, -200.0f,
    200.0f, -200.0f, -200.0f,
    200.0f,  200.0f, -200.0f,
   -200.0f,  200.0f, -200.0f,

   -200.0f, -200.0f,  200.0f,
   -200.0f, -200.0f, -200.0f,
   -200.0f,  200.0f, -200.0f,
   -200.0f,  200.0f, -200.0f,
   -200.0f,  200.0f,  200.0f,
   -200.0f, -200.0f,  200.0f,

    200.0f, -200.0f, -200.0f,
    200.0f, -200.0f,  200.0f,
    200.0f,  200.0f,  200.0f,
    200.0f,  200.0f,  200.0f,
    200.0f,  200.0f, -200.0f,
    200.0f, -200.0f, -200.0f,

   -200.0f, -200.0f,  200.0f,
   -200.0f,  200.0f,  200.0f,
    200.0f,  200.0f,  200.0f,
    200.0f,  200.0f,  200.0f,
    200.0f, -200.0f,  200.0f,
   -200.0f, -200.0f,  200.0f,

   -200.0f,  200.0f, -200.0f,
    200.0f,  200.0f, -200.0f,
    200.0f,  200.0f,  200.0f,
    200.0f,  200.0f,  200.0f,
   -200.0f,  200.0f,  200.0f,
   -200.0f,  200.0f, -200.0f,

   -200.0f, -200.0f, -200.0f,
   -200.0f, -200.0f,  200.0f,
    200.0f, -200.0f, -200.0f,
    200.0f, -200.0f, -200.0f,
   -200.0f, -200.0f,  200.0f,
    200.0f, -200.0f,  200.0f

    };

    // VAOs and VBOs for Skybox

    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);

    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // cubemap textures 

    vector <string> faces{

        "media/Cubemap/skybox_right.png",     // right
        "media/Cubemap/skybox_left.png",  //left
        "media/Cubemap/skybox_down.png",     // bottom
        "media/Cubemap/skybox_up.png",    // top  
        "media/Cubemap/skybox_back.png",   // back      
        "media/Cubemap/skybox_front.png",    //front    

    }; 

    // binds textures to the cubemap 
    CubemapTexture = LoadCubemap(faces);

    // VAOs for Earth
    glGenVertexArrays(1, &earthVAO);
    glBindVertexArray(earthVAO);

    // VBOs for Earth
    glGenBuffers(1, &earthVBO);
    glBindBuffer(GL_ARRAY_BUFFER, earthVBO);

    // Load Earth 
    Model Earth("media/Earth/earth.obj");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO

    // VAO for Moon
    glGenVertexArrays(1, &moonVAO);
    glBindVertexArray(moonVAO);

    // VBO for Moon
    glGenBuffers(1, &moonVBO);
    glBindBuffer(GL_ARRAY_BUFFER, moonVBO);

    // Load Moon 
    Model Moon("media/Moon/moon6.obj");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO

    // VAO for Moon
    glGenVertexArrays(1, &SatVAO);
    glBindVertexArray(SatVAO);

    // VBO for Moon
    glGenBuffers(1, &SatVBO);
    glBindBuffer(GL_ARRAY_BUFFER, SatVBO);

    Model Satellite("media/Satellite/Satellite3.obj");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO


    // Earth Texture
    // ----------------------------------------------

    int width, height, colourChannels;

    // Apply Earth Texture to the Earth Model
      unsigned char* data = stbi_load("media/Earth/Earth_texture.jpg", &width, &height, &colourChannels, 0);
      if (data) {
  
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);
  
          stbi_image_free(data);
      }
      else {
  
          cout << "Textures failed to load!" << endl;
          return -1;
      }
     
     // Moon Texture
     // ---------------------------------------------

      int MoonWidth, MoonHeight, MoonColourChannels;

    // Apply Moon Texture to the Moon Model
      unsigned char* Moondata = stbi_load("media/Moon/moon_texture.jpg", &MoonWidth, &MoonHeight, &MoonColourChannels, 0);
      if (Moondata) {

          glGenTextures(1, &moonTexture); 
          glBindTexture(GL_TEXTURE_2D, moonTexture); 

          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MoonWidth, MoonHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Moondata);
          glGenerateMipmap(GL_TEXTURE_2D);

          stbi_image_free(Moondata);
      }
      else {

          cout << "Moon Texture failed to load!" << endl;
          return -1;
      }

      int Satwidth, SatHeight, SatColourChannels;

      // Apply Satellite Texture to the Satellite Model
      unsigned char* Satdata = stbi_load("media/Satellite/10477_Satellite_v1_Diffuse.jpg", &Satwidth, &SatHeight, &SatColourChannels, 0);
      if (Satdata) {

          glGenTextures(1, &SatelliteTexture);
          glBindTexture(GL_TEXTURE_2D, SatelliteTexture);

          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Satwidth, SatHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Satdata);
          glGenerateMipmap(GL_TEXTURE_2D);

          stbi_image_free(Satdata);
      }
      else {

          cout << "Satellite Texture failed to load!" << endl;
          return -1;
      }

      // ---------------------------------------------
      
    //Sets the viewport size within the window to match the window size of 1280x720
    glViewport(0, 0, 1280, 720);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetCursorPosCallback(window, mouse_callback);

    //Render loop
    while (glfwWindowShouldClose(window) == false)
    {
        //Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Input
        ProcessUserInput(window); 

        //Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Colour to display on cleared window
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CULL_FACE); //Discards all back-facing triangles

        glEnable(GL_DEPTH_TEST); 

        Shaders.use();  // use shaders

        //Viewer orientation

        modelCamera = mat4(1.0f);
        modelCamera = scale(modelCamera, vec3(0.050f, 0.050f, 0.050f));
        modelCamera = translate(modelCamera, vec3(-100.0f, -50.0f, 0.0f));
        view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); //Sets the position of the viewer, the movement direction in relation to it & the world up direction
        projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 2.0f, 1000.0f);   // 1000.0 so the player can see the entire skybox 
                                            // FOV - Set perspective to 45.0f for default 
        Shaders.use();
        Shaders.setInt("texture_diffuse", 1); // 1 = moon Texture 

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, moonTexture);

        //Moon
        modelMoon = mat4(1.0f);
        modelMoon = translate(modelMoon, vec3(-35.0f, 10.0f, 0.0f)); // Moon translation  
        // First agument = time, second arguemnt = rotation speed, third argument = Rotation axis 
        modelMoon = rotate(modelMoon, MoonRotationSpeed * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f));
        modelMoon = scale(modelMoon, vec3(6.0f, 6.0f, 6.0f)); // Moon scaling

        mvp = projection * view * modelCamera * modelMoon;// mvp
        Shaders.setMat4("mvpIn", mvp);

        glBindVertexArray(moonVAO);
        Moon.Draw(Shaders);
        glBindVertexArray(0);

        // ----------------------------------------------------------
        Shaders.use();
        
        Shaders.setInt("texture_diffuse", 0); // 0 = earth  Texture

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, earthTexture);

        //Earth
        modelEarth = mat4(1.0f);
        modelEarth = translate(modelEarth, vec3(1.0f, 0.0f, 0.0f)); // Earth translation 
        modelEarth = rotate(modelEarth, EarthrotationSpeed * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.1f));
        modelEarth = scale(modelEarth, vec3(10.5f, 10.5f, 10.5f));

        mvp = projection * view * modelCamera * modelEarth;   // mvp
        Shaders.setMat4("mvpIn", mvp);

        glBindVertexArray(earthVAO);
        Earth.Draw(Shaders);
        glBindVertexArray(0);

        // ----------------------------------------------------------

        // Satellite 

        Shaders.use();
        Shaders.setInt("texture_diffuse", 2); // 2 = Satellite Texture

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, SatelliteTexture);

        modelSatellite = mat4(1.0f);
        modelSatellite = translate(modelSatellite, vec3(-20.0f, 0.0f, 0.0f));
        modelSatellite = rotate(modelSatellite, EarthrotationSpeed * (float)glfwGetTime(), vec3(0.0f, -5.0f, 0.0f));
        modelSatellite = scale(modelSatellite, vec3(10.0f, 10.0f, 10.0f));

        mvp = projection * view * modelCamera * modelSatellite;// mvp
        Shaders.setMat4("mvpIn", mvp);
        glBindVertexArray(SatVAO);
        Satellite.Draw(Shaders);
        glBindVertexArray(0);

        // ----------------------------------------------------------

        // Skybox

        glDepthMask(GL_FALSE);  // depth test <=  to keep skybox behind objects

        SkyboxShaders.use();

        //glm::mat4 view = glm::mat4(glm::mat3(cameraView.GetViewMatrix()));    

        glUniform1i(glGetUniformLocation(SkyboxShaders.ID, "skybox"), 0); 
        glUniformMatrix4fv(glGetUniformLocation(SkyboxShaders.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(SkyboxShaders.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        //Transformations & Drawing

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthMask(GL_TRUE);  // reset depth testing

        //Refreshing
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //Safely terminates GLFW
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Resizes window based on contemporary width & height values
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //sets last positions to current positions
    if (mouseFirstEntry)
    {
        cameraLastXPos = (float)xpos;
        cameraLastYPos = (float)ypos;
        mouseFirstEntry = false;
    }

    //Sets values for change in position since last frame to current frame
    float xOffset = (float)xpos - cameraLastXPos;
    float yOffset = cameraLastYPos - (float)ypos;

    //Sets last positions to current positions for next frame
    cameraLastXPos = (float)xpos;
    cameraLastYPos = (float)ypos;

    //Moderates the change in position based on sensitivity value
    const float sensitivity = 0.025f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    //Adjusts yaw & pitch values against changes in positions
    cameraYaw += xOffset;
    cameraPitch += yOffset;

    //Prevents turning up & down beyond 90 degrees to look backwards
    if (cameraPitch > 89.0f)
    {
        cameraPitch = 89.0f;
    }
    else if (cameraPitch < -89.0f)
    {
        cameraPitch = -89.0f;
    }

    //Modification of direction vector based on mouse turning
    vec3 direction;
    direction.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
    direction.y = sin(radians(cameraPitch));
    direction.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
    cameraFront = normalize(direction);
}

void ProcessUserInput(GLFWwindow* WindowIn)
{
    //Closes window on 'exit' key press
    if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(WindowIn, true);
    }

    //Extent to which to move in one instance
    const float movementSpeed = 20.0f * deltaTime;  // make this 1.5 
    //WASD controls
    if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
}