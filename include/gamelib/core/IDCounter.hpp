#ifndef GAMELIB_ID_COUNTER_HPP
#define GAMELIB_ID_COUNTER_HPP

#include "Identifiable.hpp"
#include "res/JsonSerializer.hpp"
#include "gamelib/utils/log.hpp"


namespace gamelib
{
    template <typename T, ID offset = 0>
    class IDCounter : public Identifiable, public JsonSerializer
    {
        public:
            static constexpr ID IDOffset = offset;

        public:
            IDCounter();
            virtual ~IDCounter() {};

            auto invalidateID() -> void;
            auto getID() const -> ID;

            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

        private:
            // Maybe make this public if really needed, but seems dangerous to do so
            auto _assignID(ID id) -> void;

        private:
            ID _id;

        private:
            static ID _counter;
    };

    typedef IDCounter<void> GlobalIDCounter;


    template <typename T, ID offset>
    ID IDCounter<T, offset>::_counter = offset;


    template <typename T, ID offset>
    IDCounter<T, offset>::IDCounter()
    {
        _assignID(_counter);
    }

    template <typename T, ID offset>
    void IDCounter<T, offset>::_assignID(ID id)
    {

        _id = id;

        if (id == invalidID)
            LOG_WARN("Assigning invalid ID: ", FUNC_STRING);
        else if (id >= _counter)
            _counter = id + 1;
    }

    template <typename T, ID offset>
    void IDCounter<T, offset>::invalidateID()
    {
        _id = invalidID;
    }

    template <typename T, ID offset>
    ID IDCounter<T, offset>::getID() const
    {
        return _id;
    }

    template <typename T, ID offset>
    bool IDCounter<T, offset>::loadFromJson(const Json::Value& node)
    {
        _assignID(node.get("uid", invalidID).asUInt());
        return true;
    }

    template <typename T, ID offset>
    void IDCounter<T, offset>::writeToJson(Json::Value& node)
    {
        node["uid"] = getID();
    }
}

#endif
