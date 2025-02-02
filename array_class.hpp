#pragma once
#include <iostream>
#include "exception_class.h"

template<typename T>
class Array
{
public:
    Array() = default;

    Array(size_t length) : _length{ length }
    {
        _data = new T[_length]{};
    };

    Array(const Array& copy)
    {
        reallocate(copy.getLength());
        for (size_t element = 0; i < _length; ++element)
        {
            _data[element] = copy._data[element];
        }
    };

    ~Array()
    {
        delete[] _data;
    };

    T& operator [] (int element)
    {
        try {
            if (element >= 0 && element < _length)
            {
                return _data[element];
            }
            else
            {
                throw Bad_Length();
            }
        }
        catch (exception& error_Length)
        {
            std::cout << error_Length.what() << "The element " << i << " is out of the array." << endl;
        }
    };

    Array& operator = (const Array& array)
    {
        if (&array == this)
        {
            return *this;
        }
        reallocate(array.getLength());
        for (size_t element = 0; element < _length; ++element)
        {
            _data[element] = array._data[element];
        }
        return *this;
    };

   int getLength() const
    {
        return _length;
    };

    T getElement(int element) const
    {
        return _data[element];
    };

    bool checkRange(size_t element)
    {
        try
        {
            if (element < 0 || element >= getLength())
            {
                throw Bad_Range();
            }
        }
        catch (exception& error_Range)
        {
            std::cout << error_Range.what() << "The entered number is outside the array.\n";
            return false;
        }
        return true;
    };

    bool checkLength(size_t newLenght)
    {
        try
        {
            if (newLenght <= 0 || newLenght > 1000)
            {
                throw Bad_Length();
            }
        }
        catch (exception& error_Length)
        {
            std::cout << error_Length.what() << "The container size should be in the range from 1 to 1000.\n";
            return false;
        }
        return true;
    };

   void addElement(T newElement)
    {
        T* data = new T[_length + 1];
        for (size_t element = 0; element < _length; ++element)
        {
            data[element] = _data[element];
        }
        data[++_length] = newElement;

        delete[] _data;
        _data = data;
    };

    /*void remove(int element)
    {
        if (checkRange(element))
        {
            if (_length == 1)
            {
                erase();
                return;
            }
            T* data = new T[_length - 1];
            for (int before = 0; before < element; ++before)
            {
                data[before] = _data[before];
            }
            for (int after = element + 1; after < _length; ++after)
            {
                data[after - 1] = _data[after];
            }
            delete[] _data;
            _data = data;
            --_length;
        }
    };*/

    void search(T login, T password)
    {
        //size_t* index = new size_t[_length];
        //int count = 0;
        for (size_t element = 0; i < _length; ++element)
        {
            if (_data[element] == value)
            {
                index[count] = element;
                ++count;
            }
        }
       /* std::cout << "Found elements with the value " << value << ": " << count << "\n";
        if (count == 0) return;
        for (int element = 0; element < count; ++element)
        {
            std::cout << "Element " << index[element] << ": " << value << "\n";
        }
        std::cout << "To delete element, enter \"d\", to clear the element, enter \"c\":  \n";
        char goOn = '0';
        std::cin >> goOn;
        if (goOn == 'd' || goOn == 'c')
        {
            for (int element = count - 1; element >= 0; --element)
            {
                if (checkRange(index[element]))
                {
                    if (goOn == 'd')
                    {
                        std::cout << "Deleting an element " << index[element] << ".\n";
                        remove(index[i]);
                    }
                    else
                    {
                        std::cout << "Clearing the element " << index[element] << ".\n";
                        _data[index[element]] = 0;
                    }
                }
            }
        }
        delete[] index;*/
    };

    void resize(size_t newLength)
    {
        if (newLength == _length)
        {
            return;
        }
        T* data = new T[newLength];
        for (size_t element = 0; element < newLength; ++element)
        {
            if (element < _length)
            {
                data[element] = _data[element];
            }
            else
            {
                data[element] = T();
            }
        }
        delete[] _data;
        _data = data;
        _length = newLength;
    };

    void reallocate(size_t newLength)
    {
        erase();
        if (newLength <= 0)
        {
            return;
        }
        _data = new T[newLength];
        _length = newLength;
    };

    void erase()
    {
        delete[] _data;
        _data = nullptr;
        _length = 0;
    };

 private:
     size_t _length{ 0 };
     T _data{ nullptr };        
};

