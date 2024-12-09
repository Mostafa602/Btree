#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class Vector
{
private:
    T *data;
    int capacity;
    int size;

public:
    Vector()
    {
        capacity = 1;
        size = 0;
        data = new T[capacity];
    }

    Vector(const Vector<T> &other)
    {
        capacity = other.capacity;
        size = other.size;
        data = new T[capacity];
        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    Vector<T> &operator=(const Vector<T> &other)
    {
        if (this != &other)
        {
            delete[] data;

            capacity = other.capacity;
            size = other.size;
            data = new T[capacity];
            for (int i = 0; i < size; i++)
            {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~Vector()
    {
        if (data)
        {
            delete[] data;
            data = nullptr;
        }
    }

    void resize()
    {
        capacity *= 2;
        T *newData = new T[capacity];
        for (int i = 0; i < size; i++)
        {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

    void resize(int newCapacity)
    {
        if (newCapacity <= 0)
            return;
        capacity = newCapacity;
        T *newData = new T[capacity];
        int minimum = std::min(size, newCapacity);
        for (int i = 0; i < minimum; i++)
        {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        size = minimum;
    }

    T *begin()
    {
        return data;
    }

    T *end()
    {
        return data + size;
    }

    void push_back(T element)
    {
        if (size == capacity)
            resize();
        data[size++] = element;
    }

    void insert(int index, T element)
    {
        if (index < 0 || index > size)
        {
            throw std::out_of_range("Index out of range");
        }
        if (size == capacity)
        {
            resize();
        }
        for (int i = size; i > index; i--)
        {
            data[i] = data[i - 1];
        }
        data[index] = element;
        size++;
    }

    int Size()
    {
        return size;
    }

    T &operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void assign(T *begin, T *end)
    {
        int newSize = end - begin;
        if (newSize > capacity)
        {
            capacity = newSize;
            delete[] data;
            data = new T[capacity];
        }
        for (int i = 0; i < newSize; i++)
        {
            data[i] = begin[i];
        }
        size = newSize;
    }
};
