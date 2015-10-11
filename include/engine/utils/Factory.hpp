#ifndef UTILS_FACTORY_HPP
#define UTILS_FACTORY_HPP

#include <cassert>
#include <map>

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
        // TODO: consider using rvalue references and std::forward
        Base* create(const ID& id, Args&... args) const
        {
            assert(_makers.find(id) != _makers.end());
            return _makers.find(id)->second(args...);
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

        void clear()
        {
            _makers.clear();
        }

        size_t size() const
        {
            return _makers.size();
        }

    private:
        std::map<ID, CreatorFunction> _makers;
};

#endif
