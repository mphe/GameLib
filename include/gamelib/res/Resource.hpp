#ifndef GAMELIB_RESOURCE_HPP
#define GAMELIB_RESOURCE_HPP

#include <memory>
#include "../Identifier.hpp"

namespace gamelib
{
    class BaseResource : public Identifiable
    {
        public:
            virtual ~BaseResource() {}

            template <typename T>
            T& as()
            {
                return *static_cast<T*>(this);
            }

            template <typename T>
            const T& as() const
            {
                return *static_cast<const T*>(this);
            }
    };

    typedef std::shared_ptr<BaseResource> ResourcePtr;

    template <typename T, ID idval>
    class Resource : public Identifier<idval, BaseResource>
    {
        public:
            Resource() {}
            virtual ~Resource() {}

            template <typename... Args>
            Resource(Args&&... args) : res(std::forward<Args>(args)...) {}

            template <typename... Args>
            static ResourcePtr create(Args&&... args)
            {
                return ResourcePtr(
                        new Resource<T, idval>(std::forward<Args>(args)...));
            }

        public:
            T res;
    };
}

#endif
