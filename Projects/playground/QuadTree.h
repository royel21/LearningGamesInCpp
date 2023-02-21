#pragma once
#include <Math/Rect.h>

#include <array>
#include <memory>

#include <vector>
#include <list>
#include <utility>

#include <Log/Logger.h>
#include <Time/Timer.h>

namespace Plutus
{
    constexpr size_t MAX_DEPTH = 20;

    using QuadItem = std::pair<Rect, uint32_t>;

    class QuadTree
    {

    private:
        size_t mDepth = 0;

        // area of the 4 children
        std::array<Rect, 4> mChildsArea{};
    public:
        //Area of root quad tree
        Rect mRect;
        // 4 child of the quad tree
        std::array<std::shared_ptr<QuadTree>, 4> mChilds{};
        //Items belonging to this qtree
        std::vector<QuadItem> mItems;
        QuadTree(const Rect& area = { 0,0, 100, 100 }, const size_t depth = 0): mDepth(depth) { resize(area); }

        void clear() {
            mItems.clear();

            for (size_t i = 0; i < 4; i++) {
                if (mChilds[i]) mChilds[i]->clear();
                mChilds[i].reset();
            }
        }

        size_t size() {
            size_t nCount = mItems.size();
            for (size_t i = 0; i < 4; i++)
                if (mChilds[i]) nCount += mChilds[i]->size();

            return nCount;
        }

        std::pair<uint32_t, QuadTree*> insert(uint32_t index, const Rect& itemSize) {
            auto curDepth = mDepth + 1;

            for (size_t i = 0; i < 4; i++)
            {
                if (mChildsArea[i].contains(itemSize)) {
                    if (curDepth < MAX_DEPTH) {

                        if (!mChilds[i]) {
                            mChilds[i] = std::make_shared<QuadTree>(mChildsArea[i], curDepth);
                        }
                        return mChilds[i]->insert(index, itemSize);
                    }
                }
            }
            mItems.push_back({ itemSize, index });
            return { mItems.size() - 1, this };
        }

        void resize(const Rect& area) {
            clear();
            mRect = area;

            Vec2f halfSize = area.size / 2.0f;

            mChildsArea = {
                Rect{area.pos, halfSize}, // bottom left
                Rect{{area.pos.x + halfSize.x, area.pos.y}, halfSize}, // bottom right
                Rect{area.pos + halfSize, halfSize}, // top right
                Rect{{area.pos.x, area.pos.y + halfSize.y}, halfSize} // top left
            };
        }


        void queryItems(const Rect& area, std::vector<uint32_t>& itemsList) {

            for (size_t i = 0; i < 4; i++)
            {
                if (mChilds[i]) {
                    if (mChilds[i]->mRect.overlaps(area)) {
                        mChilds[i]->queryItems(area, itemsList);
                    }
                }
            }

            for (auto& p : mItems) {
                itemsList.push_back(p.second);
            }
        }

        void items(std::list<QuadItem*>& itemsList) {
            for (auto& item : mItems)
                itemsList.push_back(&item);


            for (size_t i = 0; i < 4; i++)
                if (mChilds[i])  mChilds[i]->items(itemsList);
        }

        const Rect& getArea() { return mRect; }

        QuadTree* getQuadItemListRef(const Rect& rect) {
            auto curDepth = mDepth + 1;

            for (size_t i = 0; i < 4; i++)
            {
                if (mChildsArea[i].contains(rect)) {
                    if (curDepth < MAX_DEPTH) {

                        if (!mChilds[i]) {
                            mChilds[i] = std::make_shared<QuadTree>(mChildsArea[i], curDepth);
                        }
                        return mChilds[i]->getQuadItemListRef(rect);
                    }
                }
            }

            return this;
        }
    };

    /************** Quad Tree Container *****************************************************/
    template <typename T>
    struct QuadTreeItem {
        T* ref;
        uint32_t index;
        uint32_t qIndex;
        QuadTree* qRef;

        QuadTreeItem(T* rf, uint32_t i, uint32_t ii, QuadTree* qt): ref(rf), index(i), qIndex(ii), qRef(qt) {}
    };

    template <typename T>
    class QuadTreeContainer {
        using Container = std::vector<QuadTreeItem<T>>;

    private:
        Container mItems;

    public:
        QuadTree root;
        QuadTreeContainer(const Rect& area = { 0,0, 100, 100 }, const size_t depth = 0): root(area) { }
        ~QuadTreeContainer() {
            for (auto& item : mItems) {
                delete item.ref;
            }
        }
        void resize(const Rect area) {
            root.resize(area);
        }

        void reserve(size_t count) { mItems.reserve(count); }

        size_t size() const { return mItems.size(); }

        bool empty() const
        {
            return mItems.empty();
        }

        void clear()
        {
            root.clear();
            mItems.clear();
        }

        typename Container::iterator begin() { return mItems.begin(); }
        typename Container::iterator end() { return mItems.end(); }
        typename Container::const_iterator cbegin() { return mItems.cbegin(); }
        typename Container::const_iterator cend() { return mItems.cend(); }

        QuadTreeItem<T>& operator[](int index) {
            return mItems[index];
        }

        QuadTreeItem<T>& back() { return mItems.back(); }

        template <typename E, typename... TArgs>
        E* insert(TArgs... args)
        {
            E* item = new E(std::forward<TArgs>(args)...);
            // auto start = Time::micros();
            auto qRef = root.insert(mItems.size(), item->getRect());

            mItems.push_back({ item, mItems.size(), qRef.first, qRef.second });

            return item;
        }

        QuadTree* getQuadItemListRef(const Rect& rect) {
            return root.getQuadItemListRef(rect);
        }

        std::vector<uint32_t> query(const Rect& r)
        {
            // auto start = Time::micros();
            std::vector<uint32_t> items;
            items.reserve(1000);
            root.queryItems(r, items);

            std::vector<uint32_t> itemList;
            itemList.reserve(items.size());
            for (uint32_t i : items) {
                itemList.push_back(mItems[i].index);
            }
            // Logger::info("time: %llu", Time::micros() - start);
            return itemList;
        }
    };
} // namespace Plutus