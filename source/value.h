#pragma once

#include "connection.h"

namespace react {

    template <typename T>
    class value {
    public:
        using connection = react::connection<T, T>;

    public:
        value() = default;
        value(T const & value) : m_value(value) {}

        // dereference operators
        operator T const & () const { return m_value; }
        T const & operator * () const { return m_value; }
        T const * operator -> () const { return &m_value; }

        // assignment operator
        value & operator = (T const & value) {
            if (value != m_value) {
                T previous = value;
                std::swap(previous, m_value);
                m_connections(previous, m_value);
            }
            return *this;
        }

        // math operators
        value & operator += (T const & value) { return (*this = m_value + value); }
        value & operator -= (T const & value) { return (*this = m_value - value); }
        value & operator *= (T const & value) { return (*this = m_value * value); }
        value & operator /= (T const & value) { return (*this = m_value / value); }

        void notify() {
            m_connections(m_value, m_value);
        }

        connection & connect() {
            return m_connections.emplace_back();
        }

        connection & connect(std::function<void (T const &, T const &)> const & function) {
            return connect().set_function(function);
        }

        connection & connect(std::function<void (T const &)> const & function) {
            return connect([=](T const & previous, T const & current) {
                    return function(current);
                });
        }

    private:
        T m_value {};
        typename connection::list m_connections;
    };

}

template <typename T = typename std::enable_if<true, std::ostream>>
std::ostream & operator << (std::ostream & stream, react::value<T> const & value) {
    stream << static_cast<T>(value);
    return stream;
}
