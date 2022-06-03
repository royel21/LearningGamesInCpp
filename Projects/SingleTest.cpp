#include <iostream>
#include <memory>

#include <vector>

struct Test {
    float a;
    float b;
};

int main(int argc, char const* argv[])
{
    std::vector<int> ints = { 1,2,3,4,5 };
    ints.insert(ints.begin(), 0);
    for (auto i : ints) {
        printf("%i ", i);
    }
    printf("\n");
    return 0;
}
