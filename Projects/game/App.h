#pragma once
#include <Core/Core.h>

class App : public Plutus::Core
{
public:
    App(const char* name, int width, int height);
    ~App();
    void Init() override;
    void Exit() override;
};
