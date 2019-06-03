#ifndef GAMELIB_RESOURCE_HPP
#define GAMELIB_RESOURCE_HPP

#include <memory>
#include <string>
#include "gamelib/utils/Identifier.hpp"

namespace gamelib
{
    class BaseResource;

    template <typename T, ID id>
    class Resource;


    /*
     * A handle to a Resource.
     *
     * T must be either BaseResource or Resource<...>!
     * If this is not the case, things might break!
     *
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
        static_assert(std::is_base_of<BaseResource, T>::value, "Type must of type Resource");

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
            auto as() const     -> ResourceHandle<U>;
            auto asBase() const -> ResourceHandle<BaseResource>;

            auto asBasePtr() const -> const ResourceHandle<BaseResource>*;
            auto asBasePtr() -> ResourceHandle<BaseResource>*;

            auto getResource() const -> T*;

            auto use_count() const -> long;
            auto reset() -> void;
            auto get() const -> WrappedType*;

            auto operator=(const std::shared_ptr<T>& rhs) -> ResourceHandle<T>&;
            auto operator=(const ResourceHandle<T>& rhs)  -> ResourceHandle<T>&;
            auto operator*()        -> WrappedType&;
            auto operator*() const  -> const WrappedType&;
            auto operator->() const -> WrappedType*;
            auto operator==(const ResourceHandle<T>& rhs) const -> bool;
            auto operator!=(const ResourceHandle<T>& rhs) const -> bool;

            explicit operator bool() const;

            template <typename T2>
            operator ResourceHandle<T2>() const;

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
        friend class ResourceManager;

        template <typename T, ID>
        friend class Resource;

        public:
            // Will never actually be used, it's just there to keep the
            // compiler happy.
            typedef BaseResource WrappedType;
            typedef ResourceHandle<BaseResource> Handle;

            BaseResource() {};
            BaseResource(const std::string& path, const std::string& searchpath) :
                _path(path), _searchpath(searchpath) {}
            virtual ~BaseResource() {}

            inline auto getFullPath() const -> std::string
            {
                if (_searchpath.empty())
                    return _path;
                return _searchpath + "/" + _path;
            }

            inline auto getPath() const -> const std::string&
            {
                return _path;
            }

            inline auto getSearchpath() const -> const std::string&
            {
                return _searchpath;
            }

        private:
            std::string _path;
            std::string _searchpath;
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
            template <typename... Args, typename = typename std::enable_if<std::is_constructible<T, Args...>::value>::type>
            Resource(Args&&... args) :
                res(std::forward<Args>(args)...)
            {}

            virtual ~Resource() {}

            template <typename... Args>
            static Handle create(Args&&... args)
            {
                return std::make_shared<Resource<T, idval>>(
                        std::forward<Args>(args)...);
            }

            template <typename... Args>
            static Handle createWithPath(const std::string& path, const std::string& searchpath, Args&&... args)
            {
                auto handle = create(std::forward<Args>(args)...);
                handle.getResource()->_path = path;
                handle.getResource()->_searchpath = searchpath;
                return handle;
            }

        public:
            T res;
    };
}

#include "Resource.inl"

#endif
