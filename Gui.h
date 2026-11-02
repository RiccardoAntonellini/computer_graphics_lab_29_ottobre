#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

// ============================================================================
// Gui
// Incapsula Dear ImGui e i parametri modificabili dall'utente.
// ============================================================================
class Gui
{
public:
    void initialize(GLFWwindow* window);
    void beginFrame();
    void draw();
    void render();
    void shutdown();

    const glm::vec4& clearColor() const
    {
        return m_clearColor;
    }

    bool wireframe() const
    {
        return m_wireframe;
    }

private:
    glm::vec4 m_clearColor = glm::vec4(0.0f, 0.2f, 1.0f, 1.0f);
    bool m_wireframe = false;
};
