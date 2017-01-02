#pragma once

#include <functional>
#include <list>

namespace react {

    template <typename ... Args>
    class connection {
    public:
        using function  = std::function<void (Args const & ...)>;
        using predicate = std::function<bool (Args const & ...)>;

    public:
        class list {
        public:
            void operator() (Args const & ... args) const {
                for (auto const & connection : m_connections) {
                    connection(args...);
                }
            }

            connection & emplace_back() {
                auto it = m_connections.emplace(m_connections.end());
                it->m_disconnect = [=]() { m_connections.erase(it); };
                return *it;
            }

            void disconnect() const {
                while (!m_connections.empty()) {
                    m_connections.front().disconnect();
                }
            }

        private:
            std::list<connection> m_connections;
        };

    public:
        void operator() (Args const & ... args) const {
            if (m_function) {
                if (!m_predicate || m_predicate(args...)) {
                    m_function(args...);
                    if (m_once) disconnect();
                    // possible dead
                }
            }
        }

        void disconnect() const {
            if (m_disconnect) {
                m_disconnect();
            }
        }

        connection & set_once() {
            m_once = true;
            return *this;
        }

        connection & set_predicate(std::function<bool (Args const & ...)> const & predicate) {
            m_predicate = predicate;
            return *this;
        }

        connection & set_function(std::function<void (Args const & ...)> const & function) {
            m_function = function;
            return *this;
        }

    private:
        std::function<void (Args const & ...)> m_function;
        std::function<bool (Args const & ...)> m_predicate;
        std::function<void ()> m_disconnect;
        bool m_once { false };
    };

}
