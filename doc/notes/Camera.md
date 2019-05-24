# Camera

* Cache efficiency shouldn't matter because there will very rarely be more than 1-2 cameras

* Adding/Removal without invalidating existing handles
    -> plain vector won't work
    * SlotMap
    * vector of pointers

* (Unique) Identifier that persists across restarts
    -> SlotMapKey won't work
    -> Index only works locally in Scene's config
    * string
        * user/debugging friendly
    * ID
        * can be auto generated -> IDCounter
        * less user/debugging friendly

* Ownership
    * Owned by Scene
        * Allows easy serialization as no polymorphism is involved
        * Necessary to provide a way to uniquely identify Cameras across restarts
        * To control cameras a "camera controller" is needed
        -> don't expose pointer based approach so the user doesn't think it supports polymorphism
    * Not owned by Scene
        * Scene stores pointers to Camera objects
        * Cameras are instantiated e.g. by entities and a CameraComponent
        -> Relies on having a Camera entity
        -> Serialization not necessarily automated
        -> allows polymorphism
        -> identifying Cameras is not related to Scene anymore

* Access
    * by handle -> returns pointer
    * by identifier -> returns handle/pointer
    * by index -> returns handle/pointer
        * not available in SlotMap

# Conclusions
* gamemaker style view system

* if owned outside scene
    * vector of pointers
    * no serialization by Scene
        -> camera property changes must be saved manually
    * identifier support not coupled to Scene
        * but: identifier support needed anyway
        * requires additional functionality to query cameras by name
    * camera entity needed for editor support
    * needs Transformable support for Camera

* if owned by scene
    * storage
        * slotmap
            * using an array of unique_ptr seems like unnecessary overhead
            * no polymorphism supported anyway
            * but: access only through handles, as a reallocation could invalidate pointers
        * array of ptr
            * nice and simple access
                * index
                * pointer
            * but: a bit ugly to have an array of pointers
    * some identifier needs to be supported
        -> string
    * automated serialization
    -> simpler
    -> cleaner
    -> implemented with array of ptr approach for better access
