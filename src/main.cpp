#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.hpp>
#include <shader.hpp>
#include <chunk_manager.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <Lsystems_generator.hpp>
#include <turtle_interpreter.hpp>
#include <plant_generator.hpp>
#include <gui.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window, float deltaTime, GUI& gui);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

unsigned int load_texture(const std::string& texturePath);

unsigned int WIDTH = 1600;
unsigned int HEIGHT = 1200;

// Important stuff for camera
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Procedural World", NULL, NULL);
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

    GUI gui(window);

    shader terrainShader("shaders/terrainVertex.glsl", "shaders/terrainFragment.glsl");

    // Terrain settings
    auto smoothingFunc = [](float x)
        {
            return x * x * (3.f - 2.f * x);
        };
    terrainSettings settings;
    settings.scale = 0.006f;
    settings.seed = 694202137;
    settings.lacunarity = 6.0f;
    settings.persistence = 0.1f;
    settings.smoothingFunction = smoothingFunc;
    settings.octaves = 5;
    settings.maxMeshHeight = 2.0f;

    int viewDistance = 4;
    chunkManager manager(viewDistance, settings);

    // Lightning
    glm::vec3 lighDir = glm::vec3(0.5f, 1.0f, 0.3f);
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

    // Plants
    shader plantShader("shaders/instancedVertex.glsl", "shaders/instancedFragment.glsl");
    LSystemsSettings lSettings;
    lSettings.isStochastic = false;
    lSettings.axiom = "A";
    lSettings.AddRule('A', "F[&+A][^--A][&++A][^A]");
    lSettings.AddRule('F', "FF");

    LSystemsGenerator lGen(lSettings);
    std::string s = lGen.Generate(6);
    turtleInterpreter turtle(25.0f, 0.05f, 0.03f);
    std::vector<glm::mat4> transforms = turtle.Translate(s);

    cylinderSettings cylSettings;
    cylSettings.height = 5.0f;
    cylSettings.radiusBottom = 0.5f;
    cylSettings.radiusTop = 0.45f;
    cylSettings.sectorCount = 30;

    plant_generator pGen;
    instancedMesh plants = pGen.Generate(transforms, cylSettings);
    bool cursorEnabled = false;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        process_input(window, deltaTime, gui);


        gui.BeginFrame();


        gui.RenderSettingsWindow(settings, lSettings);

        // Check if terrain needs regeneration
        if (gui.TerrainNeedsRegeneration())
        {
            std::cout << "Regenerating terrain..." << std::endl;
            settings.smoothingFunction = smoothingFunc; 
            manager = chunkManager(viewDistance, settings);
            gui.ResetTerrainFlag();
        }

        // Check if plants need regeneration
        if (gui.PlantNeedsRegeneration())
        {
            std::cout << "Regenerating plants..." << std::endl;
            LSystemsGenerator newLGen(lSettings);
            std::string newS = newLGen.Generate(6);
            turtleInterpreter newTurtle(25.0f, 0.05f, 0.03f);
            std::vector<glm::mat4> newTransforms = newTurtle.Translate(newS);
            plants = pGen.Generate(newTransforms, cylSettings);
            gui.ResetPlantFlag();
        }

        // Projection and view matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 300.0f);

        // Terrain shader
        terrainShader.use();
        terrainShader.setMat4("model", model);
        terrainShader.setMat4("view", view);
        terrainShader.setMat4("projection", projection);
        terrainShader.setFloat("maxMeshHeight", settings.maxMeshHeight);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        manager.Update(camera.GetPosition());
        manager.Draw(terrainShader);

        plantShader.use();
        plantShader.setMat4("view", view);
        plantShader.setMat4("projection", projection);
        plantShader.setVec3("lightDir", lighDir);
        plantShader.setVec3("lightColor", lightColor);
        plants.Draw(plantShader);

       
        gui.EndFrame();

  
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}

void process_input(GLFWwindow* window, float deltaTime, GUI& gui)
{
  
    static bool tabPressed = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabPressed)
    {
        tabPressed = true;
        static bool cursorEnabled = false;
        cursorEnabled = !cursorEnabled;

        if (cursorEnabled)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true; // Reset to prevent camera jump
        }
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
        tabPressed = false;
    }


    if (!gui.IsMouseOverGUI())
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
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
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

unsigned int load_texture(const std::string& texturePath)
{
    unsigned int texture;
    glGenTextures(1, &texture);
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