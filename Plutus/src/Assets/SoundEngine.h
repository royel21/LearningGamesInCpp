
#ifdef __EMSCRIPTEN__
#include <Platforms/Web/AudioEngine.h>
#elif __ANDROID__
#include <Platforms/Android/AudioEngine.h>
#else
#include <Platforms/Windows/AudioEngine.h>
#endif