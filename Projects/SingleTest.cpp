
#include <Core/Window.h>


using namespace Plutus;

int main(int argc, char** argv)
{
    //Create a window
    // Window window;
    // window.init("Plutus App", 1280, 768);


    // while (window.isFinish())
    // {
    //     window.update();
    // }

    uint16_t val = 0xF0FF;

    for (size_t i = 1; i < 0xffff; i *= 2)
    {
        printf("%i ", val & i);
    }

    printf("\n");

    return 0;
}