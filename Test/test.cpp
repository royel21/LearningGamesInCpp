
#include <cstdio>
#include <ECS/Scene.h>
#include <ECS/Components.h> 
#include <typeinfo>

int main(int argc, char const* argv[])
{
    Plutus::Scene scene;


    auto trans = Plutus::Transform(0, 0, 25, 25);
    auto trans2 = Plutus::Transform(trans);
    trans2.x = 10;

    auto tmap1 = Plutus::TileMap(32, 32, 1);
    tmap1.addTile({ 25,25,0,50, false, false, 90.0f });
    auto tmap2 = Plutus::TileMap(tmap1);
    tmap2.mTiles[0].x = 600;

    printf("trans: %.0f %.0f %i %i\n", trans.x, trans.y, trans.w, trans.h);
    printf("trans: %.0f %.0f %i %i\n", trans2.x, trans2.y, trans2.w, trans2.h);

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

    // Plutus::SoundEngine.cleanUp();

    // "5 9 7"

    return 0;
}
