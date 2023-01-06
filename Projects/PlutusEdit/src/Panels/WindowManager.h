#pragma once

#include <string>
#include <vector>
#include "ImWindow.h"

namespace Plutus
{
    struct Config;
    class App;

    class WindowManager
    {
    public:
        bool mExit = false;

    public:
        WindowManager() {}
        ~WindowManager();

        void init(Config* config, App* app);
        void update(float dt);
        void draw();

        template <typename T, typename... TArgs>
        T* addWindow(TArgs &&... args) {
            auto id = getListId<T>();
            if (id == mWindows.size()) {
                mWindows.resize(id + 1);
            }

            T* newWindow = nullptr;

            if (mWindows[id] == nullptr) {
                newWindow = new T(std::forward<TArgs>(args)...);
                mWindows[id] = newWindow;
            }

            return newWindow;
        }

        template <typename T>
        T* getWindow()
        {
            return static_cast<T*>(mWindows[getListId<T>()]);
        }

        template <typename T>
        bool hasWindow()
        {
            return mWindows[getListId<T>()] != nullptr;
        }

    private:
        bool isInitialized = false;

        Config* mConfig;
        std::string toEdit;
        std::string mProjToRemove;

        App* mApp;
        std::vector<ImWindow*> mWindows;

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

        void DockingWindow();
        void MenuGui(int flags);

        // Start Dear ImGui frame
        void Begin();
        // End Dear ImGui frame
        void End();
    };
} // namespace Plutus