#ifndef GAMELIB_UPDATE_COMPONENT_HPP
#define GAMELIB_UPDATE_COMPONENT_HPP

#include "Component.hpp"

/*
 * UpdateComponent inherits from Component and adds a wrapper around the
 * init(Subsystem*) function (init(UpdateSystem*)) to perform an assert ID check
 * to make sure the passed subsystem is an UpdateSystem. 
 * For easier usage it casts the Subsystem pointer to a UpdateSystem pointer.
 * 
 * It also declares a _subsys member variable that stores the UpdateSystem
 * in which the component is registered. It's automatically set in the
 * init wrapper function.
 */

namespace gamelib
{
    class UpdateSystem;

    class UpdateComponent : public Component
    {
        public:
            UpdateComponent();

            // Calls destroy().
            virtual ~UpdateComponent();

            // Wrapper around init(Subsystem*). Sets _subsys member to usystem.
            virtual bool init(UpdateSystem* usystem);

            // Unregisters from subsystem (_subsys).
            virtual void destroy();

            virtual void update(float fps) = 0;

        protected:
            UpdateSystem* _subsys;

        private:
            // Performs a Subsystem ID check and calls init(UpdateSystem*)
            bool init(Subsystem* ssystem);
    };
}

#endif
