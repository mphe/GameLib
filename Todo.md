# Todo

## Priority

* adapt default snap distance in player entity
* property
    * property flags (readonly!)
    * uint property
* Component
    * Add enable()/disable() function
* Make components more interface-y to prevent diamond problems
    * remove Transform inheritance in base component classes
* use PropComponent in QPhysics for hull

## gamejam inspired
* move Component to component/ subdir
* generic Controller class
    * virtual handleInput()
        * handles physical button presses
        * writes to a bitflag member
    * virtual inputToState()
        * uses button bitflags to determine state
    * virtual getState() -> int/string
* generic Actor class
    * takes a Controller component
    * uses its getState() to determine correct sprite
* sprite padding
* entity custom names / tags
* vibrate component
* tracker component
* sound system
* collision layers and layer masks
* cameras as resources

## All

* FPS scaling
* use median frametime to prevent stuttering
* Give resources a type name member
* move utils in a separate git repo
* entity tags
* inputBitflags() exclude list
* replace std::unordered_map with a better implementation
* return nullptr when dereferencing BaseResource instead of using static_assert
* font resource
* implement own frame capping mechanic to allow better time measurement
* move all resources to a "coreres" folder to allow easy symlinking when using the engine in a real project
* Message System
* give flags a fixed width integer (int32, int64)
* consistently use pointers for arguments that are modified in the function
* Give GameState a virtual handleInput() function
    * Game calls this function for every event
* use TextureResource::Handle in PixelCollision
* Support rendering to OSB in Game class
* Delay entity destroy to begin frame or end frame to prevent possible segfaults
* Aseprite import
* Unregister properties from base classes if not needed
    * e.g. SpriteComponent doesn't need RenderComponents's "texture" property, because it defines its own "sprite"
* Hierachy clone
* EventManager: Allow respective event pointer in callbacks instead of EventPtr

RenderSystem:
    * Write an IPointSet adapter to edit meshes
    * Wireframe support

* Entity: remove deprecated getEntity() stuff

* provide failsafe implementations in core subsystems
    * no crash when trying to create unknown entities/components
    * no crash with missing components
    * etc

* config normalization problem
    * related to c6d84f340805c911d58c15af3d198d16f56ffa2d
    * entity configurations are usually handwritten
    * entity configurations usually only include the properties that changed from the default
    * properties can be user extended and can therefore have basically custom format/interpretations
        * e.g. PropComponent uses a string with the referenced component name
            * can be "Component#1" or "Component" to reference the first of a kind
    * the configuration (diff) must be normalized in order to be used for further diffing and map saving

    * normalization process:
        * create new entity
        * write to json
            * json contains the default values for entity and components
        * merge config with custom json
        * the config is normalized because it is machine generated and contains all possible properties

    * problem:
        * to make it work completely, components have to be created, too
        * but: normalization should not modify the game state in a visible way

    * normalization only 100% works if the entity is created from the json to normalize and then write that entity to json again
        * must be ensured it does not alter gamestate

    * solutions:
        * add standard
            * ensure component manipulation does not alter the gamestate
        * allow no ambiguous properties and config values


* SlotMap
    * add operator->() to SlotMapIterator
    * size() function
    * consider using std::vector to store empty fields
        * nextempty var in each field is unnecessary memory overhead
    * specify key size in terms of bits
        * sum is always sizeof(int)
        * version size is the difference
        * provide function to get key/version combined as int
    * add function to get by index disregarding version
    * overwrite on delete only if not trivially destructable
    * base on BatchAllocator


* create documents defining standard behaviour and templates
    * .hpp/.cpp template for new component
    * pointers from Component::getTransform() are volatile and could change over time


* more efficient transformable system
    * remove getBBox()
        * move it to new class
    * TransformableSystem
        * keeps track of Transformables
        * stored in a SlotMap
        * sorted in hierachical order
    * classes that use Transformables reference them using pointers
    * split classes
        * Transform
            * contains data (matrix, pos, scale, angle, ...)
        * Transformable
            * references a Transform
            * reference by handle or instantiates a Transform
            * acts as a client to Transform


