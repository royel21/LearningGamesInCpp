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
    constexpr size_t MAX_DEPTH = 4;

    template<typename T>
    class QuadTree
    {
    private:
        size_t mDepth = 0;
        //Area of root quad tree
        Rect mRect;

        // area of the 4 children
        std::array<Rect, 4> mChildsArea{};

        // 4 child of the quad tree
        std::array<std::shared_ptr<QuadTree<T>>, 4> mChilds{};
        //Items belonging to this qtree
        std::vector<std::pair<Rect, T>> mItems;

    public:
        QuadTree(const Rect& area = { 0,0, 100, 100 }, const size_t depth = 0): mDepth(depth) { resize(area); }

        void getChilds(std::vector<Rect*>& childs) {
            for (size_t i = 0; i < 4; i++) {
                if (mChilds[i]) {
                    mChilds[i]->getChilds(childs);
                }
            }
            childs.push_back(&mRect);
        }

        void clear() {
            mItems.clear();

            for (size_t i = 0; i < 4; i++) {
                if (mChilds[i]) {
                    mChilds[i]->clear();
                    mChilds[i].reset();
                }

            }
        }

        size_t size() {
            size_t nCount = mItems.size();
            for (size_t i = 0; i < 4; i++)
                if (mChilds[i]) nCount += mChilds[i]->size();

            return nCount;
        }

        void insert(const T& item, const Rect& itemSize) {
            auto curDepth = mDepth + 1;

            if (curDepth < MAX_DEPTH) {
                for (size_t i = 0; i < 4; i++)
                {
                    if (mChildsArea[i].contains(itemSize)) {
                        //if child don't exit create it
                        if (!mChilds[i]) {
                            mChilds[i] = std::make_shared<QuadTree<T>>(mChildsArea[i], curDepth);
                        }
                        //insert item on child
                        mChilds[i]->insert(item, itemSize);
                        return;
                    }
                }
            }

            mItems.push_back({ itemSize, item });
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

        std::list<T> query(const Rect& area) const {
            std::list<T> listItems;
            queryItems(area, listItems);
            return listItems;
        }

        void queryItems(const Rect& area, std::list<T>& itemsList) const {
            for (const auto& p : mItems) {
                if (area.overlaps(p.first))
                    itemsList.push_back(p.second);
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

        void items(std::list<T>& itemsList) {
            for (const auto& p : mItems)
                itemsList.push_back(p.second);

            for (size_t i = 0; i < 4; i++)
                if (mChilds[i])  mChilds[i]->items(itemsList);
        }

        const Rect& getArea() { return mRect; }
    };

    /************** Quad Tree Container *****************************************************/

    template <typename T>
    class QuadTreeContainer {
        using Container = std::vector<T>;

    private:
        Container mAllItems;
        QuadTree<uint32_t> root;

    public:
        QuadTreeContainer(const Rect& area = { 0,0, 100, 100 }, const size_t depth = 0): root(area) { }

        std::vector<Rect*> getQTRects() {
            std::vector<Rect*> rects;
            root.getChilds(rects);
            return rects;
        }

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

        void insert(const T& item, const Rect& itemSize)
        {
            mAllItems.push_back(item);
            // auto start = Time::micros();
            root.insert(mAllItems.size() - 1, itemSize);
            // Logger::info("time: %llu %zu", Time::micros() - start, sizeof(it));
        }

        std::vector<T*> query(const Rect& r)
        {
            std::list<uint32_t> items;
            std::vector<T*> founds;

            root.queryItems(r, items);
            founds.reserve(items.size());

            for (auto i : items) {
                founds.push_back(&mAllItems[i]);
            }
            return founds;
        }


    };
} // namespace Plutus