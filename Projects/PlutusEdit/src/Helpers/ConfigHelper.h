#pragma once

namespace Plutus
{
    struct Config;

    void loadConfig(Config* config);

    void SaveConfig(Config* config);

    void createProject(const char* path);
} // namespace Plutus
