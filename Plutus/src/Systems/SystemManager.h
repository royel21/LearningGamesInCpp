#pragma once

#include <vector>

namespace Plutus
{
    class ISystem;
    struct Project;

    class SystemManager
    {

    public:
        SystemManager() = default;
        ~SystemManager() { cleanup(); };

        void setProject(Project* project) { mProject = project; }

        void init();
        void update(float dt);
        void draw();
        void stop();


        template <typename T, typename... TArgs>
        T* AddSystem(TArgs &&... args)
        {
            auto id = getId<T>();
            if (id == mSystems.size()) {
                T* newSystem = new T(std::forward<TArgs>(args)...);
                mSystems.push_back(newSystem);
                return newSystem;
            }

            return getSystem<T>();
        }

        template <typename T>
        T* getSystem()
        {
            return hasSystem<T>() ? static_cast<T*>(mSystems[getId<T>()]) : nullptr;
        }

        template <typename T>
        bool hasSystem()
        {
            return getId<T>() < mSystems.size();
        }

        void cleanup();

    private:
        Project* mProject;
        std::vector<ISystem*> mSystems;

        inline uint32_t genId() {
            static int id = 0;
            return id++;
        }

        template<typename T>
        inline uint32_t getId()
        {
            static int id = genId();
            return id;
        }
    };
} // namespace Plutus
