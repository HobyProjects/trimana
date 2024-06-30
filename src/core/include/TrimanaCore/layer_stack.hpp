#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <concepts>

template <typename Ty>
class LayerStack
{
public:
    class iterator;
    class reverse_iterator;

    iterator begin()
    {
        return iterator(0, *this);
    }

    iterator end()
    {
        return iterator((m_stack[m_index].get() == nullptr) ? (m_index - 1) : m_index, *this);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator((m_stack[m_index].get() == nullptr) ? (m_index - 1) : m_index, *this);
    }

    reverse_iterator rend()
    {
        return reverse_iterator(0, *this);
    }

public:
    LayerStack() = default;
    ~LayerStack() = default;

    template <typename T, typename... Args>
    requires std::is_same<Ty, T>::value
    void push_front(Args &&...args)
    {
        T *ptr_v(new T(std::forward<Args>(args)...));
        if (m_index > 0)
        {
            for (int i = m_index; i > (m_index - m_index); i--)
            {
                m_stack[i] = std::move(m_stack[i - 1]);
            }

            std::unique_ptr<T> unique_ptr_v{ptr_v};
            m_stack[0] = std::move(unique_ptr_v);
            if (++m_index == m_stack.size())
                m_stack.emplace_back(std::unique_ptr<T>(nullptr));
        }
        else
        {
            std::unique_ptr<T> unique_ptr_v{ptr_v};
            m_stack.emplace_back(std::move(unique_ptr_v));
            m_index++;
        }
    }

    template <typename T, typename... Args>
        requires std::is_same<Ty, T>::value
    void push_back(Args &&...args)
    {
        T *ptr_v(new T(std::forward<Args>(args)...));
        std::unique_ptr<T> unique_ptr_v{ptr_v};
        m_stack.emplace_back(std::move(unique_ptr_v));
        m_index++;
    }

    void pop_front()
    {
        Ty *ptr_v = m_stack[0].release();
        delete ptr_v;
        ptr_v = nullptr;
        m_index = 0;
        for (unsigned int i = 1; i < m_stack.size(); i++)
        {
            m_stack[m_index++] = std::move(m_stack[i]);
        }
    }

    void pop_back()
    {
        if (m_stack[m_index].get() == nullptr)
        {
            m_index--;
            pop_back();
        }
        else
        {
            Ty *ptr_v = m_stack[m_index].release();
            delete ptr_v;
            m_stack[m_index] = nullptr;
        }
    }

    int size() const
    {
        return (m_index - 1);
    }

    Ty *operator[](int index)
    {
        return m_stack[index].get();
    }

private:
    int m_index{NULL};
    std::vector<std::unique_ptr<Ty>> m_stack;
};

template <typename Ty>
class LayerStack<Ty>::iterator
{
public:
    iterator(int index, LayerStack &stack) : m_index(index), m_stack(stack) {}
    ~iterator() = default;

    iterator &operator++(int)
    {
        ++m_index;
        return *this;
    }

    iterator &operator++()
    {
        ++m_index;
        return *this;
    }

    Ty *operator->() const
    {
        return m_stack[m_index];
    }

    bool operator!=(const iterator &other) const
    {
        return (m_index != other.m_index);
    }

private:
    int m_index{NULL};
    LayerStack &m_stack{nullptr};
};

template <typename Ty>
class LayerStack<Ty>::reverse_iterator
{
public:
    reverse_iterator(int index, LayerStack &stack) : m_rindex(index), m_rstack(stack) {}
    ~reverse_iterator() = default;

    reverse_iterator &operator--(int)
    {
        --m_rindex;
        return *this;
    }

    reverse_iterator &operator--()
    {
        --m_rindex;
        return *this;
    }

    Ty *operator->() const
    {
        return m_rstack[m_rindex];
    }

    bool operator!=(const reverse_iterator &other) const
    {
        return (m_rindex != other.m_rindex);
    }

private:
    int m_rindex{NULL};
    LayerStack &m_rstack{nullptr};
};