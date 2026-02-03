#include "gui.hpp"
#include "chunk_manager.hpp"
#include "Lsystems_generator.hpp"
#include <string>

GUI::GUI(GLFWwindow* window)
    : terrainChanged(false)
    , plantChanged(false)
    , showSettingsWindow(true)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::StyleColorsDark();
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUI::IsMouseOverGUI() const
{
    return ImGui::GetIO().WantCaptureMouse;
}

void GUI::RenderSettingsWindow(terrainSettings& terrain, LSystemsSettings& lsystem)
{
    if (!showSettingsWindow)
        return;

    ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Procedural World Settings", &showSettingsWindow);

    if (ImGui::CollapsingHeader("Terrain Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        RenderTerrainSettings(terrain);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Plant Settings (L-Systems)", ImGuiTreeNodeFlags_DefaultOpen))
    {
        RenderLSystemSettings(lsystem);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.0f / ImGui::GetIO().Framerate,ImGui::GetIO().Framerate);
    ImGui::End();
}

void GUI::RenderTerrainSettings(terrainSettings& settings)
{
    bool changed = false;

    ImGui::Text("Noise Parameters");
    changed |= ImGui::SliderFloat("Scale", &settings.scale, 0.001f, 0.05f, "%.4f");
    changed |= ImGui::SliderFloat("Lacunarity", &settings.lacunarity, 1.0f, 10.0f);
    changed |= ImGui::SliderFloat("Persistence", &settings.persistence, 0.01f, 1.0f);
    changed |= ImGui::SliderInt("Octaves", &settings.octaves, 1, 10);
    
    ImGui::Spacing();
    ImGui::Text("Height Settings");
    changed |= ImGui::SliderFloat("Max Height", &settings.maxMeshHeight, 0.5f, 50.0f);
    ImGui::Spacing();
    ImGui::Text("Level Of Detail");
    changed |= ImGui::SliderInt("LOD", &settings.levelOfDetail, 1, 6);
    ImGui::Spacing();
    ImGui::Text("Seed");
    int seed = static_cast<int>(settings.seed);
    if (ImGui::InputInt("Seed", &seed))
    {
        settings.seed = static_cast<unsigned int>(seed);
        changed = true;
    }

    if (ImGui::Button("Random Seed"))
    {
        settings.seed = static_cast<unsigned int>(rand());
        changed = true;
    }

    if (changed)
    {
        terrainChanged = true;
    }

    ImGui::Spacing();
    if (ImGui::Button("Regenerate Terrain"))
    {
        terrainChanged = true;
    }
}

void GUI::RenderLSystemSettings(LSystemsSettings& settings)
{
    bool changed = false;

    ImGui::Text("L-System Configuration");

    static char axiomBuffer[256] = "A";
    if (settings.axiom.size() < 256)
    {
        strcpy(axiomBuffer, settings.axiom.c_str());
    }

    if (ImGui::InputText("Axiom", axiomBuffer, 256))
    {
        settings.axiom = std::string(axiomBuffer);
        changed = true;
    }

    changed |= ImGui::Checkbox("Stochastic", &settings.isStochastic);

    ImGui::Spacing();
    ImGui::Text("Production Rules");
    ImGui::Text("Current rules:");

    ImGui::BeginChild("RulesDisplay", ImVec2(0, 100), true);
    for (const auto& rule : settings.rules)
    {
        ImGui::Text("%c -> %s", rule.first, rule.second);
    }
    ImGui::EndChild();

    ImGui::Spacing();
    ImGui::Text("Add New Rule:");

    static char predecessorChar = 'A';
    static char successorBuffer[512] = "F[&+A][^--A][&++A][^A]";

    ImGui::InputText("Predecessor", &predecessorChar, 1);
    ImGui::InputText("Successor", successorBuffer, 512);

    if (ImGui::Button("Add/Update Rule"))
    {
        settings.AddRule(predecessorChar, std::string(successorBuffer));
        changed = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear Rules"))
    {
        settings.rules.clear();
        changed = true;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Presets:");

    if (ImGui::Button("Bush/Tree"))
    {
        settings.axiom = "A";
        settings.rules.clear();
        settings.AddRule('A', "F[&+A][^--A][&++A][^A]");
        settings.AddRule('F', "FF");
        settings.isStochastic = false;
        strcpy(axiomBuffer, "A");
        changed = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Simple Tree"))
    {
        settings.axiom = "F";
        settings.rules.clear();
        settings.AddRule('F', "FF+[+F-F-F]-[-F+F+F]");
        settings.isStochastic = false;
        strcpy(axiomBuffer, "F");
        changed = true;
    }

    if (ImGui::Button("Branching Plant"))
    {
        settings.axiom = "X";
        settings.rules.clear();
        settings.AddRule('X', "F[+X]F[-X]+X");
        settings.AddRule('F', "FF");
        settings.isStochastic = false;
        strcpy(axiomBuffer, "X");
        changed = true;
    }

    if (changed)
    {
        plantChanged = true;
    }

    ImGui::Spacing();
    if (ImGui::Button("Regenerate Plants"))
    {
        plantChanged = true;
    }
}