#include "Engine.h"
#include <iostream>
#include "PrimitiveRenderer.h"
#include "Camera.h"
Engine* Engine::instance = nullptr;
Camera* camera = new Camera;
Engine* Engine::getInstance()
{
    if (!instance)
    {
        instance = new Engine;
    }
    return instance;
}

void Engine::initGL()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }
    // Ustawienie wersji OpenGL (3.3) oraz profilu
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //Ustawienie zmiany rozmiaru okna na true
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24); // Ustawienie 24 bitrowej glebi
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);// Ustawienie Podwojnego buforowania
    if (fullscreen)
    {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        window = glfwCreateWindow(mode->width, mode->height, windowTitle, primaryMonitor, nullptr);
    }
    else
    {
        window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
    }

    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    //Ustawienie  callbacki klawiatura i mysz
    glfwSetKeyCallback(window, keyboardCallback);
    //Zlapanie kursora myszy (stale ustawienie na srodek ekranu)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(window, mouseCallbackButtons);
    glfwSetCursorPosCallback(window, mouseCallbackCursor);
    // Ustawienie glebi
    glEnable(GL_DEPTH_TEST);
    // Czyszczenie ekranu 
    glClearColor(r, g, b, a);
}

void Engine::setWindowSize(int windowWidth, int windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
}

void Engine::setWindowTitle(const char* title)
{
    this->windowTitle = title;
}

void Engine::setFPS(float fps)
{
    this->fps = fps;
}

