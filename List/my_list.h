#pragma once
#include <initializer_list>
#include <memory>
#include <iterator>
#include <tuple>
#include <concepts>

template<typename T>
struct node
{
    node* next;
    node* prev;
    T data;
    node() : next(nullptr), prev(nullptr) {}

    node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template<typename T>
class MyList
{
    node<T>* _end;
    node<T>* _head;
    size_t _size = 0;
public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    class iterator
    {
        
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = value_type;
        using difference_type   = difference_type;
        using pointer           = pointer;
        using reference         = reference;

        node<T>* ptr;

        iterator(node<T>* point)
        {
            ptr = point;
        }

        bool operator==(const iterator& other) const
        {
            return this->ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const
        {
            return !(this->ptr == other.ptr);
        }
        pointer   operator->()
        {
            return ptr;
        }
        iterator& operator--()
        {
            ptr = ptr->prev;
            return *this;
        }
        iterator  operator--(int)
        {
            iterator ret = *this;
            --(*this);
            return ret;
        }
        iterator& operator++()
        {
            ptr = ptr->next;
            return *this;
        }
        iterator  operator++(int)
        {
            iterator ret = *this;
            ++(*this);
            return ret;
        }
        reference operator*()
        {
            return (*ptr).data;
        }
    };

    using reverse_iterator = std::reverse_iterator<iterator>;

    MyList()
    {
        void* false_end = operator new(sizeof(node<T>));
        _end = static_cast<node<T>*>(false_end);
        _end->next = _end;
        _end->prev = _end;
        _head = _end;
    }
    MyList(MyList& other)
    {
        void* false_end = operator new(sizeof(node<T>));
        _end = static_cast<node<T>*>(false_end);
        _end->next = _end;
        _end->prev = _end;
        _head = _end;
        iterator ptr = other.begin();
        while (ptr != other.end())
        {
            this->push_back(*ptr);
            ++ptr;
        }
    }
    MyList(std::initializer_list<T> init)
    {
        void* false_end = operator new(sizeof(node<T>));
        _end = static_cast<node<T>*>(false_end);
        _end->next = _end;
        _end->prev = _end;
        _head = _end;
        auto it = init.begin();
        while (it != init.end())
        {
            this->push_back(*it);
            ++it;
        }
    }

    MyList& operator=(MyList& other)
    {
        void* false_end = operator new(sizeof(node<T>));
        _end = static_cast<node<T>*>(false_end);
        _end->next = _end;
        _end->prev = _end;
        _head = _end;
        clear();
        auto it = other.begin();
        while (it != other.end())
        {
            push_back(*it);
            ++it;
        }
        return *this;
    }
    MyList& operator=(std::initializer_list<T> ilist)
    {
        void* false_end = operator new(sizeof(node<T>));
        _end = static_cast<node<T>*>(false_end);
        _end->next = _end;
        _end->prev = _end;
        _head = _end;
        clear();
        for (const T& value : ilist)
        {
            push_back(value);
        }
        return *this;
    }

    reference front()
    {
        return _head->data;
    }
    reference back()
    {
        return _end->prev->data;
    }

    iterator begin() noexcept
    {
        return _head;
    }
    iterator end() noexcept
    {
        return _end;
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(iterator(this->_end));
    }
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(iterator(_head));
    }

    bool empty() const noexcept
    {
        return _head == _end;
    }
    size_type size() const noexcept
    {
        return _size;
    }

    void clear() noexcept
    {
        auto b = this->begin();
        auto e = this->end();
        while (b != e)
        {
            b = this->erase(b);
        }
        _size = 0;
        _head = _end;
        _end->next = _end;
        _end->prev = _end;
    }

    iterator insert(iterator pos, const T& value)
    {
        node<T>* nodeP = pos.ptr;
        node<T>* prev = nodeP->prev;
        node<T>* new_node = new node<T>(value);
        new_node->next = nodeP;
        new_node->prev = prev;
        prev->next = new_node;
        nodeP->prev = new_node;
        ++_size;
        if (nodeP = _head) _head = new_node;
        return new_node;

    }

    iterator erase(iterator pos)
    {

        if (pos == this->end()) return pos;
            node<T>* nodeP = pos.ptr;
            if (!nodeP) return this->end();
            auto nextPos = nodeP->next;
            if (nodeP == _head) _head = nodeP->next;
            node<T>* prev = nodeP->prev;
            node<T>* next = nodeP->next;
            next->prev = prev;
            prev->next = next;

            delete nodeP;
            --_size;
            return nextPos;
        
    }
    iterator erase(iterator first, iterator last)
    {
        while (first != last)
        {
            first = this->erase(first);   
        }
        return first;
    }

