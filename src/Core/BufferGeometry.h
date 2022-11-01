#pragma once
#include <unordered_map>
#include <string>
#include <Core/BufferAttribute.h>
using namespace std;
template <typename T>
class BufferGeometry
{
public:
    unordered_map<string, BufferAttribute<T>> attributes;
};