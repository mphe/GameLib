# Todo

## Priority

* Component
  * Implement init()/quit() wrapper around \_init()/\_quit() that sets an initialized bool
  * Add isInitialized() function
  * Add enable()/disable() function
* Return bbox copies instead of references in getBBox()
* Use new name assignment system
* ResourceManager
  * Implement multiple searchpath
  * Revert relative loads
* Make components more interface-y to prevent diamond problems

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
* option to specify base entity in entity configs
* use override keyword
* move all resources to a "coreres" folder to allow easy symlinking when using the engine in a real project
* Message System
* give flags a fixed width integer (int32, int64)

* provide failsafe implementations in core subsystems
  * no crash when trying to create unknown entities/components
  * no crash with missing components
  * etc

* config normalization problem
  * related to c6d84f340805c911d58c15af3d198d16f56ffa2d
  * entity configurations are usually handwritten and therefore
  * entity configurations usually only include the properties that changed from the default
  * properties can be user extended and can therefore have basically custom format/interpretations
    * e.g. PropComponent uses a string with the referenced component name
  * the configuration (diff) must be normalized in order to be used for further diffing and map saving

  * normalization process:
    * create new entity
    * load config
      * entity has now the config values
    * write config
    * the written config is normalized because it is machine generated and contains all possible properties

  * problem:
    * to make it work completely, components have to be created, too
    * but: normalization should not modify the game state in a visible way
      * component should not be initialized
      * unknown what happens if it was initialized
        * e.g. could execute game changing code
    * unknown what happens if the properties are accessed (read/write) without component initialization

  * solutions:
    * add standard
      * ensure component manipulation does not alter the gamestate
        * safely call Component::init()/quit() without side effects
        * probably best option as it should be possible to activate/deactivate entities and components ingame or in editor without risking component data loss
      * allow no ambiguous properties and config values
    * introduce another virtual function to IPropType to normalize a given json
      * json normalize function calls the IPropType::normalize function if possible to normalize the user part


RenderSystem:
  * Write an IPointSet adapter to edit meshes

  * Consider splitting camera functionality to other class
    * Scene
      * stores cameras
      * uses render hierachy from RenderSystem
      * determines visibility
      * renders for each camera
        * render call to RenderSystem with camera rect?

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

* create documents defining standard behaviour and templates
  * .hpp/.cpp template for new component
  * properties must always be readable/writable
    * even if the component is not initialized
    * even if the component is quit()ed
    * must not have side effects if component is not initialized
    * must not crash if component is not initialized
    * must reflect changes
    * if the component requires a system to store its data, it must be initialized, otherwise undefined behavior
      * e.g. RenderComponent needs a running RenderSystem to store its render data
    -> can help to solve c6d84f340805c911d58c15af3d198d16f56ffa2d
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

* ECS parent system
  * maybe new base class: Parentable
  * provide new virtual functions
    * setParent(parent) -> ptr:  sets the parent and returns or pointer or nullptr if not possible
    * tryParent(parent) -> bool: checks if setting this parent is possible
  * components and entities implement this interface
  * what the entity/component does with the parent is implementation dependant
    * Inherit transformation
    * Inherit flags
  * consider making all components use the parent transform (as in unity)
    * no child transforms
    * pros
      * simplifies system
      * doesn't require maintaining another transform-parent system (for components)
    * cons
      * requires a prefab loading system
        * loading/saving entity hierachies, not just whole levels
    * maybe keep both but don't implement parent system for components?


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

* Camera rewrite
  * base Camera on sf::View
  * give Camera an active flag
  * inherit from Transformable
  * rename to View to make clear it needs a controller to control its movement
  * https://www.reddit.com/r/gamedev/comments/4o7w59/how_do_you_managing_scenes_with_a_dod_approach/d4aip2m/


* imgui
  * imgui game state
  * imgui dialogues
  * disabled widgets
    * update imgui to upstream
    * https://github.com/ocornut/imgui/issues/1889

* classes
  * LineRenderer class
  * EntityRef/ComponentRef/EntCompRef class
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

* Support rendering to OSB in Game class

* json
  * loadFromJson update bool to differentiate between load and update from json
  * remove clear argument in json functions, except for Transform
  * add docs to make clear how functions behave in case of wrong json node type and defaults
  * use pointers in all loadFromJson functions instead of references
  * improve camera serialization
    * should be done by Camera not Scene

* problems with lazy entity bounding box calculation
  * storing the bbox as reference
  * scale
  * bbox doesn't adapt, because it's not evaluated
  * should either return as copy or immediatelly evaluate

* ResourceManager:
  * load all files from a folder
  * recursively load-once resource files
  * write loaders for various config files
  * revert back to loading only from searchpath
    * makes things simpler
    * less path hell
  * support multiple searchpaths
    * loading a resource will try all searchpaths from newest to oldest to find the file
    * allows easy extending and soft-overriding resources

* EntityFactory
  * handle json return values
  * query ResourceManager if entity name not found

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
    * PolygonShape -> BrushRenderer
  * include origin for sprites in ActorComponent
  * fix scale in ActorComponent to match the actual max scale and not 1
  * components should be more abstract
    * don't inherit Transformable
  * allow sub-IDs
    * allows differentiating between different specializations of the same base component
      * e.g., UpdateComponent and PhysicsComponent
    * can use a mask for searching by ID to specify which abstraction layer is wanted
  * rename Component::init/quit to enable/disable

* math
  * get convex hull function
  * convex optimize line vs polygon test
  * use Point for AABB.pos
  * support sweep against filled polygon
  * consider removing the \_add, \_edit, \_remove, ... variants in BasePointSet and BasePolygon
  * fix typename errors in Vector.inl

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
  * return a copy of the bbox -> solves the lazy evaluate problem

* rendering
  * Consider making Renderable::render const
  * render offset shader
  * render repeat shader (for texture regions)
  * Scene force redraw
  * remove json functionality from SceneObject and SceneData
    * SceneObject should be a data class
    * saving/loading transform, layer, etc, but not vertices is not particulary style-consistent as it saves everything else

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
  * remove custom resource related register functions
  * support getters
  * Consider moving read/write functions to IPropType
    * Makes separation between IPropType and PropertyHandle obsolete
    * Make whole property system OOP based
    * Probably cleaner
    * Performance negligible

* log
  * warn if null
  * assert


<!-- vim: tabstop=2 shiftwidth=2 wrap
-->
