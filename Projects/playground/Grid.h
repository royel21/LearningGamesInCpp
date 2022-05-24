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
                mCells[i].reserve(ITEMS_TO_RESERVE);
            }
        }

        void addItem(T* item)
        {

        }

        void moveItem(T* item, std::vector < CellItem<T>>* cell)
        {

        }

        std::vector < CellItem<T>* getCell(const Vec2f& pos)
        {

        }

        void removeItem(T* item)
        {

        }

    private:
        std::vector < std::vector<CellItem<T> > mCells;
        int mNumXCells;
        int mNumYCells;
    };
} // namespace Plutus