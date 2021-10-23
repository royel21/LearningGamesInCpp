#pragma once

namespace Plutus
{
    class IEventSystem
    {
        virtual void addEventListener();
        virtual void removeEventListener();
    };
} // namespace Plutus