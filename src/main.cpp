#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.hpp>
#include <shader.hpp>
#include <chunk_manager.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <Lsystems_generator.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window,float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


unsigned int load_texture(const std::string& texturePath);

unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;

//important stuff for camera
glm::vec3 cameraPosition = glm::vec3(0.0f, 15.0f, 0.0f);
Camera camera(cameraPosition);
float movementSpeed = 15.f;
bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;


int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  
    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT, "Procedural World", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    shader terrainShader("shaders/terrainVertex.glsl", "shaders/terrainFragment.glsl");
   
    //terrain settings
    auto smoothingFunc = [](float x)
        {
            return x * x * (3.f - 2.f * x);
        };
    terrainSettings settings;
    settings.scale = 0.007f;
    settings.seed = 694202137;
    settings.lacunarity = 6.0f;
    settings.persistence = 0.1f;
    settings.smoothingFunction = smoothingFunc;
    settings.octaves = 5;
    settings.maxMeshHeight = 25.0f;

    int viewDistance = 4;
    
    chunkManager manager(viewDistance, settings);

    //lightning
    glm::vec3 lighDir = glm::vec3(0.5f,1.0f,0.3f);
    lighDir = glm::normalize(lighDir);
    glm::vec3 lightColor = glm::vec3(1.0f);
    terrainShader.use();
    terrainShader.setVec3("lightDir", lighDir);
    terrainShader.setVec3("lightColor", lightColor);

    camera.SetMovementSpeed(movementSpeed);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(window))
    {

        //Delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        

        //Input & Camera Work
        process_input(window, deltaTime);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 300.0f);


        //terrain shader
        terrainShader.use();
        terrainShader.setMat4("model", model);
        terrainShader.setMat4("view", view);
        terrainShader.setMat4("projection", projection);
        

        //terrain shader aditional stuff
        terrainShader.setFloat("maxMeshHeight", settings.maxMeshHeight);


        //Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        manager.Update(camera.GetPosition());
        manager.Draw(terrainShader);



        //Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void process_input(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window,true);
    }
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (!firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
//so far it will be a function -> in future it would be a class!
unsigned int load_texture(const std::string& texturePath)
{
    unsigned int texture;
    glGenTextures(1,&texture );
    glBindTexture(GL_TEXTURE_2D, texture);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Failed to open texture: " << texturePath << " MAIN_LOAD_TEXTURE\n";
    }
    else
    {
        GLenum format;
        switch (nrChannels)
        {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Invalid texture format! MAIN_LOAD_TEXTURE\n";
            break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    stbi_image_free(data);
    return texture;
}