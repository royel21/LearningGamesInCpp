#include <iostream>
#include <memory>

struct Test {
    float a;
    float b;
};

int main(int argc, char const* argv[])
{
    Test t;
    printf("size: %zu - %zu, %i, %i", sizeof(Test), sizeof(t), offsetof(Test, b), &Test::b);
    return 0;
}
