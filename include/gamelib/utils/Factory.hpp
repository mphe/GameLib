#ifndef UTILS_FACTORY_HPP
#define UTILS_FACTORY_HPP

#include <cassert>
#include <unordered_map>
#include <utility>

// TODO: Use std::function

namespace gamelib
{
    template <class Base, class ID, class... Args>
        class Factory
        {
            public:
                typedef Base*(*CreatorFunction)(Args...);

            public:
                template <class Derive>
                static Base* defaultCreator(Args... args)
                {
                    return new Derive(args...);
                }

            public:
                template <class... Args2>
                Base* create(const ID& id, Args2&&... args) const
                {
                    assert(_makers.find(id) != _makers.end());
                    return _makers.find(id)->second(std::forward<Args2>(args)...);
                }

                void add(const ID& id, CreatorFunction callback)
                {
                    _makers[id] = callback;
                }

                template <class Derive>
                void add(const ID& id)
                {
                    add(id, defaultCreator<Derive>);
                }

                void del(const ID& id)
                {
                    _makers.erase(id);
                }

                bool haskey(const ID& id) const
                {
                    return _makers.find(id) != _makers.end();
                }

                void clear()
                {
                    _makers.clear();
                }

                size_t size() const
                {
                    return _makers.size();
                }

            private:
                std::unordered_map<ID, CreatorFunction> _makers;
        };
}

#endif
