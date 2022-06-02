#include <iostream>

int cal(auto m, auto n) {
    return m * n;
}

int main(int argc, char const* argv[])
{
    printf("test val:%i\n", cal(20, 2));
    return 0;
}
