
#include <Core/Window.h>


using namespace Plutus;

int main(int argc, char** argv)
{
    //Create a window
    Window window;
    window.init("Plutus App", 1280, 768);


    while (window.isFinish())
    {
        window.update();
    }
    return 0;
}