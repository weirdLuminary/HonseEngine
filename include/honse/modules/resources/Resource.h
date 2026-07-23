#pragma once
#include <memory>

template<typename T>
using Resource = std::shared_ptr<T>;