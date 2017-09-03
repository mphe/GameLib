#ifndef GAMELIB_FACTORY_HPP
#define GAMELIB_FACTORY_HPP

#include <cassert>
#include <memory>
#include <unordered_map>

namespace gamelib
{
    template <typename Base, typename Key, typename... Args>
    class Factory
    {
        public:
            typedef std::unique_ptr<Base> BasePtr;

        public:
            typedef BasePtr(*CreatorFunction)(Args...);

        public:
            template <typename Derive>
            static BasePtr defaultCreator(Args&&... args)
            {
                return BasePtr(new Derive(std::forward<Args>(args)...));
            }

        public:
            BasePtr create(const Key& id, Args&&... args) const
            {
                assert(_makers.find(id) != _makers.end());
                return _makers.find(id)->second(std::forward<Args>(args)...);
            }

            void add(const Key& id, CreatorFunction callback)
            {
                _makers[id] = callback;
            }

            template <typename Derive>
            void add(const Key& id)
            {
                add(id, defaultCreator<Derive>);
            }

            void remove(const Key& id)
            {
                _makers.erase(id);
            }

            bool hasKey(const Key& id) const
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
            std::unordered_map<Key, CreatorFunction> _makers;
    };
}

#endif
