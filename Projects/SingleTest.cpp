
#include <cstdio>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{

    // readMapData("assets/tilemaps/testing.json", &scene);
    // auto ent = scene.getEntityByName("bg1");

    // if (ent) {
    //     auto map = ent.getComponent<TileMapComponent>();
    //     printf("size: %i", map->mTiles.size());
    // }

    // printf("Sound: %i\n", getAssetListId<Sound>());
    // printf("Sound: %i\n", getAssetListId<Sound>());
    // printf("Sound: %i\n", getAssetListId<Texture>());
    // printf("Sound: %i\n", getAssetListId<Font>());
    // printf("Sound: %i\n", getAssetListId<Texture>());

    const int minimum_number = -5;
    const int maximum_number = 5;
    unsigned int i;
    srand((int)time(0));
    for (i = 0; i <= 10; i++) {
        const int new_number = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
        printf("%d\n", new_number);
    }

    return 0;
}