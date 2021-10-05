#include "Web/AudioEngine.h"

#include <cstdio>

static uint8_t decToStr(int32_t var, char buff[], uint8_t i)
{
    do
    {
        buff[i++] = (char)(var % 10) + '0'; //integer to ANSI
        var /= 10;
    } while (var);
    return i;
}

void floatToStr(double floating, char* str)
{
    char numb[16];
    uint8_t i = 0;
    floating += 0.0005;
    if (floating < 0)
    {
        *str++ = '-';
        floating *= -1;
    }

    int integer = (int)floating;
    int dec = (int)((floating - integer) * 1000);

    if (dec > 0)
    {
        i = decToStr(dec, numb, i);

        if (dec < 100)
            numb[i++] = '0';
        if (dec < 10)
            numb[i++] = '0';
        numb[i++] = '.';
    }
    else
    {
        numb[i++] = '0';
        numb[i++] = '.';
    }
    i = decToStr(integer, numb, i);

    while (i--)
    {
        *str++ = numb[i];
    }
    *str = '\0';
}

int main(int argc, char const* argv[])
{
    // Plutus::SoundEngine.add("bg", "assets/sounds/XYZ2.ogg", Plutus::EFFECT);
    // Plutus::SoundEngine.add("rifle", "assets/sounds/shots/machine.ogg", Plutus::EFFECT);
    // Plutus::SoundEngine.play("bg");

    // char key = 'c';
    // while (key != 'q') {
    //     key = getchar();

    //     // switch (key) {
    //     // case 'p': {
    //     //     Plutus::SoundEngine.pause("bg");
    //     //     break;
    //     // }
    //     // case 's': {
    //     //     Plutus::SoundEngine.stop("bg");
    //     //     break;
    //     // }
    //     // case 'c': {
    //     //     Plutus::SoundEngine.play("rifle");
    //     //     break;
    //     // }
    //     // }
    //     printf("char %i\n", key);
    // }
    char buff[16];
    floatToStr(0.9854f, buff);

    printf("str: %sv\n", buff);
    // Plutus::SoundEngine.cleanUp();

    // "5 9 7"

    return 0;
}
