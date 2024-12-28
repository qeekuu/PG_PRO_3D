#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Engine
{
public:
    static Engine* getInstance();
    void setClearColor(float r, float g, float b, float a);
    void setWindowSize(int windowWidth, int windowHeight);
    void setWindowTitle(const char* title);
    void setFPS(float fps);
    void setFullScreen(bool fullscreen);
    void start();
    void shutdown();
    void changeResolution(int windowWidth, int windowHeight);
private:
    static Engine* instance;
    int windowWidth = 1600;
    int windowHeight = 900;
    const char* windowTitle = "";
    bool fullscreen = false;
    float fps = 60;
    float r = 0, g = 0, b = 0, a = 1;
    GLFWwindow* window = nullptr;
    static void renderCallback();
    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallbackButtons(GLFWwindow* window, int button, int action,int mods);
    static void mouseCallbackCursor(GLFWwindow* window, double xpos, double ypos);
    Engine() {};
    void initGL();
};

