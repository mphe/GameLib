# Todo

* FPS scaling
* origin in Transformable
* Give Layer a name member
* Render parent like GroupTransform but for RenderComponents
* Give resources a type name member
* ResourceManager: load all files from a folder
* Automatically register components to factory
* Automatically adapt (render size and) aspect ratio on window resize

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
  * Fix aircontrol
  * Add alternative QController for better platformer controls
  * (Give QPhysics its own AABB rather than relying on an existing one)

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
  * entity list

* component flags:
  * unused (strip component in export)
  * child (e.g. SpriteComponent instatiates a child UpdateComponent)
  * hidden (add to entity but hide to the outside (useful for auto generated editor components))
  * transformable

* make Engine a Subsystem
  * makes a backup of an existing active Engine
  * sets itself active on creation
  * reactivates old Engine on destruction

* Properties
  * PropIntArray, PropFloatArray, ... instead of PropVec2i, PropVec2f, ...
  * global properties component
  * use properties in RenderComponent
  * imgui sliders for properties with min-max bounds
  * property docs


* (Maybe fixed) Fix sprite flickering
  * Reproducing:
    * Use default QPhysics values (gravity, etc)
    * Build testmap
    * Set to fullscreen
    * Jump repeatedly
