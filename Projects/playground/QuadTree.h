#pragma once
#include <Math/Rect.h>

#include <array>
#include <memory>

#include <vector>
#include <list>
#include <utility>

#include <Log/Logger.h>

namespace Plutus
{
    constexpr size_t MAX_DEPTH = 20;

    class QuadTree;

    struct QuadTreeItem {
        Rect rect;
        uint32_t index;
        uint32_t itemIndex;
        std::vector<QuadTreeItem>* owner;
        QuadTreeItem(const Rect& r, uint32_t i, std::vector<QuadTreeItem>* q) : rect(r), index(i), owner(q) {}
    };

    class QuadTree
    {
    public:
        QuadTree(const Rect& area = { 0,0, 100, 100 }, const size_t depth = 0) : mDepth(depth) { resize(area); }

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

        void insert(uint32_t index, const Rect& itemSize) {
            auto curDepth = mDepth + 1;

            for (size_t i = 0; i < 4; i++)
            {
                if (mChildsArea[i].contains(itemSize)) {
                    if (curDepth < MAX_DEPTH) {

                        if (!mChilds[i]) {
                            mChilds[i] = std::make_shared<QuadTree>(mChildsArea[i], curDepth);
                        }
                        mChilds[i]->insert(index, itemSize);
                        return;
                    }
                }
            }
            mItems.push_back({ itemSize, index, &mItems });
            mItems.back().itemIndex = mItems.size() - 1;
        }

        void resize(const Rect& area) {
            clear();
            mRect = area;

            Vec2f halfSize = area.size / 2.0f;

            mChildsArea = {
                Rect{area.pos, halfSize}, // bottom left
                Rect{{area.pos.x + halfSize.x, area.pos.y}, halfSize}, // bottom right
                Rect{{area.pos.x, area.pos.y + halfSize.y}, halfSize}, // top left
                Rect{area.pos + halfSize, halfSize} // top right
            };
        }


        void queryItems(const Rect& area, std::list<QuadTreeItem*>& itemsList) {

            for (auto& p : mItems) {
                if (area.overlaps(p.rect))
                    itemsList.push_back(&p);
            }

            for (size_t i = 0; i < 4; i++)
            {
                if (mChilds[i]) {
                    if (area.contains(mChildsArea[i])) {
                        mChilds[i]->items(itemsList);
                    }
                    else if (mChildsArea[i].overlaps(area)) {
                        mChilds[i]->queryItems(area, itemsList);
                    }
                }
            }
        }

        void items(std::list<QuadTreeItem*>& itemsList) {
            for (auto& p : mItems)
                itemsList.push_back(&p);

            for (size_t i = 0; i < 4; i++)
                if (mChilds[i])  mChilds[i]->items(itemsList);
        }

        const Rect& getArea() { return mRect; }

    private:
        size_t mDepth = 0;
        //Area of root quad tree
        Rect mRect;

        // area of the 4 children
        std::array<Rect, 4> mChildsArea{};

        // 4 child of the quad tree
        std::array<std::shared_ptr<QuadTree>, 4> mChilds{};
        //Items belonging to this qtree
        std::vector<QuadTreeItem> mItems;
    };

    /************** Quad Tree Container *****************************************************/

    template <typename T>
    class QuadTreeContainer {
        using Container = std::vector<T>;

    private:
        Container mAllItems;
        QuadTree root;

    public:
        QuadTreeContainer(const Rect& area = { 0,0, 100, 100 }, const size_t depth = 0) : root(area) { }

        void resize(const Rect area) {
            root.resize(area);
        }

        void reserve(size_t count) { mAllItems.reserve(count); }

        size_t size() const { return mAllItems.size(); }

        bool empty() const
        {
            return mAllItems.empty();
        }

        void clear()
        {
            root.clear();
            mAllItems.clear();
        }

        typename Container::iterator begin() { return mAllItems.begin(); }
        typename Container::iterator end() { return mAllItems.end(); }
        typename Container::const_iterator cbegin() { return mAllItems.cbegin(); }
        typename Container::const_iterator cend() { return mAllItems.cend(); }

        T& QuadTreeContainer::operator[](int index) {
            return mAllItems[index];
        }

        T& QuadTreeContainer::back() { return mAllItems.back(); }

        void insert(const T& item, const Rect& itemSize)
        {
            mAllItems.push_back(item);
            // auto start = Time::micros();
            root.insert(mAllItems.size() - 1, itemSize);
            // Logger::info("time: %llu %zu", Time::micros() - start, sizeof(it));
        }

        std::list<QuadTreeItem*> query(const Rect& r)
        {
            std::list<QuadTreeItem*> items;

            root.queryItems(r, items);
            return items;
        }


    };
} // namespace Plutus