    void push_back(const T& value)
    {
        if (_head == _end)
        {
            _head = new node<T>(value);
            _head->next = _end;
            _head->prev = _end;
            _end->prev = _head;
        }
        else
        {
            node<T>* prev = _end->prev;
            node<T>* next = new node<T>(value);
            prev->next = next;
            _end->prev = next;
            next->prev = prev;
            next->next = _end;
        }
        ++_size;
    }
    void pop_back()
    {
        if (_end->prev != _head)
        {
            node<T>* erase = _end->prev;
            _end->prev = erase->prev;
            erase->prev->next = _end;
            erase->next = nullptr;
            erase->prev = nullptr;
            delete erase;
            --_size;
        }
        else if (_end->prev == _head && _head != _end)
        {
            node<T>* erase = _end->prev;
            _end->prev = _end;
            _head = _end;
            erase->next = nullptr;
            erase->prev = nullptr;
            delete erase;
            --_size;
        }
    }
    void push_front(const T& value)
    {
        if (_head == _end)
        {
            _head = new node<T>(value);
            _head->next = _end;
            _head->prev = _end;
            _end->prev = _head;
        }
        else
        {
            node<T>* next = _head;
            node<T>* new_head = new node<T>(value);
            next->prev = new_head;
            new_head->prev = _end;
            new_head->next = next;
            _head = new_head;
        }
        ++_size;
    }
    void pop_front()
    {
        if (_head != _end)
        {
            node<T>* new_head = _head->next;
            _head->prev = nullptr;
            _head->next = nullptr;
            new_head->prev = _end;
            _head = new_head;
            --_size;
        }
    }

    void reverse() noexcept
    {
        if (empty()) return;
        node<T>* current = _head;
        node<T>* temp;
        do {
            temp = current->next; 
            current->next = current->prev;
            current->prev = temp; 
            current = temp; 
        } while (current != _end); 

   
        std::swap(_head, _end->prev);
    }

    // TODO: добавить методы из условия лабы

    

    int countPos() //requires std::is_arithmetic<T>
    {
        auto b = this->begin();
        auto e = this->end();
        int res = 0;
        while (b != e)
        {
            if (*b > 0)
            {
                ++res;
            }
            ++b;
        }
        return res;
    }

    std::tuple<int, int> countMax() //requires std::is_arithmetic<T>
    {
        if (this->empty()) return std::make_tuple<int, int>(-1, -1);
        auto b = this->begin();
        auto e = this->end();
        int res = 0;
        int max = *b;
        while (b != e)
        {
            if (*b > max)
            {
                max = *b;
                res = 1;
            }
            else if (*b == max) ++res;
            ++b;
        }
        return std::make_tuple<int, int>(std::move(max), std::move(res));
    }

    std::pair<MyList<T>, MyList<T>> split() //requires std::is_arithmetic<T>
    {
        MyList<T> belowZ;
        MyList<T> aboveZ;
        auto b = this->begin();
        auto e = this->end();
        while (b != e)
        {
            if (*b >= 0)
            {
                aboveZ.push_back(*b);
            }
            else belowZ.push_back(*b);
            ++b;
        }
        return std::pair<MyList<T>, MyList<T>>(belowZ, aboveZ);
    }

    ~MyList()
    {
        clear();
        operator delete(_end, sizeof(node<T>));
    }

    // ____________________________ БОНУСНОЕ ЗАДАНИЕ ____________________________
    //     Для бонусного задания необходимо реализовать конструкторы, работающие с move семантикой
    //MyList(MyList&& other) noexcept
    //{
    //    _head = other._head;
    //    _end = other._end;
    //    _size = other._size;
    //    other._head = other._end = nullptr;
    //    other._size = 0;
    //}
    // MyList& operator=(MyList&& other) noexcept;

    //     И методы добавления элементов

    // iterator insert(iterator pos, T&& value);
    // void push_back(T&& value);
    // void push_front(T&& value);

    //     ИЛИ использовать универсальную ссылку и std::forward вместо обычных методов (https://en.cppreference.com/w/cpp/utility/forward)
    //     Подробное введение в rvalue-ссылки (https://habr.com/ru/articles/322132/)

    // template<typename U>
    // iterator insert(iterator pos, U&& value);
    // template<typename U>
    // void push_back(U&& value);
    // template<typename U>
    // void push_front(U&& value);
};
