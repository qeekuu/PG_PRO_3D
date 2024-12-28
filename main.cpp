#include "Engine.h"
int main(int argc, char** argv)
{
    Engine::getInstance()->setWindowTitle("Super Dioda");
    Engine::getInstance()->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Engine::getInstance()->start();
    
    return 0;
}