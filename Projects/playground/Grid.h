#pragma once
#include <cstdint>
#include <vector>
#include <Math/Vectors.h>

namespace Plutus
{
    template<typename T>
    struct CellItem {
        T* object;
        uint32_t index;
        std::vector<CellItem>* cell;
    };

    using Cell = std::vector<CellItem>;

    template<typename T>
    class Grid
    {
    public:
        Grid();
        void init(uint32_t w, uint32_t h, uint32_t numCells)
        {
            mCells.clear();
            mNumXCells = (int)ceil((float)w / numCells);
            mNumYCells = (int)ceil((float)h / numCells);

            // Allocate all the cells
            const int ITEMS_TO_RESERVE = 20;
            mCells.resize(mNumXCells * mNumYCells);

            for (size_t i = 0; i < mCells.size(); i++) {
                // mCells[i]
            }
        }

        void addItem(T* item)
        {

        }

        void moveItem(T* item, Cell* cell)
        {

        }

        Cell* getCell(const Vec2f& pos)
        {

        }

        void removeItem(T* item)
        {

        }

    private:
        std::vector<Cell> mCells;
        int mNumXCells;
        int mNumYCells;
    };
} // namespace Plutus