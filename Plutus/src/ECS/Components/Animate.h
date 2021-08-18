#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "Assets/Textures.h"

namespace Plutus
{
    class Entity;

    struct Animation
    {
        float mAnimTime;
        int mTexIndex;
        uint32_t mFrame = 0;
        std::vector<int> mFrames;
        Animation() = default;
        Animation(std::vector<int> frames, int texIndex, int _animTime = 100);
    };

    class Animate
    {
    public:
        Animate(Entity *ent) : mEnt(ent){};

        void update(float dt);
        void addTexture(const std::string &id);
        void AddAnimation(const std::string id, Animation anin);
        void PlayAnimation(const std::string &id, bool _loop = false);

    public:
        bool loop = false;
        float currentTime = 0.0f;
        std::string currentAnin;
        std::unordered_map<std::string, Animation> animations;
        std::vector<TileSet *> mTextures;
        Entity *mEnt = nullptr;
    };
} // namespace Plutus