# Components

## Initialization
To use components the following criteria must be met:

* Components must be initialized before use using `Component::init()`
* The component must be added to an entity

If these criteria are not met, the results of using the component in any way, are undefined behavior. There are, however, some exceptions detailed below.

After calling `Component::quit()`, the component is treated as not initialized.
If `init()` fails, the component is not initialized.
`init()` and `quit()` check the initialization status and don't run if already initialized or already quitted. It is therefore safe to call `init()` or `quit()` multiple times. This does not apply to the protected `_init()` and `_quit()`.

Both `init()` and `quit()` must work without being attached to an entity.

#### Member access
After successful `init()` the component should be in a default state, where all properties must be registered and readable. All other members must also be readable but not necessarily modifiable.
All properties should now hold their default values.
Attempting to modify the component during this state in any way (members, properties, non-const functions) results in undefined behavior.

It is therefore safe to use functions like `writeToJson()` or reading properties and other members.

After the component has been added to an entity, the component can be used in a normal way without side effects.

`Entity` takes care of calling `init()` and `quit()` when adding or removing components.


#### Implementation info
The order of initialization/quitting and adding/removing to an entity is crucial.
The component must be initialized first, then added to the entity.
Doing this out of order, results in undefined behavior.

This can be seen, for example, with `RenderComponent`s. If the inherited `Transformable` changes, the component will update the transform matrix allocated in the `RenderSystem`. If the component was not yet initialized to this point, it will segfault.
Adding a component to an entity triggers a `Transformable` change, as the component is added as child to the entity's `Transformable`. It is therefore necessary to first initialize and then register to the entity transform afterwards.


#### Summary

|                               | Construction | Initialized | Added to entity |
|-------------------------------|:------------:|:-----------:|:---------------:|
| **All properties registered** |      No      |     Yes     |       Yes       |
| **`writeToJson()`**           |      No      |     Yes     |       Yes       |
| **Reading**                   |      No      |     Yes     |       Yes       |
| **`drawGui()` or similar**    |      No      |      No     |       Yes       |
| **Writing**                   |      No      |      No     |       Yes       |
