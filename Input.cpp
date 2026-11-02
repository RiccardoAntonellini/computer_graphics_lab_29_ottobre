#include <glad/glad.h>
#include "Input.h"

namespace Input
{
    double mouseX = 0.0;
    double mouseY = 0.0;

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        (void)scancode;
        (void)mods;

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        mouseX = xpos;
        mouseY = static_cast<double>(height) - ypos;
    }

    void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        (void)window;
        glViewport(0, 0, width, height);
    }
}
