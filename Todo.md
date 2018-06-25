# Todo

* FPS scaling
* Render parent like GroupTransform but for RenderComponents
* Give resources a type name member
* ResourceManager: load all files from a folder
* Automatically adapt (render size and) aspect ratio on window resize
* ScopedIterator
* move utils in a separate git repo
* handle json return values in EntityFactory
* SpriteBrush
* Background entity
* SlotMap size()
* imgui dialogues

* Transformable
  * Add parent reference to update parent bbox on transformation
  * Consider removing GroupTransform and instead give all Transformables a vector of children
  * Origin

* InputSystem
  * map["shoot"] = { Space, Enter, ... }

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

* Q-Stuff
  * PhysicsHull flag for collidables so physics can automatically find the correct collision component
  * Fix right movement faster than left
  * Add alternative QController for better platformer controls
    * Fix aircontrol

* collisions
  * Remove Collidable and merge it into CollisionComponent

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

* Fix sprite flickering (maybe fixed)


<!--- vim: tabstop=2 shiftwidth=2 --->
