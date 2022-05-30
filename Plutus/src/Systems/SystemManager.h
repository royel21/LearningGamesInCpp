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

        inline void init() {
            for (auto& sys : mSystems) {
                if (sys) {
                    sys->setManager(this);
                    sys->setProject(mProject);
                    sys->init();
                }
            }
        }

        inline void update(float dt) {
            for (auto& sys : mSystems) {
                if (sys)sys->update(dt);
            }
        }

        inline void stop() {
            for (auto& sys : mSystems) {
                if (sys)sys->destroy();
            }
        }

        inline void cleanup() {
            for (size_t i = 0; i < mSystems.size(); i++)
            {
                if (mSystems[i]) {
                    mSystems[i]->destroy();
                    delete mSystems[i];
                    mSystems[i] = nullptr;
                }
            }
        }


        template <typename T, typename... TArgs>
        T* AddSystem(TArgs &&... args)
        {
            auto id = getListId<T>();
            if (id == mSystems.size()) {
                mSystems.resize(id + 1);
            }

            T* newSystem = nullptr;

            if (mSystems[id] == nullptr) {
                newSystem = new T(std::forward<TArgs>(args)...);
                mSystems[id] = newSystem;
            }

            return newSystem;
        }

        template <typename T>
        T* getSystem()
        {
            return static_cast<T*>(mSystems[getListId<T>()]);
        }

        template <typename T>
        bool hasSystem()
        {
            return mSystems[getListId<T>()] != nullptr;
        }

    private:
        Project* mProject;
        std::vector<ISystem*> mSystems;

        inline uint32_t getId() {
            static int id = 0;
            return id++;
        }

        template<typename T>
        inline uint32_t getListId()
        {
            static int id = getId();
            return id;
        }
    };
} // namespace Plutus
