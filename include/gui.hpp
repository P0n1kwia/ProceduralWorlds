#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <functional>

struct terrainSettings;
struct LSystemsSettings;

class GUI
{
public:
    GUI(GLFWwindow* window);
    ~GUI();
    void BeginFrame();
    void EndFrame();
    void RenderSettingsWindow(terrainSettings& terrain, LSystemsSettings& lsystem);
    bool TerrainNeedsRegeneration() const { return terrainChanged; }
    bool PlantNeedsRegeneration() const { return plantChanged; }
    void ResetTerrainFlag() { terrainChanged = false; }
    void ResetPlantFlag() { plantChanged = false; }
    bool IsMouseOverGUI() const;

private:
    void RenderTerrainSettings(terrainSettings& settings);
    void RenderLSystemSettings(LSystemsSettings& settings);

    bool terrainChanged;
    bool plantChanged;
    bool showSettingsWindow;
};