# Entity Serialization

## Saving
Entities can be saved in two ways:
* Full - Save all properties
* Delta - Save only changed properties

The full save is usually not needed.

Delta saving works as follows:
* Normalize entity template
* Full serialize entity instance
* Diff normalized vs full instance json
* Add "name" field to diffed json

### Normalization

The normalization step produces a json config that contains all properties available in this entity and its components.
This step is needed to get the default values of all properties, in order to find the differences.

An entity is usually registered to EntityFactory with a (user-written) json template, which is often not complete, meaning, not all properties are defined, only those that require custom values.

Since entity templates should never require special tools and should always be writable manually by user hand, this is rather the rule than an exception.


### Delta-ing

To compute the deltas, the entity is fully serialized with all its properties and then recursively diffed against its normalized template.
This produces an entity dict contained all changed values.


### Fixup
The last step writes the entity's name to the delta dict in order to ensure there will always be the correct entity name and never an empty dict.


## Loading

Loading works as follows:
* Load json config
* Query EntityFactory if an entity template exists with the given name
    * Yes -> Instantiate entity by template
    * No -> return false
* Merge json config into entity
    * Overwriting everything, including name
    * Merge component config only if a matching component already exist
        * Don't create components if they don't exist in entity
            * Except if argument given to `extendFromJson` says to do so
