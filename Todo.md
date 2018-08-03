# Todo

* FPS scaling
* Render parent like GroupTransform but for RenderComponents
* Give resources a type name member
* ResourceManager: load all files from a folder
* Automatically adapt (render size and) aspect ratio on window resize
* ScopedIterator
* move utils in a separate git repo
* Background entity
* SlotMap size()
* imgui dialogues
* entity tags
* inputBitflags() exclude list
* loadFromJson update bool to differentiate between load and update from json
* MaskComponent
* json diff

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
  * Origin

* InputSystem
  * map["shoot"] = { Space, Enter, ... }
  * functions for checking multiple inputs at once with "all pressed" or "any pressed"

* components
  * Automatically register components to factory
    * component factory as static Component member
  * migrate components to property system
    * UpdateComponent
    * RenderComponent
      * Scene should store and update a string array of layer names that can be used as hints for the layer property
    * BrushComponent
    * ...
  * component flags:
    * unused (strip component in export)
    * child (e.g. SpriteComponent instatiates a child UpdateComponent)
    * hidden (add to entity but hide to the outside (useful for auto generated editor components))
    * transformable

* Camera
  * base Camera on sf::View
  * give Camera an active flag

* math
  * Generic polygon functions that accept pointers and offsets to compute polygon stuff
    * allows running functions on any data layouts (vertex buffers, raw point data, ...)
  * Fix pointDirection in cppmath (Vector.angle() works correctly)
  * use inheritance for polygon classes

* Q-Stuff
  * fix slope corners
    * averaging normals might not be the best solution
  * add a non-gravity version
  * fix sharp corners (<90°)

* collisions
  * Remove Collidable and merge it into CollisionComponent
  * Return TraceResult in all line checks

* rendering
  * Consider making Renderable::render const
  * render offset shader
  * render repeat shader (for texture regions)
  * Scene force redraw
  * SceneObject should not render itself
    * contains vertices, texture, shader, transform, ...
    * rendered by scene
    * optionally static render function to render outside of a scene

* editor
  * grid numbers
  * mouse coordinates
  * automatically add sprite and mask to entities without rendering
  * toolbox class
  * custom entities
    * entity flag to load an entity from savefile directly
  * RectBrush
  * PhysicsBrush
  * copy entity
  * fix behaviour in negative coord space
  * BrushTool solid option

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


<!-- vim: tabstop=2 shiftwidth=2 -->
