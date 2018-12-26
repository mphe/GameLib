# Todo

* FPS scaling
* use median frametime to prevent stuttering
* Give resources a type name member
* move utils in a separate git repo
* SlotMap size()
* entity tags
* inputBitflags() exclude list
* replace std::unordered_map with a better implementation
* return nullptr when dereferencing BaseResource instead of using static_assert
* font resource
* implement own frame capping mechanic to allow better time measurement
* option to specify base entity in entity configs
* use override keyword

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

  * RenderSystem
    * stores cameras
    * uses render hierachy from Scene
    * determines visibility
    * renders for each camera
    * Scene
      * no longer renders
      * keeps track of objects to be rendered
      * manages hierachy (layers, depth)


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

* EntityFactory
  * handle json return values
  * query ResourceManager if entity name not found

* SpriteComponent
  * loadFromJson fix overrides by tmp saving overrides and applying afterwards (again)

* Transformable
  * Consider removing GroupTransform and instead give all Transformables a vector of children

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

* math
  * Generic polygon functions that accept pointers and offsets to compute polygon stuff
    * allows running functions on any data layouts (vertex buffers, raw point data, ...)
  * use inheritance for polygon classes
  * PolygonCollider (interface/abstract)
    * intersect
    * foreach
    * find nearest
    * derived:
      * TriangleStripCollider
      * LineStripCollider
      * ...
  * LineStrip polygon closed flag

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
  * rename Property.hpp/cpp to PropertyContainer.hpp/cpp

* log
  * warn if null
  * assert


<!-- vim: tabstop=2 shiftwidth=2 wrap
-->
