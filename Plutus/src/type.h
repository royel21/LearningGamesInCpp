#pragma once
#include <memory>

using u8 = unsigned char;
using u16 = unsigned char;
using u32 = unsigned int;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}