void Engine::setClearColor(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Engine::setFullScreen(bool fullscreen)
{
    this->fullscreen = fullscreen;
}

void Engine::changeResolution(int windowWidth, int windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    glfwSetWindowSize(window, windowWidth, windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
}

void Engine::start()
{
    initGL();
    
    while (!glfwWindowShouldClose(window))
    {
        renderCallback();
        glfwPollEvents();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    shutdown();
}

void Engine::shutdown()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }

    glfwTerminate();

    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

void Engine::renderCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Przelaczenie na macierz projekcji
    glMatrixMode(GL_PROJECTION);
    // Wyzerowanie macierzy
    glLoadIdentity();
    glm::mat4 projection = glm::perspective(
        /*Kat pola widzenia w pionie */ glm::radians(45.0f),
        /* Stosunek szerokosci do wysokosci okna */(float)Engine::getInstance()->windowWidth / (float)Engine::getInstance()->windowHeight,
        /* Obiekty blizej nic 0.1f nie beda rysowane */0.1f,
        /* Obiekty dalej niz 100.0f nie beda rysowane */1000.0f
    );
    // Wczytanie macierzy do OpenGL
    glLoadMatrixf(glm::value_ptr(projection));
    // Przelaczenie na macierz modelowania
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Ustawienie macierzy widoku poprzez gluLookAt na podstawie pozycji i wektorow kamery
    gluLookAt(
        camera->position.x, camera->position.y, camera->position.z,
        camera->position.x + camera->front.x, camera->position.y + camera->front.y, camera->position.z + camera->front.z,
        camera->up.x, camera->up.y, camera->up.z
    );
    glm::mat4 view = camera->getViewMatrix();
    PrimitiveRenderer cube_vert;

    // Przednia
    cube_vert.addVertex(1.0f, 1.0f, 1.0f);    
    cube_vert.addVertex(-1.0f, 1.0f, 1.0f);   
    cube_vert.addVertex(-1.0f, -1.0f, 1.0f);  
    cube_vert.addVertex(1.0f, -1.0f, 1.0f);   

    // Tylna
    cube_vert.addVertex(1.0f, 1.0f, -1.0f);   
    cube_vert.addVertex(-1.0f, 1.0f, -1.0f);  
    cube_vert.addVertex(-1.0f, -1.0f, -1.0f); 
    cube_vert.addVertex(1.0f, -1.0f, -1.0f);  

    // Prawa
    cube_vert.addVertex(1.0f, 1.0f, 1.0f);   
    cube_vert.addVertex(1.0f, -1.0f, 1.0f); 
    cube_vert.addVertex(1.0f, -1.0f, -1.0f);  
    cube_vert.addVertex(1.0f, 1.0f, -1.0f);   

    // Lewa
    cube_vert.addVertex(-1.0f, 1.0f, 1.0f); 
    cube_vert.addVertex(-1.0f, -1.0f, 1.0f); 
    cube_vert.addVertex(-1.0f, -1.0f, -1.0f); 
    cube_vert.addVertex(-1.0f, 1.0f, -1.0f);  

    // Gorna
    cube_vert.addVertex(1.0f, 1.0f, 1.0f);   
    cube_vert.addVertex(-1.0f, 1.0f, 1.0f);   
    cube_vert.addVertex(-1.0f, 1.0f, -1.0f); 
    cube_vert.addVertex(1.0f, 1.0f, -1.0f);   

    // Dolna 
    cube_vert.addVertex(1.0f, -1.0f, 1.0f);  
    cube_vert.addVertex(-1.0f, -1.0f, 1.0f); 
    cube_vert.addVertex(-1.0f, -1.0f, -1.0f); 
    cube_vert.addVertex(1.0f, -1.0f, -1.0f); 

    // Przednia
    cube_vert.addIndex(0, 1, 2);
    cube_vert.addIndex(0, 2, 3);

    // Tylna
    cube_vert.addIndex(4, 5, 6);
    cube_vert.addIndex(4, 6, 7);

    // Prawa
    cube_vert.addIndex(8, 9, 10);
    cube_vert.addIndex(8, 10, 11);

    // Lewa
    cube_vert.addIndex(12, 13, 14);
    cube_vert.addIndex(12, 14, 15);

    // Gorna
    cube_vert.addIndex(16, 17, 18);
    cube_vert.addIndex(16, 18, 19);

    // Dolna
    cube_vert.addIndex(20, 21, 22);
    cube_vert.addIndex(20, 22, 23);
    // Ladowanie tablic
    glEnableClientState(GL_VERTEX_ARRAY);
    // Ustawienie wskaznika na tablice
    glVertexPointer(3, GL_FLOAT, 0, cube_vert.getData());
    // Rysowanie
    glDrawElements(GL_TRIANGLES, cube_vert.getIndexCount(), GL_UNSIGNED_INT, cube_vert.getIndexData());

    glfwSwapBuffers(Engine::getInstance()->window);
}

void Engine::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        camera->CameraKeyBoardHandler(GLFW_KEY_W);
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        camera->CameraKeyBoardHandler(GLFW_KEY_S);
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        camera->CameraKeyBoardHandler(GLFW_KEY_A);
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        camera->CameraKeyBoardHandler(GLFW_KEY_D);
    }
    if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        camera->CameraKeyBoardHandler(GLFW_KEY_SPACE);
    }
    if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        camera->CameraKeyBoardHandler(GLFW_KEY_LEFT_CONTROL);
    }
}

void Engine::mouseCallbackButtons(GLFWwindow* window, int button, int action,int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        std::cout << "Lewy przycisk nacisniety" << std::endl;
    }
}

void Engine::mouseCallbackCursor(GLFWwindow* window,/*Obecna pozycja myszy*/ double xpos, double ypos)
{
    float lastX = Engine::getInstance()->windowWidth/2;
    float lastY = Engine::getInstance()->windowHeight/2;
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= camera->sensitivity;
    yoffset *= camera->sensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    if (camera->pitch > 89.0f)
    {
        camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f)
    {
        camera->pitch = -89.0f;
    }

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    newFront.y = sin(glm::radians(camera->pitch));
    newFront.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->front = glm::normalize(newFront);
    camera->right = glm::normalize(glm::cross(camera->front, camera->worldUp)); 
    camera->up = glm::normalize(glm::cross(camera->right, camera->front));
}