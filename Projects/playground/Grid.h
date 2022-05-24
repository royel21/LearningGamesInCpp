#pragma once
#include <cstdint>
#include <vector>

#include <Math/PMath.h>
#include <Math/Vectors.h>
#include <Physics/Shapes.h>

namespace Plutus
{
#define Cell std::vector<CellItem>

    struct CellItem {
        uint32_t itemIndex;
        uint32_t cellIndex;
        Cell* cell;
        CellItem(uint32_t ii, uint32_t ci, Cell* cll) : itemIndex(ii), cellIndex(ci), cell(cll) {}
    };

    template <typename T>
    class Grid
    {
    public:
        Grid() = default;
        void init(uint32_t w, uint32_t h, uint32_t numCells)
        {
            mCellSize = numCells;
            mCells.clear();
            mNumXCells = (int)ceil((float)w / numCells);
            mNumYCells = (int)ceil((float)h / numCells);

            // Allocate all the cells
            const int ITEMS_TO_RESERVE = 20;
            mCells.resize(mNumXCells * mNumYCells);

            for (size_t i = 0; i < mCells.size(); i++) {
                mCells[i].reserve(ITEMS_TO_RESERVE);
            }
        }

        template <typename E, typename... TArgs>
        E* addItem(TArgs &&... args)
        {
            E* item = new E(std::forward<TArgs>(args)...);
            mItems.push_back(item);

            auto cell = getCell(item->pos);

            cell->push_back({ mItems.size() - 1, cell->size(), cell });

            return item;
        }

        void moveItem(Shape* item, Cell* cell)
        {
            // cell->balls.push_back(ball);
            // ball->ownerCell = cell;
            // ball->cellVectorIndex = cell->balls.size() - 1;
        }

        Cell* getCell(float x, float y)
        {
            x = CLAMP(x, 0, mNumXCells - 1);
            y = CLAMP(y, 0, mNumXCells - 1);

            return &mCells[int(y) * mNumXCells + int(x)];
        }

        Cell* getCell(const Vec2f& pos)
        {
            int cellX = (int)(pos.x / mCellSize);
            int cellY = (int)(pos.y / mCellSize);

            return getCell(pos.x, pos.y);
        }

        void removeItem(CellItem* item)
        {
            // mItems[item->itemIndex] = mItems.back();
            // mItems.pop_back();

            // auto& cell = (*item->cell);
            // // Normal vector swap
            // cell[item->index] = cell.back();
            // cell[item->index].itemIndex = item->itemIndex;
            // cell[item->index].index = item.index;

            // cell.pop_back();

            // // Update vector index
            // if ((size_t)item->index < cell.size()) {
            //     auto& cellitem = cell[item->index];
            //     cellitem.index = item->index;
            // }
            // // Set the index of ball to -1
            // item->index = -1;
            // item->cell = nullptr;
        }


        T& operator[](int index) { return mItems[index]; }

        typename std::vector<T>::iterator begin() { return mItems.begin(); }
        typename std::vector<T>::iterator end() { return mItems.end(); }
        typename std::vector<T>::const_iterator cbegin() { return mItems.cbegin(); }
        typename std::vector<T>::const_iterator cend() { return mItems.cend(); }
        T& back() { return items.back(); }
        size_t size() { return items.size(); }

    private:
        std::vector<Cell> mCells;
        std::vector<T> mItems;

        int mNumXCells;
        int mNumYCells;
        int mCellSize;
    };

} // namespace Plutus