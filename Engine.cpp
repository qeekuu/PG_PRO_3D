#include "Engine.h"
#include <iostream>
#include "PrimitiveRenderer.h"
#include "Camera.h"
Engine* Engine::instance = nullptr;
Camera* camera = new Camera;
static float lastX = 0.0f;
static float lastY = 0.0f;
static bool cameraCubeControl = true;
static bool firstMouse = true;
PrimitiveRenderer cube = PrimitiveRenderer(0, 0, 0);
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
    //Ustawienie wersji OpenGL (3.3) oraz profilu
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//Profil compat pozwala kozystac z funkcji takich jak: gluLookAt
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);//Ustawienie zmiany rozmiaru okna na true
    glfwWindowHint(GLFW_DEPTH_BITS, 24); //Ustawienie 24 bitrowej glebi
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);//Ustawienie Podwojnego buforowania
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
    glewExperimental = GL_TRUE; 
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "glewInit() failed: " << glewGetErrorString(err) << std::endl;
        exit(-1);
    }
    //Ustawienie  callbacki klawiatura i mysz
    glfwSetKeyCallback(window, keyboardCallback);
    //Zlapanie kursora myszy (stale ustawienie na srodek ekranu)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(window, mouseCallbackButtons);
    glfwSetCursorPosCallback(window, mouseCallbackCursor);
    //Ustawienie glebi
    glEnable(GL_DEPTH_TEST);
    //Czyszczenie ekranu 
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
    //Ladowanie tablic
    glEnableClientState(GL_VERTEX_ARRAY);
    //Ustawienie wskaznika na tablice
    glVertexPointer(3, GL_FLOAT, 0, cube.getVertexData());
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
    delete camera;
}

void Engine::renderCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Przelaczenie na macierz projekcji
    glMatrixMode(GL_PROJECTION);
    //Wyzerowanie macierzy
    glLoadIdentity();
    glm::mat4 projection = glm::perspective(
        /*Kat pola widzenia w pionie */ glm::radians(45.0f),
        /* Stosunek szerokosci do wysokosci okna */(float)Engine::getInstance()->windowWidth / (float)Engine::getInstance()->windowHeight,
        /* Obiekty blizej nic 0.1f nie beda rysowane */0.1f,
        /* Obiekty dalej niz 1000.0f nie beda rysowane */1000.0f
    );
    //Wczytanie macierzy do OpenGL
    glLoadMatrixf(glm::value_ptr(projection));
    //Przelaczenie na macierz modelowania
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Ustawienie macierzy widoku poprzez gluLookAt na podstawie pozycji i wektorow kamery
    gluLookAt(
        camera->position.x, camera->position.y, camera->position.z,
        camera->position.x + camera->front.x, camera->position.y + camera->front.y, camera->position.z + camera->front.z,
        camera->up.x, camera->up.y, camera->up.z
    );
    glm::mat4 view = camera->getViewMatrix();

    //Rysowanie szescianu
    cube.draw();
    //Przelaczenie podwojengo buforu
    glfwSwapBuffers(Engine::getInstance()->window);
}

void Engine::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    //Obsluga przelaczenia pomiedzy sterowaniem kamera a szescianem
    if (cameraCubeControl)
    {
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        {
            cameraCubeControl = false;
        }
    }
    else
    {
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        {
            cameraCubeControl = true;
        }
    }
    //Obsluga kamery
    if (cameraCubeControl)
    {
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
    else
    {
        //Obsluga ruchem szescianu w lewo,prawo itd
        if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.cubeUp();
        }
        if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.cubeDown();
        }
        if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.cubeLeft();
        }
        if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.cubeRight();
        }
        if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.cubeForward();
        }
        if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.cubeBack();
        }

        //Obracanie os Y:
        if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.rotateY(-5.0f);
        }
        if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.rotateY(5.0f); 
        }

        //Obracanie os X
        if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.rotateX(5.0f);
        }
        if (key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.rotateX(-5.0f);
        }

        //Obracanie os Z
        if (key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.rotateZ(5.0f);
        }
        if (key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.rotateZ(-5.0f);
        }

        // Skalowanie
        if (key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.scaleUp(0.1f);
        }
        if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
        {
            cube.scaleDown(0.1f);
        }
    }
}

void Engine::mouseCallbackButtons(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        std::cout << "Lewy przycisk nacisniety" << std::endl;
    }
}

void Engine::mouseCallbackCursor(GLFWwindow* window,/*Obecna pozycja myszy*/ double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
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