#include "Scene.h"
#include <algorithm>

namespace Plutus
{
    Ref<Entity> Layer::add(Ref<Entity> item)
    {
        auto index = mEntities.size();
        mEntities.push_back(item);
        return mEntities[index];
    }

    void Layer::remove(const Entity* item)
    {
        auto e1 = std::remove_if(mEntities.begin(), mEntities.end(), [item](auto e) -> bool
            { return e->mId == item->mId; });

        if (e1 != mEntities.end())
        {
            mEntities.erase(e1, mEntities.end());
        }
    }
} // namespace Plutus
