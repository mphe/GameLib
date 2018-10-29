# Todo

* FPS scaling
* use median frametime to prevent stuttering
* Render parent like GroupTransform but for RenderComponents
* Give resources a type name member
* Automatically adapt (render size and) aspect ratio on window resize
* ScopedIterator
* move utils in a separate git repo
* Background entity
* SlotMap size()
* imgui dialogues
* entity tags
* inputBitflags() exclude list
* MaskComponent
* imgui game state
* replace std::unordered_map with a better implementation
* return nullptr when dereferencing BaseResource instead of using static_assert
* EntityRef/ComponentRef/EntCompRef class
* shorten/rename component names
  * AABBComponent -> AABB
  * PolygonShape -> BrushRenderer
* LineRenderer class

* json
  * loadFromJson update bool to differentiate between load and update from json
  * make JsonSerializer::write\* functions const
  * remove clear argument in json functions, except for Transform
  * add docs to make clear how functions behave in case of wrong json node type and defaults

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
    * UpdateComponent
    * RenderComponent
      * Scene could store and update a string array of layer names that can be used as hints for the layer property
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
  * PhysicsBrush
  * fix behaviour in negative coord space
  * EditorShared -> EditorContext
    * passed to every tool callback
  * region select
  * Fix negative scalebox in SelectTool
  * switch between global and local transform input

* make Engine a Subsystem
  * makes a backup of an existing active Engine
  * sets itself active on creation
  * reactivates old Engine on destruction

* Properties
  * PropIntArray, PropFloatArray, ... instead of PropVec2i, PropVec2f, ...
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
  * static map with id -> struct { imguicb, jsonwritecb, jsonloadcb }


<!-- vim: tabstop=2 shiftwidth=2 
-->
