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
    bool TerrainNeedsRegeneration() const { return m_terrainChanged; }
    bool PlantNeedsRegeneration() const { return m_plantChanged; }
    void ResetTerrainFlag() { m_terrainChanged = false; }
    void ResetPlantFlag() { m_plantChanged = false; }
    bool IsMouseOverGUI() const;

private:
    void RenderTerrainSettings(terrainSettings& settings);
    void RenderLSystemSettings(LSystemsSettings& settings);

    bool m_terrainChanged;
    bool m_plantChanged;
    bool m_showSettingsWindow;
};