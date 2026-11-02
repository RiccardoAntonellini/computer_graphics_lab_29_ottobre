#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Input
{
    extern double mouseX;
    extern double mouseY;

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
}
