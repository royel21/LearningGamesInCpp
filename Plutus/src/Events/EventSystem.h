#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "IEvent.h"

namespace Plutus
{
    class EventSystem
    {
    public:
        static EventSystem* get();

        ~EventSystem() { cleanUp(); }
        template <typename T, typename... TArgs>
        void addEvent(TArgs &&... args)
        {
            auto listId = getListId<T>();
            if (!hasEvents<T>()) mEvents.resize(listId + 1);

            mEvents[listId].push_back(std::make_unique<T>(std::forward<TArgs>(args)...));
        }

        template<typename T>
        std::vector<std::unique_ptr<IEvent>>& getEvents() {
            auto listId = getListId<T>();
            if (!hasEvents<T>()) mEvents.resize(listId + 1);

            return mEvents[listId];
        }

        template<typename T>
        inline bool hasEvents() {
            return getListId<T>() < mEvents.size();
        }

        void cleanUp();

    private:
        std::vector<std::vector<std::unique_ptr<IEvent>>> mEvents;

        EventSystem() {};

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