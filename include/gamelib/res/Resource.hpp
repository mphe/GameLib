#ifndef GAMELIB_RESOURCE_HPP
#define GAMELIB_RESOURCE_HPP

#include <memory>
#include "../Identifier.hpp"

namespace gamelib
{
    class BaseResource;


    /*
     * A handle to a Resource.
     * It's basically a wrapper around std::shared_ptr with some extra
     * functionality to allow easy access to the underlying resource.
     * To cast between BaseResource and the actual resource as() can be used.
     * The dereference operators (*,->) provide access to the raw resource
     * data.
     * To get the resource container (the Resource<T, ID> based object), use
     * getResource().
     */
    template <typename T>
    class ResourceHandle
    {
        private:
            typedef typename T::WrappedType WrappedType;

        public:
            ResourceHandle() {}
            ResourceHandle(const ResourceHandle<T>& h) : _res(h._res) {}
            ResourceHandle(ResourceHandle<T>&& h) : _res(std::move(h._res)) {}

            template <typename... Args, typename = typename std::enable_if<std::is_constructible<std::shared_ptr<T>, Args...>::value>::type>
            ResourceHandle(Args&&... args): _res(std::forward<Args>(args)...) {}

        public:
            template <typename U>
            auto as() const -> ResourceHandle<U>;

            auto getResource() const -> T*;

            auto use_count() const -> long;
            auto reset() -> void;
            auto get() const -> WrappedType*;

            auto operator=(const std::shared_ptr<T>& rhs) -> ResourceHandle<T>&;
            auto operator=(const ResourceHandle<T>& rhs)  -> ResourceHandle<T>&;
            auto operator*()        -> WrappedType&;
            auto operator*() const  -> const WrappedType&;
            auto operator->() const -> WrappedType*;
            explicit operator bool() const;

        private:
            // This is only called in dereference functions, because
            // BaseResource handles cannot be dereferenced, but it's fine to
            // have a BaseResource handle.
            constexpr bool _assertBase() const
            {
                static_assert(!std::is_same<T, BaseResource>::value, "Can't dereference BaseResource handles.");
                return true;
            }

        private:
            std::shared_ptr<T> _res;
    };


    /*
     * Base class for resources.
     * If you creae your own resource, don't derive from BaseResource!
     * Use Resource instead, otherwise things might break.
     */
    class BaseResource : public Identifiable
    {
        public:
            // Will never actually be used, it's just there to keep the
            // compiler happy.
            typedef BaseResource WrappedType;
            typedef ResourceHandle<BaseResource> Handle;

            virtual ~BaseResource() {}
    };

    typedef BaseResource::Handle BaseResourceHandle;


    /*
     * Container for the actual resource data, based on BaseResource.
     * It requires the resource data and an ID as template parameters.
     */
    template <typename T, ID idval>
    class Resource : public Identifier<idval, BaseResource>
    {
        public:
            typedef ResourceHandle<Resource<T, idval>> Handle;
            typedef T WrappedType;

        public:
            Resource() {}

            template <typename... Args, typename = typename std::enable_if<std::is_constructible<T, Args...>::value>::type>
            Resource(Args&&... args) : res(std::forward<Args>(args)...) {}

            virtual ~Resource() {}

            template <typename... Args>
            static Handle create(Args&&... args)
            {
                return std::make_shared<Resource<T, idval>>(
                        std::forward<Args>(args)...);
            }

        public:
            T res;
    };
}

#include "Resource.inl"

#endif