* remove ambiguous component referencing
    1. use a component (A) with PropComponent
    2. use another component (B)
    3. in the entity cfg, reference B in A by using "B" instead of "B#1"
    4. save game
    5. the output contains the reference, eventhough nothing changed

    * Component#1 == Component
    * but during diffing or normalizing phase, they are treated as different
    * this leads to incorrect diffing or normalizing configs, because it can't normalize the user written part
    * results in the reference (Component#1) being written to json because it differs from user config (Component)

    * possible solutions
        * introduce another virtual function to IPropType to normalize a given json
            * json normalize function calls the IPropType::normalize function if possible to normalize the user part
        * force usage of Component#ID notation and drop support for short notation
        * use a new naming scheme for components that removes ambiguity


* find a way to inform references about component removal
    1. somewhere a pointer to a component is stored
    2. the component gets deleted
    3. the reference holder segfaults on next access
    * could be done by a weak_ptr equivalent for unique_ptr
        * store components as shared_ptr?
    * check validity in \_refresh()
        * wouldn't work if the reference is not held by a component or it references a component from another entity
    * use a ComponentRemoved event
        * callback handler + pointer to property

* imgui
    * imgui dialogues

* classes
    * LineRenderer class
    * Render parent like GroupTransform but for RenderComponents
    * ScopedIterator
    * Background entity
    * PhysicsBrush
    * Motion
        * base class to describe different motions
        * has an update function
        * provides geometry (pos, scale, angle) and time
        * can be passed (as unique_ptr/shared_ptr) to other objects (e.g. Camera)
        * can be combined with other motion objects to create complexer motions, e.g. SinMotion + LinearMotion
        * e.g. LinearMotion (move in a direction), TargetMove (move from a to b), ...
    * Split Updatable in Updatable and BaseUpdatable
        * BaseUpdatable <- Updatable
        * BaseUpdatable <- PhysicsComponent
    * CameraComponent

* json
    * loadFromJson update bool to differentiate between load and update from json
    * remove clear argument in json functions, except for Transform
    * add docs to make clear how functions behave in case of wrong json node type and defaults
    * use pointers in all loadFromJson functions instead of references
    * reset filename when JsonSerializer::loadFromJson() is called without loadFromFile

* ResourceManager:
    * load all files from a folder
    * recursively load-once resource files
    * write loaders for various config files

* SpriteComponent
    * loadFromJson fix overrides by tmp saving overrides and applying afterwards (again)

* Transformable
    * Consider removing GroupTransform and instead give all Transformables a vector of children
    * remove getBBox() and instead introduce a Geometry interface for this

* InputSystem
    * map["shoot"] = { Space, Enter, ... }
    * functions for checking multiple inputs at once with "all pressed" or "any pressed"
    * isKeyDown/isMouseDown should check directly instead of going through getKeyState()

* components
    * component flags:
        * unused (strip component in export)
        * child (e.g. SpriteComponent instatiates a child UpdateComponent)
        * hidden (add to entity but hide to the outside (useful for auto generated editor components))
        * custom (load component directly from json (useful when adding components in the editor that aren't part of the entity))
    * shorten/rename component names
        * AABBComponent -> AABB
    * include origin for sprites in ActorComponent
    * fix scale in ActorComponent to match the actual max scale and not 1
    * allow sub-IDs
        * allows differentiating between different specializations of the same base component
            * e.g., UpdateComponent and PhysicsComponent
        * can use a mask for searching by ID to specify which abstraction layer is wanted
        * HierachyInfo class
            * inherit Identifiable
                * virtual getID()
            * virtual getParent()
            -> recursively iterate through hierachy and get IDs

* math
    * get convex hull function
    * convex optimize line vs polygon test
    * use Point for AABB.pos
    * support sweep against filled polygon
    * consider removing the \_add, \_edit, \_remove, ... variants in BasePointSet and BasePolygon

* Physics
    * fix slope corners
        * averaging normals might not be the best solution
    * add a non-gravity version
    * fix sharp corners (<90°)
    * if there's no collision component assigned, don't do clip checking, but don't disable all physics
    * add option to accelerate towards basevel rather than immediatelly setting it
    * move airFriction code to QController
    * make snap distance fps independent
        * snap only if speed and gravity point in the same direction

* collisions
    * Remove Collidable and merge it into CollisionComponent
    * Return TraceResult in all line checks
    * Merge CollisonSystem and physics to PhysicsSystem
        * Moving objects can cause collisions along the way with non-solid objects
        -> handling physics should not be separated from collisions 
        * https://www.gamedev.net/forums/topic/669494-dealing-with-different-collision-responses-in-an-entity-component-system/

* rendering
    * Consider making Renderable::render const
    * render offset shader
    * render repeat shader (for texture regions)
    * Scene force redraw

* editor
    * grid numbers
    * automatically add sprite and mask to entities without rendering
    * toolbox class
    * add option to add/remove components from entities
    * fix behaviour in negative coord space
    * EditorShared -> EditorContext
        * passed to every tool callback
    * region select
    * Fix negative scalebox in SelectTool
    * save/restore editor settings (when running/returning to editor)
        * render options (parallax, etc)
        * camera position
    * add Tool::getName() to get rid of the static tool name array in Editor.cpp

* make Engine a Subsystem
    * makes a backup of an existing active Engine
    * sets itself active on creation
    * reactivates old Engine on destruction

* Properties
    * imgui sliders for properties with min-max bounds
    * property docs
    * string hints
        * dropdown menu for strings
    * flags
        * override
        * readonly
        * filename
        * notzero
        * guarantee load first (-> race condition in SpriteComponent)
        * dummy (makes PropDummy obsolete)
    * ConstPropertyHandle
        * use in BasePropType loadFromJson, drawGui
    * PropComponent needs to react if the component list changes
    * rewrite resource property to be safer
        * resource handle gets reinterpret_casted to BaseResourceHandle
        * works fine as long as the resource is of type Resource<T>
        * if not, things might break if the resource type uses multiple inheritance

* log
    * warn if null
    * assert
    * live update support (e.g. currently rendered instances)
        * "log entry\r" fix


<!-- vim: wrap
-->
