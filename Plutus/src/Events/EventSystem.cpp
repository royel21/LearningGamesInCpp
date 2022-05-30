#include "EventSystem.h"
#include "IEvent.h"

namespace Plutus {
    EventSystem* EventSystem::get()
    {
        static EventSystem eventSystem;
        return &eventSystem;
    }

    void EventSystem::cleanUp()
    {
        for (auto& list : mEvents) {
            list.clear();
        }
    }
}