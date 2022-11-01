#pragma once
template <typename T>
class BufferAttribute
{
public:
    BufferAttribute(const vector<T> arr) : array(move(arr))
    {
    }
    vector<T> array;
    bool normalized = false;
};