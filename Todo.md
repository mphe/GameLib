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
* shorten/rename component names
  * AABBComponent -> AABB
  * PolygonShape -> BrushRenderer
* include origin for sprites in ActorComponent
* fix scale in ActorComponent to match the actual max scale and not 1
* font resource

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
  * MaskComponent
  * PhysicsBrush
  * Motion
    * base class to describe different motions
    * has an update function
    * provides geometry (pos, scale, angle) and time
    * can be passed (as unique_ptr/shared_ptr) to other objects (e.g. Camera)
    * can be combined with other motion objects to create complexer motions, e.g. SinMotion + LinearMotion
    * e.g. LinearMotion (move in a direction), TargetMove (move from a to b), ...


* Support rendering to OSB in Game class

* json
  * loadFromJson update bool to differentiate between load and update from json
  * make JsonSerializer::write\* functions const
  * remove clear argument in json functions, except for Transform
  * add docs to make clear how functions behave in case of wrong json node type and defaults
  * use pointers in all loadFromJson functions instead of references

* problems with lazy entity bounding box calculation
  * storing the bbox as reference
  * scale
  * bbox doesn't adapt, because it's not evaluated
  * should either return as copy or immediatelly evaluate

* ResourceManager:
  * load all files from a folder
  * recursively load-once resource files
  * write loaders for various config files

* Split Updatable in Updatable and BaseUpdatable
  * BaseUpdatable <- Updatable
  * BaseUpdatable <- PhysicsComponent

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
  * migrate components to property system
    * BrushComponent
    * ...
  * component flags:
    * unused (strip component in export)
    * child (e.g. SpriteComponent instatiates a child UpdateComponent)
    * hidden (add to entity but hide to the outside (useful for auto generated editor components))
    * custom (load component directly from json (useful when adding components in the editor that aren't part of the entity))

* Camera
  * base Camera on sf::View
  * give Camera an active flag
  * inherit from Transformable

* math
  * Generic polygon functions that accept pointers and offsets to compute polygon stuff
    * allows running functions on any data layouts (vertex buffers, raw point data, ...)
  * Fix pointDirection in cppmath (Vector.angle() works correctly)
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
  * remove invert support in Polygon as it became obsolete with normal directions

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
  * ConstPropertyHandle
    * use in BasePropType loadFromJson, drawGui
  * remove custom resource related register functions
  * provide pointer in writeToJson for consistency

* log
  * warn if null
  * assert


<!-- vim: tabstop=2 shiftwidth=2 
-->
