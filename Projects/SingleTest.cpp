#include <miniaudio.h>

#include <stdio.h>

#include <Time/Timer.h>
#include <Log/Logger.h>

int main(int argc, char** argv)
{
    ma_result result;
    ma_engine engine;
    ma_sound sound;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return -1;
    }

    auto start = Plutus::Timer::micros();
    result = ma_sound_init_from_file(&engine, "Chunky.mp3", 0, NULL, NULL, &sound);

    ma_sound_start(&sound);
    ma_sound_set_looping(&sound, true);



    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return -1;
    }

    // result = ma_engine_play_sound(&engine, "./assets/sounds/XYZ.ogg", NULL);
    // ma_engine_play_sound(&engine, "./Chunky.mp3", NULL);
    Logger::info("Time: %llu", Plutus::Timer::micros() - start);
    printf("Press Enter to quit...");
    getchar();
    ma_sound_stop(&sound);

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);

    return 0;
}